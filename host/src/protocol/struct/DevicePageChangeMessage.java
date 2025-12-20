package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DevicePageChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DevicePageChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_PAGE_CHANGE;

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
        private final int parameterType;
        private final String[] discreteValueNames;
        private final int currentValueIndex;
        private final boolean hasAutomation;
        private final float modulatedValue;
        private final boolean isModulated;

        public RemoteControls(int remoteControlIndex, float parameterValue, String parameterName, float parameterOrigin, boolean parameterExists, short discreteValueCount, String displayValue, int parameterType, String[] discreteValueNames, int currentValueIndex, boolean hasAutomation, float modulatedValue, boolean isModulated) {
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

        public int getParameterType() {
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

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final PageInfo pageInfo;
    private final List<RemoteControls> remoteControls;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DevicePageChangeMessage
     *
     * @param pageInfo The pageInfo value
     * @param remoteControls The remoteControls value
     */
    public DevicePageChangeMessage(PageInfo pageInfo, List<RemoteControls> remoteControls) {
        this.pageInfo = pageInfo;
        this.remoteControls = remoteControls;
    }

    // ============================================================================
    // Getters
    // ============================================================================

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
    public List<RemoteControls> getRemoteControls() {
        return remoteControls;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 9180;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] pageInfo_devicePageIndex_encoded = Encoder.encodeUint8(pageInfo.getDevicePageIndex());
        System.arraycopy(pageInfo_devicePageIndex_encoded, 0, buffer, offset, pageInfo_devicePageIndex_encoded.length);
        offset += pageInfo_devicePageIndex_encoded.length;
        byte[] pageInfo_devicePageCount_encoded = Encoder.encodeUint8(pageInfo.getDevicePageCount());
        System.arraycopy(pageInfo_devicePageCount_encoded, 0, buffer, offset, pageInfo_devicePageCount_encoded.length);
        offset += pageInfo_devicePageCount_encoded.length;
        byte[] pageInfo_devicePageName_encoded = Encoder.encodeString(pageInfo.getDevicePageName(), ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(pageInfo_devicePageName_encoded, 0, buffer, offset, pageInfo_devicePageName_encoded.length);
        offset += pageInfo_devicePageName_encoded.length;
        byte[] remoteControls_count = Encoder.encodeUint8(remoteControls.size());
        System.arraycopy(remoteControls_count, 0, buffer, offset, 1);
        offset += 1;

        for (RemoteControls item : remoteControls) {
    byte[] item_remoteControlIndex_encoded = Encoder.encodeUint8(item.getRemoteControlIndex());
            System.arraycopy(item_remoteControlIndex_encoded, 0, buffer, offset, item_remoteControlIndex_encoded.length);
            offset += item_remoteControlIndex_encoded.length;
    byte[] item_parameterValue_encoded = Encoder.encodeFloat32(item.getParameterValue());
            System.arraycopy(item_parameterValue_encoded, 0, buffer, offset, item_parameterValue_encoded.length);
            offset += item_parameterValue_encoded.length;
    byte[] item_parameterName_encoded = Encoder.encodeString(item.getParameterName(), ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_parameterName_encoded, 0, buffer, offset, item_parameterName_encoded.length);
            offset += item_parameterName_encoded.length;
    byte[] item_parameterOrigin_encoded = Encoder.encodeFloat32(item.getParameterOrigin());
            System.arraycopy(item_parameterOrigin_encoded, 0, buffer, offset, item_parameterOrigin_encoded.length);
            offset += item_parameterOrigin_encoded.length;
    byte[] item_parameterExists_encoded = Encoder.encodeBool(item.getParameterExists());
            System.arraycopy(item_parameterExists_encoded, 0, buffer, offset, item_parameterExists_encoded.length);
            offset += item_parameterExists_encoded.length;
    byte[] item_discreteValueCount_encoded = Encoder.encodeInt16(item.getDiscreteValueCount());
            System.arraycopy(item_discreteValueCount_encoded, 0, buffer, offset, item_discreteValueCount_encoded.length);
            offset += item_discreteValueCount_encoded.length;
    byte[] item_displayValue_encoded = Encoder.encodeString(item.getDisplayValue(), ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_displayValue_encoded, 0, buffer, offset, item_displayValue_encoded.length);
            offset += item_displayValue_encoded.length;
    byte[] item_parameterType_encoded = Encoder.encodeUint8(item.getParameterType());
            System.arraycopy(item_parameterType_encoded, 0, buffer, offset, item_parameterType_encoded.length);
            offset += item_parameterType_encoded.length;
            byte[] count_discreteValueNames = Encoder.encodeUint8((byte) item.getDiscreteValueNames().length);
            System.arraycopy(count_discreteValueNames, 0, buffer, offset, 1);
            offset += 1;
            for (String type : item.getDiscreteValueNames()) {
        byte[] type_encoded = Encoder.encodeString(type, ProtocolConstants.STRING_MAX_LENGTH);
                System.arraycopy(type_encoded, 0, buffer, offset, type_encoded.length);
                offset += type_encoded.length;
            }
    byte[] item_currentValueIndex_encoded = Encoder.encodeUint8(item.getCurrentValueIndex());
            System.arraycopy(item_currentValueIndex_encoded, 0, buffer, offset, item_currentValueIndex_encoded.length);
            offset += item_currentValueIndex_encoded.length;
    byte[] item_hasAutomation_encoded = Encoder.encodeBool(item.getHasAutomation());
            System.arraycopy(item_hasAutomation_encoded, 0, buffer, offset, item_hasAutomation_encoded.length);
            offset += item_hasAutomation_encoded.length;
    byte[] item_modulatedValue_encoded = Encoder.encodeFloat32(item.getModulatedValue());
            System.arraycopy(item_modulatedValue_encoded, 0, buffer, offset, item_modulatedValue_encoded.length);
            offset += item_modulatedValue_encoded.length;
    byte[] item_isModulated_encoded = Encoder.encodeBool(item.isModulated());
            System.arraycopy(item_isModulated_encoded, 0, buffer, offset, item_isModulated_encoded.length);
            offset += item_isModulated_encoded.length;
        }


        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 188;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DevicePageChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DevicePageChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DevicePageChangeMessage decode");
        }

        int offset = 0;

        int pageInfo_devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int pageInfo_devicePageCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        String pageInfo_devicePageName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + pageInfo_devicePageName.length();
        PageInfo pageInfo = new PageInfo(pageInfo_devicePageIndex, pageInfo_devicePageCount, pageInfo_devicePageName);

        int count_remoteControls = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<RemoteControls> remoteControls_list = new ArrayList<>();
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
    int item_parameterType = Decoder.decodeUint8(data, offset);
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
            remoteControls_list.add(new RemoteControls(item_remoteControlIndex, item_parameterValue, item_parameterName, item_parameterOrigin, item_parameterExists, item_discreteValueCount, item_displayValue, item_parameterType, item_discreteValueNames, item_currentValueIndex, item_hasAutomation, item_modulatedValue, item_isModulated));
        }


        return new DevicePageChangeMessage(pageInfo, remoteControls_list);
    }

    // ============================================================================
    // Logging
    // ============================================================================
    
    /**
     * Format float with 4 decimal places, handling edge cases.
     * 
     * @param value Float value to format
     * @return Formatted string (e.g., "3.1416", "NaN", "Inf")
     */
    private static String formatFloat(float value) {
        if (Float.isNaN(value)) return "NaN";
        if (Float.isInfinite(value)) return value > 0 ? "Inf" : "-Inf";
        return String.format("%.4f", value);
    }
    
    /**
     * Convert message to YAML format for logging.
     * 
     * @return YAML string representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(256);
        sb.append("# DevicePageChange\n");
        sb.append("devicePageChange:\n");
        sb.append("  pageInfo:\n");
        sb.append("    devicePageIndex: ").append(getPageInfo().getDevicePageIndex()).append("\n");
        sb.append("    devicePageCount: ").append(getPageInfo().getDevicePageCount()).append("\n");
        sb.append("    devicePageName: \"").append(getPageInfo().getDevicePageName()).append("\"\n");
        sb.append("  remoteControls:\n");
        for (RemoteControls item : getRemoteControls()) {
            sb.append("    - remoteControlIndex: ").append(item.getRemoteControlIndex()).append("\n");
            sb.append("      parameterValue: ").append(formatFloat(item.getParameterValue())).append("\n");
            sb.append("      parameterName: \"").append(item.getParameterName()).append("\"\n");
            sb.append("      parameterOrigin: ").append(formatFloat(item.getParameterOrigin())).append("\n");
            sb.append("      parameterExists: ").append(item.getParameterExists() ? "true" : "false").append("\n");
            sb.append("      discreteValueCount: ").append(item.getDiscreteValueCount()).append("\n");
            sb.append("      displayValue: \"").append(item.getDisplayValue()).append("\"\n");
            sb.append("      parameterType: ").append(item.getParameterType()).append("\n");
            sb.append("      currentValueIndex: ").append(item.getCurrentValueIndex()).append("\n");
            sb.append("      hasAutomation: ").append(item.getHasAutomation() ? "true" : "false").append("\n");
            sb.append("      modulatedValue: ").append(formatFloat(item.getModulatedValue())).append("\n");
            sb.append("      isModulated: ").append(item.isModulated() ? "true" : "false").append("\n");
        }
        return sb.toString();
    }
}  // class Message
