package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * RequestDeviceChildrenMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_DEVICE_CHILDREN message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestDeviceChildrenMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_DEVICE_CHILDREN;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "RequestDeviceChildren";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int deviceIndex;
    private final int childType;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestDeviceChildrenMessage
     *
     * @param deviceIndex The deviceIndex value
     * @param childType The childType value
     */
    public RequestDeviceChildrenMessage(int deviceIndex, int childType) {
        this.deviceIndex = deviceIndex;
        this.childType = childType;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceIndex value
     *
     * @return deviceIndex
     */
    public int getDeviceIndex() {
        return deviceIndex;
    }

    /**
     * Get the childType value
     *
     * @return childType
     */
    public int getChildType() {
        return childType;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 24;

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

        offset += Encoder.encodeUint8(buffer, offset, deviceIndex);
        offset += Encoder.encodeUint8(buffer, offset, childType);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 24;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded RequestDeviceChildrenMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RequestDeviceChildrenMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RequestDeviceChildrenMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childType = Decoder.decodeUint8(data, offset);
        offset += 1;


        return new RequestDeviceChildrenMessage(deviceIndex, childType);
    }

}  // class Message
