package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceListWindowMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_LIST_WINDOW message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceListWindowMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_LIST_WINDOW;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceListWindow";

    // ============================================================================
    // Inner Class: Devices
    // ============================================================================

    public static final class Devices {
        private final int deviceIndex;
        private final String deviceName;
        private final boolean isEnabled;
        private final int deviceType;
        private final int[] childrenTypes;

        public Devices(int deviceIndex, String deviceName, boolean isEnabled, int deviceType, int[] childrenTypes) {
            this.deviceIndex = deviceIndex;
            this.deviceName = deviceName;
            this.isEnabled = isEnabled;
            this.deviceType = deviceType;
            this.childrenTypes = childrenTypes;
        }

        public int getDeviceIndex() {
            return deviceIndex;
        }

        public String getDeviceName() {
            return deviceName;
        }

        public boolean isEnabled() {
            return isEnabled;
        }

        public int getDeviceType() {
            return deviceType;
        }

        public int[] getChildrenTypes() {
            return childrenTypes;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int deviceCount;
    private final int deviceStartIndex;
    private final int deviceIndex;
    private final boolean isNested;
    private final String parentName;
    private final List<Devices> devices;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceListWindowMessage
     *
     * @param deviceCount The deviceCount value
     * @param deviceStartIndex The deviceStartIndex value
     * @param deviceIndex The deviceIndex value
     * @param isNested The isNested value
     * @param parentName The parentName value
     * @param devices The devices value
     */
    public DeviceListWindowMessage(int deviceCount, int deviceStartIndex, int deviceIndex, boolean isNested, String parentName, List<Devices> devices) {
        this.deviceCount = deviceCount;
        this.deviceStartIndex = deviceStartIndex;
        this.deviceIndex = deviceIndex;
        this.isNested = isNested;
        this.parentName = parentName;
        this.devices = devices;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceCount value
     *
     * @return deviceCount
     */
    public int getDeviceCount() {
        return deviceCount;
    }

    /**
     * Get the deviceStartIndex value
     *
     * @return deviceStartIndex
     */
    public int getDeviceStartIndex() {
        return deviceStartIndex;
    }

    /**
     * Get the deviceIndex value
     *
     * @return deviceIndex
     */
    public int getDeviceIndex() {
        return deviceIndex;
    }

    /**
     * Get the isNested value
     *
     * @return isNested
     */
    public boolean isNested() {
        return isNested;
    }

    /**
     * Get the parentName value
     *
     * @return parentName
     */
    public String getParentName() {
        return parentName;
    }

    /**
     * Get the devices value
     *
     * @return devices
     */
    public List<Devices> getDevices() {
        return devices;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 695;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        // Encode message name (length-prefixed string for bridge logging)
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        byte[] deviceCount_encoded = Encoder.encodeUint8(deviceCount);
        System.arraycopy(deviceCount_encoded, 0, buffer, offset, deviceCount_encoded.length);
        offset += deviceCount_encoded.length;
        byte[] deviceStartIndex_encoded = Encoder.encodeUint8(deviceStartIndex);
        System.arraycopy(deviceStartIndex_encoded, 0, buffer, offset, deviceStartIndex_encoded.length);
        offset += deviceStartIndex_encoded.length;
        byte[] deviceIndex_encoded = Encoder.encodeUint8(deviceIndex);
        System.arraycopy(deviceIndex_encoded, 0, buffer, offset, deviceIndex_encoded.length);
        offset += deviceIndex_encoded.length;
        byte[] isNested_encoded = Encoder.encodeBool(isNested);
        System.arraycopy(isNested_encoded, 0, buffer, offset, isNested_encoded.length);
        offset += isNested_encoded.length;
        byte[] parentName_encoded = Encoder.encodeString(parentName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(parentName_encoded, 0, buffer, offset, parentName_encoded.length);
        offset += parentName_encoded.length;
        byte[] devices_count = Encoder.encodeUint8(devices.size());
        System.arraycopy(devices_count, 0, buffer, offset, 1);
        offset += 1;

        for (Devices item : devices) {
    byte[] item_deviceIndex_encoded = Encoder.encodeUint8(item.getDeviceIndex());
            System.arraycopy(item_deviceIndex_encoded, 0, buffer, offset, item_deviceIndex_encoded.length);
            offset += item_deviceIndex_encoded.length;
    byte[] item_deviceName_encoded = Encoder.encodeString(item.getDeviceName(), ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_deviceName_encoded, 0, buffer, offset, item_deviceName_encoded.length);
            offset += item_deviceName_encoded.length;
    byte[] item_isEnabled_encoded = Encoder.encodeBool(item.isEnabled());
            System.arraycopy(item_isEnabled_encoded, 0, buffer, offset, item_isEnabled_encoded.length);
            offset += item_isEnabled_encoded.length;
    byte[] item_deviceType_encoded = Encoder.encodeUint8(item.getDeviceType());
            System.arraycopy(item_deviceType_encoded, 0, buffer, offset, item_deviceType_encoded.length);
            offset += item_deviceType_encoded.length;
            byte[] count_childrenTypes = Encoder.encodeUint8((byte) item.getChildrenTypes().length);
            System.arraycopy(count_childrenTypes, 0, buffer, offset, 1);
            offset += 1;
            for (int type : item.getChildrenTypes()) {
        byte[] type_encoded = Encoder.encodeUint8(type);
                System.arraycopy(type_encoded, 0, buffer, offset, type_encoded.length);
                offset += type_encoded.length;
            }
        }


        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 151;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceListWindowMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceListWindowMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceListWindowMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int deviceCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int deviceStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isNested = Decoder.decodeBool(data, offset);
        offset += 1;
        String parentName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parentName.length();
        int count_devices = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<Devices> devices_list = new ArrayList<>();
        for (int i = 0; i < count_devices; i++) {
    int item_deviceIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_deviceName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_deviceName.length();
    boolean item_isEnabled = Decoder.decodeBool(data, offset);
            offset += 1;
    int item_deviceType = Decoder.decodeUint8(data, offset);
            offset += 1;
            byte count_childrenTypes = (byte) Decoder.decodeUint8(data, offset);
            offset += 1;
            int[] item_childrenTypes = new int[count_childrenTypes];
            for (int j = 0; j < count_childrenTypes && j < 4; j++) {
        int item_childrenTypes_j = Decoder.decodeUint8(data, offset);
                offset += 1;
                item_childrenTypes[j] = item_childrenTypes_j;
            }
            devices_list.add(new Devices(item_deviceIndex, item_deviceName, item_isEnabled, item_deviceType, item_childrenTypes));
        }


        return new DeviceListWindowMessage(deviceCount, deviceStartIndex, deviceIndex, isNested, parentName, devices_list);
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
        sb.append("# DeviceListWindow\n");
        sb.append("deviceListWindow:\n");
        sb.append("  deviceCount: ").append(getDeviceCount()).append("\n");
        sb.append("  deviceStartIndex: ").append(getDeviceStartIndex()).append("\n");
        sb.append("  deviceIndex: ").append(getDeviceIndex()).append("\n");
        sb.append("  isNested: ").append(isNested() ? "true" : "false").append("\n");
        sb.append("  parentName: \"").append(getParentName()).append("\"\n");
        sb.append("  devices:\n");
        for (Devices item : getDevices()) {
            sb.append("    - deviceIndex: ").append(item.getDeviceIndex()).append("\n");
            sb.append("      deviceName: \"").append(item.getDeviceName()).append("\"\n");
            sb.append("      isEnabled: ").append(item.isEnabled() ? "true" : "false").append("\n");
            sb.append("      deviceType: ").append(item.getDeviceType()).append("\n");
        }
        return sb.toString();
    }
}  // class Message
