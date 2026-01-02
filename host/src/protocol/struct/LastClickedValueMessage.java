package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * LastClickedValueMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: LAST_CLICKED_VALUE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class LastClickedValueMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.LAST_CLICKED_VALUE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "LastClickedValue";


    // ============================================================================
    // Fields
    // ============================================================================

    private final float parameterValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new LastClickedValueMessage
     *
     * @param parameterValue The parameterValue value
     */
    public LastClickedValueMessage(float parameterValue) {
        this.parameterValue = parameterValue;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the parameterValue value
     *
     * @return parameterValue
     */
    public float getParameterValue() {
        return parameterValue;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 21;

    /**
     * Encode message directly into provided buffer (zero allocation)
     *
     * @param buffer Output buffer (must have enough space)
     * @param startOffset Starting position in buffer
     * @return Number of bytes written
     */
    public int encode(byte[] buffer, int startOffset) {
        int offset = startOffset;

        // Encode MESSAGE_NAME prefix
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.encodeFloat32(buffer, offset, parameterValue);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 21;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded LastClickedValueMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static LastClickedValueMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for LastClickedValueMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        float parameterValue = Decoder.decodeFloat32(data, offset);
        offset += 4;

        return new LastClickedValueMessage(parameterValue);
    }

}  // class Message
