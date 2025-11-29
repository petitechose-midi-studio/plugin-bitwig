package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceChangeHeaderMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE_HEADER message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceChangeHeaderMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_CHANGE_HEADER;

    // ============================================================================
    // Inner Class: PageInfo
    // ============================================================================

    public static final class PageInfo {
        private final int devicePageIndex;
        private final int devicePageCount;
        private final String devicePageName;

        public PageInfo(int devicePageIndex, int devicePageCount, String devicePageName) {
            this.devicePageIndex = devicePageIndex;
            this.devicePageCount = devicePageCount;
            this.devicePageName = devicePageName;
        }

        public int getDevicePageIndex() {
            return devicePageIndex;
        }

        public int getDevicePageCount() {
            return devicePageCount;
        }

        public String getDevicePageName() {
            return devicePageName;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final String deviceName;
    private final boolean isEnabled;
    private final int deviceType;
    private final PageInfo pageInfo;
    private final List<Integer> childrenTypes;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceChangeHeaderMessage
     *
     * @param deviceName The deviceName value
     * @param isEnabled The isEnabled value
     * @param deviceType The deviceType value
     * @param pageInfo The pageInfo value
     * @param childrenTypes The childrenTypes value
     */
    public DeviceChangeHeaderMessage(String deviceName, boolean isEnabled, int deviceType, PageInfo pageInfo, List<Integer> childrenTypes) {
        this.deviceName = deviceName;
        this.isEnabled = isEnabled;
        this.deviceType = deviceType;
        this.pageInfo = pageInfo;
        this.childrenTypes = childrenTypes;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceName value
     *
     * @return deviceName
     */
    public String getDeviceName() {
        return deviceName;
    }

    /**
     * Get the isEnabled value
     *
     * @return isEnabled
     */
    public boolean isEnabled() {
        return isEnabled;
    }

    /**
     * Get the deviceType value
     *
     * @return deviceType
     */
    public int getDeviceType() {
        return deviceType;
    }

    /**
     * Get the pageInfo value
     *
     * @return pageInfo
     */
    public PageInfo getPageInfo() {
        return pageInfo;
    }

    /**
     * Get the childrenTypes value
     *
     * @return childrenTypes
     */
    public List<Integer> getChildrenTypes() {
        return childrenTypes;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 42;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] deviceName_encoded = Encoder.encodeString(deviceName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(deviceName_encoded, 0, buffer, offset, deviceName_encoded.length);
        offset += deviceName_encoded.length;
        byte[] isEnabled_encoded = Encoder.encodeBool(isEnabled);
        System.arraycopy(isEnabled_encoded, 0, buffer, offset, isEnabled_encoded.length);
        offset += isEnabled_encoded.length;
        byte[] deviceType_encoded = Encoder.encodeUint8(deviceType);
        System.arraycopy(deviceType_encoded, 0, buffer, offset, deviceType_encoded.length);
        offset += deviceType_encoded.length;
        byte[] pageInfo_devicePageIndex_encoded = Encoder.encodeUint8(pageInfo.getDevicePageIndex());
        System.arraycopy(pageInfo_devicePageIndex_encoded, 0, buffer, offset, pageInfo_devicePageIndex_encoded.length);
        offset += pageInfo_devicePageIndex_encoded.length;
        byte[] pageInfo_devicePageCount_encoded = Encoder.encodeUint8(pageInfo.getDevicePageCount());
        System.arraycopy(pageInfo_devicePageCount_encoded, 0, buffer, offset, pageInfo_devicePageCount_encoded.length);
        offset += pageInfo_devicePageCount_encoded.length;
        byte[] pageInfo_devicePageName_encoded = Encoder.encodeString(pageInfo.getDevicePageName(), ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(pageInfo_devicePageName_encoded, 0, buffer, offset, pageInfo_devicePageName_encoded.length);
        offset += pageInfo_devicePageName_encoded.length;
        byte[] childrenTypes_count = Encoder.encodeUint8(childrenTypes.size());
        System.arraycopy(childrenTypes_count, 0, buffer, offset, 1);
        offset += 1;

        for (int item : childrenTypes) {
    byte[] item_encoded = Encoder.encodeUint8(item);
            System.arraycopy(item_encoded, 0, buffer, offset, item_encoded.length);
            offset += item_encoded.length;
        }


        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 10;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceChangeHeaderMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceChangeHeaderMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceChangeHeaderMessage decode");
        }

        int offset = 0;

        String deviceName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + deviceName.length();
        boolean isEnabled = Decoder.decodeBool(data, offset);
        offset += 1;
        int deviceType = Decoder.decodeUint8(data, offset);
        offset += 1;
        int pageInfo_devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int pageInfo_devicePageCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        String pageInfo_devicePageName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + pageInfo_devicePageName.length();
        PageInfo pageInfo = new PageInfo(pageInfo_devicePageIndex, pageInfo_devicePageCount, pageInfo_devicePageName);

        int count_childrenTypes = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<Integer> childrenTypes_list = new ArrayList<>();
        for (int i = 0; i < count_childrenTypes; i++) {
    int item_childrenTypes = Decoder.decodeUint8(data, offset);
            offset += 1;
            childrenTypes_list.add(item_childrenTypes);
        }


        return new DeviceChangeHeaderMessage(deviceName, isEnabled, deviceType, pageInfo, childrenTypes_list);
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
        sb.append("# DeviceChangeHeader\n");
        sb.append("deviceChangeHeader:\n");
        sb.append("  deviceName: \"").append(getDeviceName()).append("\"\n");
        sb.append("  isEnabled: ").append(isEnabled() ? "true" : "false").append("\n");
        sb.append("  deviceType: ").append(getDeviceType()).append("\n");
        sb.append("  pageInfo:\n");
        sb.append("    devicePageIndex: ").append(getPageInfo().getDevicePageIndex()).append("\n");
        sb.append("    devicePageCount: ").append(getPageInfo().getDevicePageCount()).append("\n");
        sb.append("    devicePageName: \"").append(getPageInfo().getDevicePageName()).append("\"\n");
        sb.append("  childrenTypes:");
        if (getChildrenTypes().isEmpty()) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (int item : getChildrenTypes()) {
                sb.append("    - ").append(item).append("\n");
            }
        }
        return sb.toString();
    }
}  // class Message
