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


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 1;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] deviceStartIndex_encoded = Encoder.encodeUint8(deviceStartIndex);
        System.arraycopy(deviceStartIndex_encoded, 0, buffer, offset, deviceStartIndex_encoded.length);
        offset += deviceStartIndex_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 1;

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

        int deviceStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new RequestDeviceListWindowMessage(deviceStartIndex);
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
        sb.append("# RequestDeviceListWindow\n");
        sb.append("requestDeviceListWindow:\n");
        sb.append("  deviceStartIndex: ").append(getDeviceStartIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
