package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * RequestDeviceListWindowMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_DEVICE_LIST_WINDOW message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestDeviceListWindowMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_DEVICE_LIST_WINDOW;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "RequestDeviceListWindow";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int deviceStartIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestDeviceListWindowMessage
     *
     * @param deviceStartIndex The deviceStartIndex value
     */
    public RequestDeviceListWindowMessage(int deviceStartIndex) {
        this.deviceStartIndex = deviceStartIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceStartIndex value
     *
     * @return deviceStartIndex
     */
    public int getDeviceStartIndex() {
        return deviceStartIndex;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 25;

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

        offset += Encoder.encodeUint8(buffer, offset, deviceStartIndex);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 25;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded RequestDeviceListWindowMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RequestDeviceListWindowMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RequestDeviceListWindowMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int deviceStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new RequestDeviceListWindowMessage(deviceStartIndex);
    }

}  // class Message
