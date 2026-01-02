package protocol.struct;

import protocol.MessageID;
import protocol.Decoder;

/**
 * RequestHostStatusMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_HOST_STATUS message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestHostStatusMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_HOST_STATUS;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "RequestHostStatus";


    // ============================================================================
    // Fields
    // ============================================================================


    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestHostStatusMessage
     *
     */
    public RequestHostStatusMessage() {
    }

    // ============================================================================
    // Getters
    // ============================================================================

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 18;

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

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (name prefix only)
     */
    private static final int MIN_PAYLOAD_SIZE = 18;

    /**
     * Decode message from bytes (no fields, with name prefix)
     * @param data Input buffer
     * @return New RequestHostStatusMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RequestHostStatusMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RequestHostStatusMessage decode");
        }
        // Skip MESSAGE_NAME prefix
        int offset = 0;
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;
        return new RequestHostStatusMessage();
    }

}  // class Message
