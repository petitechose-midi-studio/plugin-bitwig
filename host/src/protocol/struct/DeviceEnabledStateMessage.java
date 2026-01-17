package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * DeviceEnabledStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_ENABLED_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Binary).
 */
public final class DeviceEnabledStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_ENABLED_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceEnabledState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int deviceIndex;
    private final boolean isEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceEnabledStateMessage
     *
     * @param deviceIndex The deviceIndex value
     * @param isEnabled The isEnabled value
     */
    public DeviceEnabledStateMessage(int deviceIndex, boolean isEnabled) {
        this.deviceIndex = deviceIndex;
        this.isEnabled = isEnabled;
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
     * Get the isEnabled value
     *
     * @return isEnabled
     */
    public boolean isEnabled() {
        return isEnabled;
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

        offset += Encoder.encodeUint8(buffer, offset, deviceIndex);
        offset += Encoder.encodeBool(buffer, offset, isEnabled);

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
     * @return Decoded DeviceEnabledStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceEnabledStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceEnabledStateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new DeviceEnabledStateMessage(deviceIndex, isEnabled);
    }

}  // class Message
