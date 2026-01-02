package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * DevicePageSelectMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_SELECT message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DevicePageSelectMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_PAGE_SELECT;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DevicePageSelect";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int devicePageIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DevicePageSelectMessage
     *
     * @param devicePageIndex The devicePageIndex value
     */
    public DevicePageSelectMessage(int devicePageIndex) {
        this.devicePageIndex = devicePageIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the devicePageIndex value
     *
     * @return devicePageIndex
     */
    public int getDevicePageIndex() {
        return devicePageIndex;
    }

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

        offset += Encoder.encodeUint8(buffer, offset, devicePageIndex);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 18;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DevicePageSelectMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DevicePageSelectMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DevicePageSelectMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new DevicePageSelectMessage(devicePageIndex);
    }

}  // class Message
