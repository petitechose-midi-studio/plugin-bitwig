package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * RequestDevicePageNamesWindowMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_DEVICE_PAGE_NAMES_WINDOW message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestDevicePageNamesWindowMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_DEVICE_PAGE_NAMES_WINDOW;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "RequestDevicePageNamesWindow";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int pageStartIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestDevicePageNamesWindowMessage
     *
     * @param pageStartIndex The pageStartIndex value
     */
    public RequestDevicePageNamesWindowMessage(int pageStartIndex) {
        this.pageStartIndex = pageStartIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the pageStartIndex value
     *
     * @return pageStartIndex
     */
    public int getPageStartIndex() {
        return pageStartIndex;
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

        offset += Encoder.writeUint8(buffer, offset, pageStartIndex);

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
     * @return Decoded RequestDevicePageNamesWindowMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RequestDevicePageNamesWindowMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RequestDevicePageNamesWindowMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int pageStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new RequestDevicePageNamesWindowMessage(pageStartIndex);
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
        sb.append("# RequestDevicePageNamesWindow\n");
        sb.append("requestDevicePageNamesWindow:\n");
        sb.append("  pageStartIndex: ").append(getPageStartIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
