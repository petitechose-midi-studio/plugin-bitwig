package protocol;

import com.bitwig.extension.controller.api.ControllerHost;
import protocol.MessageID;
import protocol.ProtocolConstants;
import protocol.DecoderRegistry;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Protocol - Bitwig Binary Protocol Implementation
 *
 * MANUAL - DO NOT REGENERATE
 *
 * Supports both UDP and TCP transports, selectable via environment variable.
 * Set MIDI_STUDIO_TRANSPORT=tcp or MIDI_STUDIO_TRANSPORT=udp (default: udp)
 *
 * Usage:
 * Protocol protocol = new Protocol(host, "127.0.0.1", 9000);
 *
 * // Assign callbacks for commands from controller
 * protocol.onTransportPlay = msg -> {
 * System.out.println("Playing: " + msg.isPlaying());
 * };
 *
 * // Send notifications to controller using explicit API
 * protocol.transportPlayingState(true);
 * protocol.deviceChange(name, enabled, ...);
 */
public class Protocol extends ProtocolCallbacks {

    // ========================================================================
    // Configuration
    // ========================================================================

    private static final int DEFAULT_BRIDGE_PORT = 9000;

    // ========================================================================
    // Dependencies
    // ========================================================================

    private final ControllerHost host;
    private volatile ProtocolTransport transport;
    private final AtomicBoolean isActive = new AtomicBoolean(true);

    // Reflection cache for performance
    // encode(byte[], int) returns bytes written - streaming, zero-allocation
    private record MessageMeta(MessageID messageId, Method encodeMethod) {}
    private final ConcurrentHashMap<Class<?>, MessageMeta> messageCache = new ConcurrentHashMap<>();

    // Pre-allocated send buffer (Bitwig is single-threaded, no need for ThreadLocal)
    private static final int MAX_SEND_BUFFER_SIZE = 4096;
    private final byte[] sendBuffer = new byte[MAX_SEND_BUFFER_SIZE];

    // ========================================================================
    // Lifecycle
    // ========================================================================

    /**
     * Deactivate the protocol (prevents sending after extension exit)
     */
    public void deactivate() {
        isActive.set(false);
    }

    /**
     * Close the connection
     */
    public void close() {
        isActive.set(false);
        ProtocolTransport current = transport;
        transport = null;
        if (current != null) {
            current.close();
        }
    }

    // ========================================================================
    // Constructor
    // ========================================================================

    /**
     * Create protocol with default port (9000)
     */
    public Protocol(ControllerHost host) {
        this(host, "127.0.0.1", DEFAULT_BRIDGE_PORT);
    }

    /**
     * Create protocol with specified host and port
     * Transport type is determined by MIDI_STUDIO_TRANSPORT env var (default: udp)
     */
    public Protocol(ControllerHost host, String bridgeHost, int bridgePort) {
        if (host == null) {
            throw new IllegalArgumentException("ControllerHost cannot be null");
        }
        this.host = host;

        this.transport = createTransport(bridgeHost, bridgePort);
    }

    /**
     * Reconnect to another bridge endpoint while keeping all registered callbacks.
     */
    public synchronized void reconnect(String bridgeHost, int bridgePort) {
        if (!isActive.get()) return;

        ProtocolTransport replacement = createTransport(bridgeHost, bridgePort);
        ProtocolTransport previous = transport;
        transport = replacement;
        if (previous != null) {
            previous.close();
        }
    }

    private ProtocolTransport createTransport(String bridgeHost, int bridgePort) {
        ProtocolTransport.Type transportType = getTransportTypeFromEnv();
        ProtocolTransport.ReceiveCallback callback = this::dispatch;

        if (transportType == ProtocolTransport.Type.TCP) {
            return new TcpTransport(host, bridgeHost, bridgePort, callback);
        }
        return new UdpTransport(host, bridgeHost, bridgePort, callback);
    }

