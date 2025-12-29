package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * DeviceChildrenMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHILDREN message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceChildrenMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_CHILDREN;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceChildren";

    // ============================================================================
    // Inner Class: Children
    // ============================================================================

    public static final class Children {
        private final int childIndex;
        private final String childName;
        private final int itemType;

        public Children(int childIndex, String childName, int itemType) {
            this.childIndex = childIndex;
            this.childName = childName;
            this.itemType = itemType;
        }

        public int getChildIndex() {
            return childIndex;
        }

        public String getChildName() {
            return childName;
        }

        public int getItemType() {
            return itemType;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    private final int deviceIndex;
    private final int childType;
    private final int childrenCount;
    private final Children[] children;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceChildrenMessage
     *
     * @param deviceIndex The deviceIndex value
     * @param childType The childType value
     * @param childrenCount The childrenCount value
     * @param children The children value
     */
    public DeviceChildrenMessage(int deviceIndex, int childType, int childrenCount, Children[] children) {
        this.deviceIndex = deviceIndex;
        this.childType = childType;
        this.childrenCount = childrenCount;
        this.children = children;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceIndex value
     *
     * @return deviceIndex
     */
    public int getDeviceIndex() {
        return deviceIndex;
    }

    /**
     * Get the childType value
     *
     * @return childType
     */
    public int getChildType() {
        return childType;
    }

    /**
     * Get the childrenCount value
     *
     * @return childrenCount
     */
    public int getChildrenCount() {
        return childrenCount;
    }

    /**
     * Get the children value
     *
     * @return children
     */
    public Children[] getChildren() {
        return children;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 579;

    /**
     * Encode message directly into provided buffer (zero allocation)
     *
     * @param buffer Output buffer (must have enough space)
     * @param startOffset Starting position in buffer
     * @return Number of bytes written
     */
    public int encode(byte[] buffer, int startOffset) {
        int offset = startOffset;

        // Encode message name (length-prefixed string for bridge logging)
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.writeUint8(buffer, offset, deviceIndex);
        offset += Encoder.writeUint8(buffer, offset, childType);
        offset += Encoder.writeUint8(buffer, offset, childrenCount);
        offset += Encoder.writeUint8(buffer, offset, children.length);

        for (Children item : children) {
            offset += Encoder.writeUint8(buffer, offset, item.getChildIndex());
            offset += Encoder.writeString(buffer, offset, item.getChildName(), ProtocolConstants.STRING_MAX_LENGTH);
            offset += Encoder.writeUint8(buffer, offset, item.getItemType());
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 67;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceChildrenMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceChildrenMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceChildrenMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childType = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childrenCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_children = Decoder.decodeUint8(data, offset);
        offset += 1;

        Children[] children = new Children[count_children];
        for (int i = 0; i < count_children; i++) {
    int item_childIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_childName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_childName.length();
    int item_itemType = Decoder.decodeUint8(data, offset);
            offset += 1;
            children[i] = new Children(item_childIndex, item_childName, item_itemType);
        }


        return new DeviceChildrenMessage(deviceIndex, childType, childrenCount, children);
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
        sb.append("# DeviceChildren\n");
        sb.append("deviceChildren:\n");
        sb.append("  deviceIndex: ").append(getDeviceIndex()).append("\n");
        sb.append("  childType: ").append(getChildType()).append("\n");
        sb.append("  childrenCount: ").append(getChildrenCount()).append("\n");
        sb.append("  children:\n");
        for (Children item : getChildren()) {
            sb.append("    - childIndex: ").append(item.getChildIndex()).append("\n");
            sb.append("      childName: \"").append(item.getChildName()).append("\"\n");
            sb.append("      itemType: ").append(item.getItemType()).append("\n");
        }
        return sb.toString();
    }
}  // class Message
