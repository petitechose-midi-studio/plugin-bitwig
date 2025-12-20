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


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 2;

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

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 2;

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

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childType = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new RequestDeviceChildrenMessage(deviceIndex, childType);
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
        sb.append("# RequestDeviceChildren\n");
        sb.append("requestDeviceChildren:\n");
        sb.append("  deviceIndex: ").append(getDeviceIndex()).append("\n");
        sb.append("  childType: ").append(getChildType()).append("\n");
        return sb.toString();
    }
}  // class Message
