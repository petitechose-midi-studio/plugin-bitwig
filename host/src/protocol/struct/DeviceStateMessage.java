package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * DeviceStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceState";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int deviceIndex;
    private final boolean isEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceStateMessage
     *
     * @param deviceIndex The deviceIndex value
     * @param isEnabled The isEnabled value
     */
    public DeviceStateMessage(int deviceIndex, boolean isEnabled) {
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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 14;

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

        offset += Encoder.writeUint8(buffer, offset, deviceIndex);
        offset += Encoder.writeBool(buffer, offset, isEnabled);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 14;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceStateMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new DeviceStateMessage(deviceIndex, isEnabled);
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
        sb.append("# DeviceState\n");
        sb.append("deviceState:\n");
        sb.append("  deviceIndex: ").append(getDeviceIndex()).append("\n");
        sb.append("  isEnabled: ").append(isEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