    /**
     * Get transport type from environment variable
     */
    private ProtocolTransport.Type getTransportTypeFromEnv() {
        // TEMP: Force UDP for testing
        return ProtocolTransport.Type.UDP;

        // String envValue = System.getenv(TRANSPORT_ENV_VAR);
        // if (envValue != null && envValue.equalsIgnoreCase("tcp")) {
        //     return ProtocolTransport.Type.TCP;
        // }
        // return ProtocolTransport.Type.UDP;  // Default
    }

    // ========================================================================
    // Internal Send (used by generated explicit API methods)
    // ========================================================================

    /**
     * Send a protocol message (internal use only)
     * Use explicit API methods instead (e.g. transportPlayingState(),
     * deviceChange())
     */
    @Override
    protected void send(Object message) {
        if (!isActive.get()) return;
        sendInternal(message);
    }

    /**
     * Send a final message (bypasses isActive check, used during exit)
     */
    public <T> void sendFinal(T message) {
        ProtocolTransport current = transport;
        if (current == null || !current.isConnected()) return;
        try {
            sendInternal(message);
        } catch (Exception e) {
            // Ignore - transport may already be closed
        }
    }

    private <T> void sendInternal(T message) {
        if (message == null) {
            throw new IllegalArgumentException("Message cannot be null");
        }

        // Use cached reflection for performance
        Class<?> clazz = message.getClass();
        MessageMeta meta = messageCache.computeIfAbsent(clazz, c -> {
            try {
                Field messageIdField = c.getField("MESSAGE_ID");
                MessageID id = (MessageID) messageIdField.get(null);
                // encode(byte[], int) - streaming, zero-allocation
                Method encode = c.getMethod("encode", byte[].class, int.class);
                return new MessageMeta(id, encode);
            } catch (Exception e) {
                throw new RuntimeException("Failed to cache message type: " + c.getName(), e);
            }
        });

        int frameLength;
        try {
            // Frame header: [MessageID]
            sendBuffer[0] = meta.messageId().getValue();
            // Encode payload starting at offset 1 (zero-allocation)
            int payloadLength = (int) meta.encodeMethod().invoke(message, sendBuffer, 1);
            frameLength = 1 + payloadLength;
        } catch (Exception e) {
            throw new RuntimeException("Failed to encode message: " + e.getMessage(), e);
        }

        ProtocolTransport current = transport;
        if (current == null || !current.isConnected()) return;

        // Send via transport (pass buffer slice without allocation)
        current.send(sendBuffer, 0, frameLength);
    }

    // ========================================================================
    // Receive (Dispatch)
    // ========================================================================

    /**
     * Dispatch incoming frame to callbacks
     */
    private void dispatch(byte[] frame) {
        if (frame == null || frame.length < ProtocolConstants.MIN_MESSAGE_LENGTH) {
            return;
        }

        // Parse header
        byte messageIdByte = frame[ProtocolConstants.MESSAGE_TYPE_OFFSET];
        MessageID messageId = MessageID.fromValue(messageIdByte);
        if (messageId == null) {
            host.errorln("[Protocol] Unknown message ID: " + messageIdByte);
            return;
        }

        // Extract payload
        int payloadLength = frame.length - ProtocolConstants.PAYLOAD_OFFSET;
        byte[] payload = new byte[payloadLength];
        System.arraycopy(frame, ProtocolConstants.PAYLOAD_OFFSET, payload, 0, payloadLength);

        // Dispatch to callbacks
        DecoderRegistry.dispatch(this, messageId, payload);
    }

    // ========================================================================
    // Internal Helpers
    // ========================================================================

    /**
     * Check if connected
     */
    public boolean isConnected() {
        ProtocolTransport current = transport;
        return current != null && current.isConnected() && isActive.get();
    }

    /**
     * Get current transport type
     */
    public ProtocolTransport.Type getTransportType() {
        ProtocolTransport current = transport;
        return current != null ? current.getType() : null;
    }

}  // class Protocol
