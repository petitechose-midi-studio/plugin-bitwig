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
 * All encoding is 7-bit MIDI-safe.
 */
public final class EnterDeviceChildMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.ENTER_DEVICE_CHILD;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

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
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 3;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] deviceIndex_encoded = Encoder.encodeUint8(deviceIndex);
        System.arraycopy(deviceIndex_encoded, 0, buffer, offset, deviceIndex_encoded.length);
        offset += deviceIndex_encoded.length;
        byte[] childType_encoded = Encoder.encodeUint8(childType);
        System.arraycopy(childType_encoded, 0, buffer, offset, childType_encoded.length);
        offset += childType_encoded.length;
        byte[] childIndex_encoded = Encoder.encodeUint8(childIndex);
        System.arraycopy(childIndex_encoded, 0, buffer, offset, childIndex_encoded.length);
        offset += childIndex_encoded.length;

        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 3;

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

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childType = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new EnterDeviceChildMessage(deviceIndex, childType, childIndex);
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
        sb.append("# EnterDeviceChild\n");
        sb.append("enterDeviceChild:\n");
        sb.append("  deviceIndex: ").append(getDeviceIndex()).append("\n");
        sb.append("  childType: ").append(getChildType()).append("\n");
        sb.append("  childIndex: ").append(getChildIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
