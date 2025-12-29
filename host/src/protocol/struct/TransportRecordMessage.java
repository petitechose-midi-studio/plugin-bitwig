package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportRecordMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_RECORD message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportRecordMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_RECORD;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportRecord";


    // ============================================================================
    // Fields
    // ============================================================================

    private final boolean isRecording;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportRecordMessage
     *
     * @param isRecording The isRecording value
     */
    public TransportRecordMessage(boolean isRecording) {
        this.isRecording = isRecording;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isRecording value
     *
     * @return isRecording
     */
    public boolean isRecording() {
        return isRecording;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 17;

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

        offset += Encoder.writeBool(buffer, offset, isRecording);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 17;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportRecordMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportRecordMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportRecordMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        boolean isRecording = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportRecordMessage(isRecording);
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
        sb.append("# TransportRecord\n");
        sb.append("transportRecord:\n");
        sb.append("  isRecording: ").append(isRecording() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
