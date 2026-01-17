package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * EnterDeviceChildMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: ENTER_DEVICE_CHILD message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Binary).
 */
public final class EnterDeviceChildMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.ENTER_DEVICE_CHILD;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "EnterDeviceChild";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int deviceIndex;
    private final int childType;
    private final int childIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new EnterDeviceChildMessage
     *
     * @param deviceIndex The deviceIndex value
     * @param childType The childType value
     * @param childIndex The childIndex value
     */
    public EnterDeviceChildMessage(int deviceIndex, int childType, int childIndex) {
        this.deviceIndex = deviceIndex;
        this.childType = childType;
        this.childIndex = childIndex;
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

    /**
     * Get the childIndex value
     *
     * @return childIndex
     */
    public int getChildIndex() {
        return childIndex;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 20;

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
        offset += Encoder.encodeUint8(buffer, offset, childIndex);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 20;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded EnterDeviceChildMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static EnterDeviceChildMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for EnterDeviceChildMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childType = Decoder.decodeUint8(data, offset);
        offset += 1;

        int childIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new EnterDeviceChildMessage(deviceIndex, childType, childIndex);
    }

}  // class Message
