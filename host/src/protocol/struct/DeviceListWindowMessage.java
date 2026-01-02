package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import protocol.DeviceType;

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
        private final DeviceType deviceType;
        private final int[] childrenTypes;

        public Devices(int deviceIndex, String deviceName, boolean isEnabled, DeviceType deviceType, int[] childrenTypes) {
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

        public DeviceType getDeviceType() {
            return deviceType;
        }

        public int[] getChildrenTypes() {
            return childrenTypes;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    private final int deviceCount;
    private final int deviceStartIndex;
    private final int deviceIndex;
    private final boolean isNested;
    private final String parentName;
    private final Devices[] devices;

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
    public DeviceListWindowMessage(int deviceCount, int deviceStartIndex, int deviceIndex, boolean isNested, String parentName, Devices[] devices) {
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
    public Devices[] getDevices() {
        return devices;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 711;

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

        offset += Encoder.encodeUint8(buffer, offset, deviceCount);
        offset += Encoder.encodeUint8(buffer, offset, deviceStartIndex);
        offset += Encoder.encodeUint8(buffer, offset, deviceIndex);
        offset += Encoder.encodeBool(buffer, offset, isNested);
        offset += Encoder.encodeString(buffer, offset, parentName);
        offset += Encoder.encodeUint8(buffer, offset, devices.length);

        for (Devices item : devices) {
            offset += Encoder.encodeUint8(buffer, offset, item.getDeviceIndex());
            offset += Encoder.encodeString(buffer, offset, item.getDeviceName());
            offset += Encoder.encodeBool(buffer, offset, item.isEnabled());
            offset += Encoder.encodeUint8(buffer, offset, item.getDeviceType().getValue());
            offset += Encoder.encodeUint8(buffer, offset, item.getChildrenTypes().length);
            for (int type : item.getChildrenTypes()) {
                offset += Encoder.encodeUint8(buffer, offset, type);
            }
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 23;

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

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

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

        Devices[] devices = new Devices[count_devices];
        for (int i = 0; i < count_devices; i++) {
    int item_deviceIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_deviceName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_deviceName.length();
    boolean item_isEnabled = Decoder.decodeBool(data, offset);
            offset += 1;
            DeviceType item_deviceType = DeviceType.fromValue(Decoder.decodeUint8(data, offset));
            offset += 1;
            byte count_childrenTypes = (byte) Decoder.decodeUint8(data, offset);
            offset += 1;
            int[] item_childrenTypes = new int[count_childrenTypes];
            for (int j = 0; j < count_childrenTypes && j < 4; j++) {
        int item_childrenTypes_j = Decoder.decodeUint8(data, offset);
                offset += 1;
                item_childrenTypes[j] = item_childrenTypes_j;
            }
            devices[i] = new Devices(item_deviceIndex, item_deviceName, item_isEnabled, item_deviceType, item_childrenTypes);
        }


        return new DeviceListWindowMessage(deviceCount, deviceStartIndex, deviceIndex, isNested, parentName, devices);
    }

}  // class Message
