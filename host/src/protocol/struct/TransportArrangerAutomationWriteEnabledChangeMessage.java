package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportArrangerAutomationWriteEnabledChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportArrangerAutomationWriteEnabledChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportArrangerAutomationWriteEnabledChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isArrangerAutomationWriteEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportArrangerAutomationWriteEnabledChangeMessage
     *
     * @param isArrangerAutomationWriteEnabled The isArrangerAutomationWriteEnabled value
     */
    public TransportArrangerAutomationWriteEnabledChangeMessage(boolean isArrangerAutomationWriteEnabled) {
        this.isArrangerAutomationWriteEnabled = isArrangerAutomationWriteEnabled;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isArrangerAutomationWriteEnabled value
     *
     * @return isArrangerAutomationWriteEnabled
     */
    public boolean isArrangerAutomationWriteEnabled() {
        return isArrangerAutomationWriteEnabled;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 47;

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

        offset += Encoder.writeBool(buffer, offset, isArrangerAutomationWriteEnabled);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 47;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportArrangerAutomationWriteEnabledChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportArrangerAutomationWriteEnabledChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportArrangerAutomationWriteEnabledChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        boolean isArrangerAutomationWriteEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportArrangerAutomationWriteEnabledChangeMessage(isArrangerAutomationWriteEnabled);
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
        sb.append("# TransportArrangerAutomationWriteEnabledChange\n");
        sb.append("transportArrangerAutomationWriteEnabledChange:\n");
        sb.append("  isArrangerAutomationWriteEnabled: ").append(isArrangerAutomationWriteEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
