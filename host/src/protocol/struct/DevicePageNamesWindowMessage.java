package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * DevicePageNamesWindowMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_NAMES_WINDOW message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DevicePageNamesWindowMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_PAGE_NAMES_WINDOW;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DevicePageNamesWindow";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int devicePageCount;
    private final int pageStartIndex;
    private final int devicePageIndex;
    private final String[] pageNames;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DevicePageNamesWindowMessage
     *
     * @param devicePageCount The devicePageCount value
     * @param pageStartIndex The pageStartIndex value
     * @param devicePageIndex The devicePageIndex value
     * @param pageNames The pageNames value
     */
    public DevicePageNamesWindowMessage(int devicePageCount, int pageStartIndex, int devicePageIndex, String[] pageNames) {
        this.devicePageCount = devicePageCount;
        this.pageStartIndex = pageStartIndex;
        this.devicePageIndex = devicePageIndex;
        this.pageNames = pageNames;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the devicePageCount value
     *
     * @return devicePageCount
     */
    public int getDevicePageCount() {
        return devicePageCount;
    }

    /**
     * Get the pageStartIndex value
     *
     * @return pageStartIndex
     */
    public int getPageStartIndex() {
        return pageStartIndex;
    }

    /**
     * Get the devicePageIndex value
     *
     * @return devicePageIndex
     */
    public int getDevicePageIndex() {
        return devicePageIndex;
    }

    /**
     * Get the pageNames value
     *
     * @return pageNames
     */
    public String[] getPageNames() {
        return pageNames;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 554;

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

        offset += Encoder.encodeUint8(buffer, offset, devicePageCount);
        offset += Encoder.encodeUint8(buffer, offset, pageStartIndex);
        offset += Encoder.encodeUint8(buffer, offset, devicePageIndex);
        offset += Encoder.encodeUint8(buffer, offset, pageNames.length);

        for (String item : pageNames) {
            offset += Encoder.encodeString(buffer, offset, item);
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 26;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DevicePageNamesWindowMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DevicePageNamesWindowMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DevicePageNamesWindowMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int devicePageCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int pageStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_pageNames = Decoder.decodeUint8(data, offset);
        offset += 1;

        String[] pageNames = new String[count_pageNames];
        for (int i = 0; i < count_pageNames; i++) {
            pageNames[i] = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + pageNames[i].length();
        }


        return new DevicePageNamesWindowMessage(devicePageCount, pageStartIndex, devicePageIndex, pageNames);
    }

}  // class Message
