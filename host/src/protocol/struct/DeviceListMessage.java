package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceListMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_LIST message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_LIST;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceList";

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
    private final int deviceIndex;
    private final boolean isNested;
    private final String parentName;
    private final List<Devices> devices;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceListMessage
     *
     * @param deviceCount The deviceCount value
     * @param deviceIndex The deviceIndex value
     * @param isNested The isNested value
     * @param parentName The parentName value
     * @param devices The devices value
     */
    public DeviceListMessage(int deviceCount, int deviceIndex, boolean isNested, String parentName, List<Devices> devices) {
        this.deviceCount = deviceCount;
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
    public static final int MAX_PAYLOAD_SIZE = 1360;

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

        offset += Encoder.writeUint8(buffer, offset, deviceCount);
        offset += Encoder.writeUint8(buffer, offset, deviceIndex);
        offset += Encoder.writeBool(buffer, offset, isNested);
        offset += Encoder.writeString(buffer, offset, parentName, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeUint8(buffer, offset, devices.size());

        for (Devices item : devices) {
            offset += Encoder.writeUint8(buffer, offset, item.getDeviceIndex());
            offset += Encoder.writeString(buffer, offset, item.getDeviceName(), ProtocolConstants.STRING_MAX_LENGTH);
            offset += Encoder.writeBool(buffer, offset, item.isEnabled());
            offset += Encoder.writeUint8(buffer, offset, item.getDeviceType());
            offset += Encoder.writeUint8(buffer, offset, item.getChildrenTypes().length);
            for (int type : item.getChildrenTypes()) {
                offset += Encoder.writeUint8(buffer, offset, type);
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
    private static final int MIN_PAYLOAD_SIZE = 176;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceListMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceListMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceListMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int deviceCount = Decoder.decodeUint8(data, offset);
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


        return new DeviceListMessage(deviceCount, deviceIndex, isNested, parentName, devices_list);
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
        sb.append("# DeviceList\n");
        sb.append("deviceList:\n");
        sb.append("  deviceCount: ").append(getDeviceCount()).append("\n");
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
