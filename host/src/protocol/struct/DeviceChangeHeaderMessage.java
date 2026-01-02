package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import protocol.DeviceType;

/**
 * DeviceChangeHeaderMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE_HEADER message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceChangeHeaderMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_CHANGE_HEADER;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceChangeHeader";

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

    private final String deviceName;
    private final boolean isEnabled;
    private final DeviceType deviceType;
    private final PageInfo pageInfo;
    private final int[] childrenTypes;

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
    public DeviceChangeHeaderMessage(String deviceName, boolean isEnabled, DeviceType deviceType, PageInfo pageInfo, int[] childrenTypes) {
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
    public DeviceType getDeviceType() {
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
    public int[] getChildrenTypes() {
        return childrenTypes;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 94;

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

        offset += Encoder.encodeString(buffer, offset, deviceName);
        offset += Encoder.encodeBool(buffer, offset, isEnabled);
        offset += Encoder.encodeUint8(buffer, offset, deviceType.getValue());
        offset += Encoder.encodeUint8(buffer, offset, pageInfo.getDevicePageIndex());
        offset += Encoder.encodeUint8(buffer, offset, pageInfo.getDevicePageCount());
        offset += Encoder.encodeString(buffer, offset, pageInfo.getDevicePageName());
        offset += Encoder.encodeUint8(buffer, offset, childrenTypes.length);

        for (int item : childrenTypes) {
            offset += Encoder.encodeUint8(buffer, offset, item);
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
     * @return Decoded DeviceChangeHeaderMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceChangeHeaderMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceChangeHeaderMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        String deviceName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + deviceName.length();
        boolean isEnabled = Decoder.decodeBool(data, offset);
        offset += 1;
        DeviceType deviceType = DeviceType.fromValue(Decoder.decodeUint8(data, offset));
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

        int[] childrenTypes = new int[count_childrenTypes];
        for (int i = 0; i < count_childrenTypes; i++) {
            childrenTypes[i] = Decoder.decodeUint8(data, offset);
            offset += 1;
        }


        return new DeviceChangeHeaderMessage(deviceName, isEnabled, deviceType, pageInfo, childrenTypes);
    }

}  // class Message
