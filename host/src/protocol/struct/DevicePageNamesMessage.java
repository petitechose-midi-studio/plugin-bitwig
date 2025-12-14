package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DevicePageNamesMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_NAMES message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DevicePageNamesMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_PAGE_NAMES;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int devicePageCount;
    private final int devicePageIndex;
    private final List<String> pageNames;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DevicePageNamesMessage
     *
     * @param devicePageCount The devicePageCount value
     * @param devicePageIndex The devicePageIndex value
     * @param pageNames The pageNames value
     */
    public DevicePageNamesMessage(int devicePageCount, int devicePageIndex, List<String> pageNames) {
        this.devicePageCount = devicePageCount;
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
    public List<String> getPageNames() {
        return pageNames;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 1058;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] devicePageCount_encoded = Encoder.encodeUint8(devicePageCount);
        System.arraycopy(devicePageCount_encoded, 0, buffer, offset, devicePageCount_encoded.length);
        offset += devicePageCount_encoded.length;
        byte[] devicePageIndex_encoded = Encoder.encodeUint8(devicePageIndex);
        System.arraycopy(devicePageIndex_encoded, 0, buffer, offset, devicePageIndex_encoded.length);
        offset += devicePageIndex_encoded.length;
        byte[] pageNames_count = Encoder.encodeUint8(pageNames.size());
        System.arraycopy(pageNames_count, 0, buffer, offset, 1);
        offset += 1;

        for (String item : pageNames) {
    byte[] item_encoded = Encoder.encodeString(item, ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_encoded, 0, buffer, offset, item_encoded.length);
            offset += item_encoded.length;
        }


        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 34;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DevicePageNamesMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DevicePageNamesMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DevicePageNamesMessage decode");
        }

        int offset = 0;

        int devicePageCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_pageNames = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<String> pageNames_list = new ArrayList<>();
        for (int i = 0; i < count_pageNames; i++) {
    String item_pageNames = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_pageNames.length();
            pageNames_list.add(item_pageNames);
        }


        return new DevicePageNamesMessage(devicePageCount, devicePageIndex, pageNames_list);
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
        sb.append("# DevicePageNames\n");
        sb.append("devicePageNames:\n");
        sb.append("  devicePageCount: ").append(getDevicePageCount()).append("\n");
        sb.append("  devicePageIndex: ").append(getDevicePageIndex()).append("\n");
        sb.append("  pageNames:");
        if (getPageNames().isEmpty()) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (String item : getPageNames()) {
                sb.append("    - \"").append(item).append("\"\n");
            }
        }
        return sb.toString();
    }
}  // class Message
