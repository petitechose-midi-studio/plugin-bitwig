package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportAutomationWriteModeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_AUTOMATION_WRITE_MODE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportAutomationWriteModeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_AUTOMATION_WRITE_MODE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportAutomationWriteMode";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int automationWriteMode;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportAutomationWriteModeMessage
     *
     * @param automationWriteMode The automationWriteMode value
     */
    public TransportAutomationWriteModeMessage(int automationWriteMode) {
        this.automationWriteMode = automationWriteMode;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the automationWriteMode value
     *
     * @return automationWriteMode
     */
    public int getAutomationWriteMode() {
        return automationWriteMode;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 30;

    /**
     * Encode message directly into provided buffer (zero allocation)
     *
     * @param buffer Output buffer (must have enough space)
     * @param startOffset Starting position in buffer
     * @return Number of bytes written
     */
    public int encode(byte[] buffer, int startOffset) {
        int offset = startOffset;

        // Encode message name (length-prefixed string for bridge logging)
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.writeUint8(buffer, offset, automationWriteMode);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 30;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportAutomationWriteModeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportAutomationWriteModeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportAutomationWriteModeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int automationWriteMode = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new TransportAutomationWriteModeMessage(automationWriteMode);
    }

    // ============================================================================
    // Logging
    // ============================================================================
    
    /**
     * Convert message to YAML format for logging.
     * 
     * @return YAML string representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(256);
        sb.append("# TransportAutomationWriteMode\n");
        sb.append("transportAutomationWriteMode:\n");
        sb.append("  automationWriteMode: ").append(getAutomationWriteMode()).append("\n");
        return sb.toString();
    }
}  // class Message
