package protocol;

import com.bitwig.extension.controller.api.ControllerHost;
import protocol.MessageID;
import protocol.ProtocolConstants;
import protocol.DecoderRegistry;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Protocol - Bitwig Serial8 Protocol Implementation
 *
 * MANUAL - DO NOT REGENERATE
 *
 * Supports both UDP and TCP transports, selectable via environment variable.
 * Set MIDI_STUDIO_TRANSPORT=tcp or MIDI_STUDIO_TRANSPORT=udp (default: udp)
 *
 * Usage:
 *   Protocol protocol = new Protocol(host, "127.0.0.1", 9000);
 *
 *   // Assign callbacks
 *   protocol.onTransportPlay = msg -> {
 *       System.out.println("Playing: " + msg.isPlaying());
 *   };
 *
 *   // Send
 *   protocol.send(new TransportPlayMessage(true));
 */
public class Protocol extends ProtocolCallbacks {

    // ========================================================================
    // Configuration
    // ========================================================================

    private static final int DEFAULT_BRIDGE_PORT = 9000;
    private static final String TRANSPORT_ENV_VAR = "MIDI_STUDIO_TRANSPORT";

    // ========================================================================
    // Dependencies
    // ========================================================================

    private final ControllerHost host;
    private final ProtocolTransport transport;
    private final AtomicBoolean isActive = new AtomicBoolean(true);

    // Reflection cache for performance
    // encode(byte[], int) returns bytes written - streaming, zero-allocation
    private record MessageMeta(MessageID messageId, Method encodeMethod) {}
    private final ConcurrentHashMap<Class<?>, MessageMeta> messageCache = new ConcurrentHashMap<>();

    // Pre-allocated send buffer (Bitwig is single-threaded, no need for ThreadLocal)
    private static final int MAX_SEND_BUFFER_SIZE = 4096;
    private final byte[] sendBuffer = new byte[MAX_SEND_BUFFER_SIZE];

    // ========================================================================
    // Metrics (for bottleneck analysis)
    // ========================================================================

    private static final boolean METRICS_ENABLED = true;
    private static final int METRICS_INTERVAL_MS = 1000;

    private final AtomicInteger messageCount = new AtomicInteger(0);
    private final AtomicLong byteCount = new AtomicLong(0);
    private final AtomicInteger peakMessageCount = new AtomicInteger(0);
    private final AtomicLong peakByteCount = new AtomicLong(0);
    private volatile long lastMetricsTime = System.currentTimeMillis();

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
        if (transport != null) {
            transport.close();
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

        // Determine transport type from environment
        ProtocolTransport.Type transportType = getTransportTypeFromEnv();
        host.println("[Protocol] Using transport: " + transportType);

        // Create transport
        ProtocolTransport.ReceiveCallback callback = this::dispatch;

        if (transportType == ProtocolTransport.Type.TCP) {
            this.transport = new TcpTransport(host, bridgeHost, bridgePort, callback);
        } else {
            this.transport = new UdpTransport(host, bridgeHost, bridgePort, callback);
        }
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
    // Send API
    // ========================================================================

    /**
     * Send a protocol message (auto-encodes and auto-detects MessageID)
     */
    public <T> void send(T message) {
        if (!isActive.get()) return;
        sendInternal(message);
    }

    /**
     * Send a final message (bypasses isActive check, used during exit)
     */
    public <T> void sendFinal(T message) {
        if (transport == null || !transport.isConnected()) return;
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
            // Frame header: [MessageID][fromHost]
            sendBuffer[0] = meta.messageId().getValue();
            sendBuffer[1] = 1;  // fromHost = true
            // Encode payload starting at offset 2 (zero-allocation)
            int payloadLength = (int) meta.encodeMethod().invoke(message, sendBuffer, 2);
            frameLength = 2 + payloadLength;
        } catch (Exception e) {
            throw new RuntimeException("Failed to encode message: " + e.getMessage(), e);
        }

        // Send via transport (pass buffer slice without allocation)
        transport.send(sendBuffer, 0, frameLength);

        // Update metrics
        if (METRICS_ENABLED) {
            messageCount.incrementAndGet();
            byteCount.addAndGet(frameLength);
            maybeLogMetrics();
        }
    }

    /**
     * Log metrics if interval elapsed
     */
    private void maybeLogMetrics() {
        long now = System.currentTimeMillis();
        long elapsed = now - lastMetricsTime;

        if (elapsed >= METRICS_INTERVAL_MS) {
            int msgs = messageCount.getAndSet(0);
            long bytes = byteCount.getAndSet(0);
            lastMetricsTime = now;

            // Track peaks
            if (msgs > peakMessageCount.get()) {
                peakMessageCount.set(msgs);
            }
            if (bytes > peakByteCount.get()) {
                peakByteCount.set(bytes);
            }

            // Log current stats
            double kbPerSec = bytes / 1024.0;
            host.println(String.format("[Protocol] TX: %d msg/s, %.2f KB/s (peak: %d msg/s, %.2f KB/s)",
                msgs, kbPerSec,
                peakMessageCount.get(), peakByteCount.get() / 1024.0));
        }
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

        boolean fromHost = (frame[ProtocolConstants.FROM_HOST_OFFSET] != 0);

        // Extract payload
        int payloadLength = frame.length - ProtocolConstants.PAYLOAD_OFFSET;
        byte[] payload = new byte[payloadLength];
        System.arraycopy(frame, ProtocolConstants.PAYLOAD_OFFSET, payload, 0, payloadLength);

        // Dispatch to callbacks
        DecoderRegistry.dispatch(this, messageId, payload, fromHost);
    }

    // ========================================================================
    // Internal Helpers
    // ========================================================================

    /**
     * Check if connected
     */
    public boolean isConnected() {
        return transport != null && transport.isConnected() && isActive.get();
    }

    /**
     * Get current transport type
     */
    public ProtocolTransport.Type getTransportType() {
        return transport != null ? transport.getType() : null;
    }

}  // class Protocol
