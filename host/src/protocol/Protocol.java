package protocol;

import com.bitwig.extension.controller.api.MidiOut;
import com.bitwig.extension.controller.api.MidiIn;
import protocol.MessageID;
import protocol.ProtocolConstants;
import protocol.DecoderRegistry;

/**
 * Protocol - Bitwig Protocol Implementation
 *
 * MANUAL - DO NOT REGENERATE
 *
 * Handles transport layer (MidiOut/MidiIn) only.
 * Message callbacks are defined in ProtocolCallbacks (generated).
 *
 * Usage:
 *   Protocol protocol = new Protocol(midiOut, midiIn);
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
    // Dependencies
    // ========================================================================

    private final MidiOut midiOut;
    private volatile boolean isActive = true;

    // ========================================================================
    // Lifecycle
    // ========================================================================

    /**
     * Deactivate the protocol (prevents sending after extension exit)
     * Must be called before sending HostDeactivatedMessage
     */
    public void deactivate() {
        isActive = false;
    }

    // ========================================================================
    // Constructor
    // ========================================================================

    public Protocol(MidiOut midiOut, MidiIn midiIn) {
        if (midiOut == null) {
            throw new IllegalArgumentException("MidiOut cannot be null");
        }
        if (midiIn == null) {
            throw new IllegalArgumentException("MidiIn cannot be null");
        }
        this.midiOut = midiOut;

        // Register SysEx callback automatically (no need to store midiIn)
        midiIn.setSysexCallback(hexData -> {
            byte[] data = hexStringToByteArray(hexData);
            dispatch(data);
        });
    }

    // ========================================================================
    // Send API
    // ========================================================================

    /**
     * Send a protocol message (auto-encodes and auto-detects MessageID)
     *
     * Example:
     *   protocol.send(new TransportPlayMessage(true));
     */
    public <T> void send(T message) {
        if (!isActive) return;  // Skip if extension is deactivating
        sendInternal(message);
    }

    /**
     * Send a final message (bypasses isActive check, used during exit)
     * Catches exceptions to handle socket closed gracefully
     */
    public <T> void sendFinal(T message) {
        try {
            sendInternal(message);
        } catch (Exception e) {
            // Ignore - socket may already be closed
        }
    }

    private <T> void sendInternal(T message) {
        if (message == null) {
            throw new IllegalArgumentException("Message cannot be null");
        }

        // Auto-detect MessageID from message type using reflection
        MessageID messageId;
        byte[] payload;
        try {
            var messageIdField = message.getClass().getField("MESSAGE_ID");
            messageId = (MessageID) messageIdField.get(null);

            var encodeMethod = message.getClass().getMethod("encode");
            payload = (byte[]) encodeMethod.invoke(message);
        } catch (Exception e) {
            throw new RuntimeException("Failed to send message: " + e.getMessage(), e);
        }

        // Build SysEx frame
        byte[] sysex = buildSysExFrame(messageId.getValue(), payload);

        // Send via MidiOut
        midiOut.sendSysex(sysex);
    }

    // ========================================================================
    // Receive (Dispatch)
    // ========================================================================

    /**
     * Dispatch incoming SysEx to callbacks
     * Called automatically by SysEx receive callback
     */
    public void dispatch(byte[] sysex) {
        // Validate SysEx frame (now includes fromHost byte)
        if (sysex == null || sysex.length < ProtocolConstants.MIN_MESSAGE_LENGTH) {
            return;
        }

        if (sysex[0] != ProtocolConstants.SYSEX_START ||
            sysex[sysex.length - 1] != ProtocolConstants.SYSEX_END) {
            return;
        }

        if (sysex[1] != ProtocolConstants.MANUFACTURER_ID ||
            sysex[2] != ProtocolConstants.DEVICE_ID) {
            return;
        }

        // Extract MessageID and fromHost flag
        byte messageIdByte = sysex[ProtocolConstants.MESSAGE_TYPE_OFFSET];
        MessageID messageId = MessageID.fromValue(messageIdByte);
        if (messageId == null) {
            return;
        }

        boolean fromHost = (sysex[ProtocolConstants.FROM_HOST_OFFSET] != 0);

        // Extract payload
        int payloadLength = sysex.length - ProtocolConstants.MIN_MESSAGE_LENGTH;
        byte[] payload = new byte[payloadLength];
        System.arraycopy(sysex, ProtocolConstants.PAYLOAD_OFFSET, payload, 0, payloadLength);

        // Dispatch to callbacks with fromHost flag
        DecoderRegistry.dispatch(this, messageId, payload, fromHost);
    }

    // ========================================================================
    // Internal Helpers
    // ========================================================================

    private byte[] buildSysExFrame(byte messageId, byte[] payload) {
        int totalSize = ProtocolConstants.MIN_MESSAGE_LENGTH + payload.length;
        byte[] sysex = new byte[totalSize];

        int offset = 0;
        sysex[offset++] = ProtocolConstants.SYSEX_START;
        sysex[offset++] = ProtocolConstants.MANUFACTURER_ID;
        sysex[offset++] = ProtocolConstants.DEVICE_ID;
        sysex[offset++] = messageId;
        sysex[offset++] = 1;  // fromHost flag (Java = host)

        System.arraycopy(payload, 0, sysex, offset, payload.length);
        offset += payload.length;

        sysex[offset] = ProtocolConstants.SYSEX_END;

        return sysex;
    }

    private static byte[] hexStringToByteArray(String hexString) {
        int len = hexString.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(hexString.charAt(i), 16) << 4)
                                 + Character.digit(hexString.charAt(i+1), 16));
        }
        return data;
    }

}  // class Protocol
