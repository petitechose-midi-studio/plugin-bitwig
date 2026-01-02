package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import protocol.ParameterType;

/**
 * DeviceChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceChange";

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
    // Inner Class: RemoteControls
    // ============================================================================

    public static final class RemoteControls {
        private final int remoteControlIndex;
        private final float parameterValue;
        private final String parameterName;
        private final float parameterOrigin;
        private final boolean parameterExists;
        private final short discreteValueCount;
        private final String displayValue;
        private final ParameterType parameterType;
        private final String[] discreteValueNames;
        private final int currentValueIndex;
        private final boolean hasAutomation;
        private final float modulatedValue;
        private final boolean isModulated;

        public RemoteControls(int remoteControlIndex, float parameterValue, String parameterName, float parameterOrigin, boolean parameterExists, short discreteValueCount, String displayValue, ParameterType parameterType, String[] discreteValueNames, int currentValueIndex, boolean hasAutomation, float modulatedValue, boolean isModulated) {
            this.remoteControlIndex = remoteControlIndex;
            this.parameterValue = parameterValue;
            this.parameterName = parameterName;
            this.parameterOrigin = parameterOrigin;
            this.parameterExists = parameterExists;
            this.discreteValueCount = discreteValueCount;
            this.displayValue = displayValue;
            this.parameterType = parameterType;
            this.discreteValueNames = discreteValueNames;
            this.currentValueIndex = currentValueIndex;
            this.hasAutomation = hasAutomation;
            this.modulatedValue = modulatedValue;
            this.isModulated = isModulated;
        }

        public int getRemoteControlIndex() {
            return remoteControlIndex;
        }

        public float getParameterValue() {
            return parameterValue;
        }

        public String getParameterName() {
            return parameterName;
        }

        public float getParameterOrigin() {
            return parameterOrigin;
        }

        public boolean getParameterExists() {
            return parameterExists;
        }

        public short getDiscreteValueCount() {
            return discreteValueCount;
        }

        public String getDisplayValue() {
            return displayValue;
        }

        public ParameterType getParameterType() {
            return parameterType;
        }

        public String[] getDiscreteValueNames() {
            return discreteValueNames;
        }

        public int getCurrentValueIndex() {
            return currentValueIndex;
        }

        public boolean getHasAutomation() {
            return hasAutomation;
        }

        public float getModulatedValue() {
            return modulatedValue;
        }

        public boolean isModulated() {
            return isModulated;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    private final String deviceTrackName;
    private final String deviceName;
    private final boolean isEnabled;
    private final PageInfo pageInfo;
    private final RemoteControls[] remoteControls;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceChangeMessage
     *
     * @param deviceTrackName The deviceTrackName value
     * @param deviceName The deviceName value
     * @param isEnabled The isEnabled value
     * @param pageInfo The pageInfo value
     * @param remoteControls The remoteControls value
     */
    public DeviceChangeMessage(String deviceTrackName, String deviceName, boolean isEnabled, PageInfo pageInfo, RemoteControls[] remoteControls) {
        this.deviceTrackName = deviceTrackName;
        this.deviceName = deviceName;
        this.isEnabled = isEnabled;
        this.pageInfo = pageInfo;
        this.remoteControls = remoteControls;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceTrackName value
     *
     * @return deviceTrackName
     */
    public String getDeviceTrackName() {
        return deviceTrackName;
    }

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
     * Get the pageInfo value
     *
     * @return pageInfo
     */
    public PageInfo getPageInfo() {
        return pageInfo;
    }

    /**
     * Get the remoteControls value
     *
     * @return remoteControls
     */
    public RemoteControls[] getRemoteControls() {
        return remoteControls;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 9260;

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

        offset += Encoder.encodeString(buffer, offset, deviceTrackName);
        offset += Encoder.encodeString(buffer, offset, deviceName);
        offset += Encoder.encodeBool(buffer, offset, isEnabled);
        offset += Encoder.encodeUint8(buffer, offset, pageInfo.getDevicePageIndex());
        offset += Encoder.encodeUint8(buffer, offset, pageInfo.getDevicePageCount());
        offset += Encoder.encodeString(buffer, offset, pageInfo.getDevicePageName());
        offset += Encoder.encodeUint8(buffer, offset, remoteControls.length);

        for (RemoteControls item : remoteControls) {
            offset += Encoder.encodeUint8(buffer, offset, item.getRemoteControlIndex());
            offset += Encoder.encodeFloat32(buffer, offset, item.getParameterValue());
            offset += Encoder.encodeString(buffer, offset, item.getParameterName());
            offset += Encoder.encodeFloat32(buffer, offset, item.getParameterOrigin());
            offset += Encoder.encodeBool(buffer, offset, item.getParameterExists());
            offset += Encoder.encodeInt16(buffer, offset, item.getDiscreteValueCount());
            offset += Encoder.encodeString(buffer, offset, item.getDisplayValue());
            offset += Encoder.encodeUint8(buffer, offset, item.getParameterType().getValue());
            offset += Encoder.encodeUint8(buffer, offset, item.getDiscreteValueNames().length);
            for (String type : item.getDiscreteValueNames()) {
                offset += Encoder.encodeString(buffer, offset, type);
            }
            offset += Encoder.encodeUint8(buffer, offset, item.getCurrentValueIndex());
            offset += Encoder.encodeBool(buffer, offset, item.getHasAutomation());
            offset += Encoder.encodeFloat32(buffer, offset, item.getModulatedValue());
            offset += Encoder.encodeBool(buffer, offset, item.isModulated());
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 20;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceChangeMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        String deviceTrackName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + deviceTrackName.length();
        String deviceName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + deviceName.length();
        boolean isEnabled = Decoder.decodeBool(data, offset);
        offset += 1;
        int pageInfo_devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int pageInfo_devicePageCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        String pageInfo_devicePageName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + pageInfo_devicePageName.length();
        PageInfo pageInfo = new PageInfo(pageInfo_devicePageIndex, pageInfo_devicePageCount, pageInfo_devicePageName);

        int count_remoteControls = Decoder.decodeUint8(data, offset);
        offset += 1;

        RemoteControls[] remoteControls = new RemoteControls[count_remoteControls];
        for (int i = 0; i < count_remoteControls; i++) {
    int item_remoteControlIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    float item_parameterValue = Decoder.decodeFloat32(data, offset);
            offset += 4;
    String item_parameterName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_parameterName.length();
    float item_parameterOrigin = Decoder.decodeFloat32(data, offset);
            offset += 4;
    boolean item_parameterExists = Decoder.decodeBool(data, offset);
            offset += 1;
    short item_discreteValueCount = Decoder.decodeInt16(data, offset);
            offset += 2;
    String item_displayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_displayValue.length();
            ParameterType item_parameterType = ParameterType.fromValue(Decoder.decodeUint8(data, offset));
            offset += 1;
            byte count_discreteValueNames = (byte) Decoder.decodeUint8(data, offset);
            offset += 1;
            String[] item_discreteValueNames = new String[count_discreteValueNames];
            for (int j = 0; j < count_discreteValueNames && j < 32; j++) {
        String item_discreteValueNames_j = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
                offset += 1 + item_discreteValueNames_j.length();
                item_discreteValueNames[j] = item_discreteValueNames_j;
            }
    int item_currentValueIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    boolean item_hasAutomation = Decoder.decodeBool(data, offset);
            offset += 1;
    float item_modulatedValue = Decoder.decodeFloat32(data, offset);
            offset += 4;
    boolean item_isModulated = Decoder.decodeBool(data, offset);
            offset += 1;
            remoteControls[i] = new RemoteControls(item_remoteControlIndex, item_parameterValue, item_parameterName, item_parameterOrigin, item_parameterExists, item_discreteValueCount, item_displayValue, item_parameterType, item_discreteValueNames, item_currentValueIndex, item_hasAutomation, item_modulatedValue, item_isModulated);
        }


        return new DeviceChangeMessage(deviceTrackName, deviceName, isEnabled, pageInfo, remoteControls);
    }

}  // class Message
