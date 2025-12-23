package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * LastClickedTouchMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: LAST_CLICKED_TOUCH message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class LastClickedTouchMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.LAST_CLICKED_TOUCH;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "LastClickedTouch";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isTouched;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new LastClickedTouchMessage
     *
     * @param isTouched The isTouched value
     */
    public LastClickedTouchMessage(boolean isTouched) {
        this.isTouched = isTouched;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isTouched value
     *
     * @return isTouched
     */
    public boolean isTouched() {
        return isTouched;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 18;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        // Encode message name (length-prefixed string for bridge logging)
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.writeBool(buffer, offset, isTouched);

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 18;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded LastClickedTouchMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static LastClickedTouchMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for LastClickedTouchMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        boolean isTouched = Decoder.decodeBool(data, offset);
        offset += 1;

        return new LastClickedTouchMessage(isTouched);
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
        sb.append("# LastClickedTouch\n");
        sb.append("lastClickedTouch:\n");
        sb.append("  isTouched: ").append(isTouched() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
