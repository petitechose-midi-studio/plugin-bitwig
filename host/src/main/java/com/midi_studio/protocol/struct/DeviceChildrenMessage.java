package com.midi_studio.protocol.struct;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.Encoder;
import com.midi_studio.protocol.Decoder;
import com.midi_studio.protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceChildrenMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHILDREN message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceChildrenMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_CHILDREN;

    // ============================================================================
    // Inner Class: Children
    // ============================================================================

    public static final class Children {
        private final int childIndex;
        private final String childName;

        public Children(int childIndex, String childName) {
            this.childIndex = childIndex;
            this.childName = childName;
        }

        public int getChildIndex() {
            return childIndex;
        }

        public String getChildName() {
            return childName;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int deviceIndex;
    private final int childType;
    private final int childrenCount;
    private final List<Children> children;

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
    public DeviceChildrenMessage(int deviceIndex, int childType, int childrenCount, List<Children> children) {
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
    public List<Children> getChildren() {
        return children;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 580;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] deviceIndex_encoded = Encoder.encodeUint8(deviceIndex);
        System.arraycopy(deviceIndex_encoded, 0, buffer, offset, deviceIndex_encoded.length);
        offset += deviceIndex_encoded.length;
        byte[] childType_encoded = Encoder.encodeUint8(childType);
        System.arraycopy(childType_encoded, 0, buffer, offset, childType_encoded.length);
        offset += childType_encoded.length;
        byte[] childrenCount_encoded = Encoder.encodeUint8(childrenCount);
        System.arraycopy(childrenCount_encoded, 0, buffer, offset, childrenCount_encoded.length);
        offset += childrenCount_encoded.length;
        byte[] children_count = Encoder.encodeUint8(children.size());
        System.arraycopy(children_count, 0, buffer, offset, 1);
        offset += 1;

        for (Children item : children) {
    byte[] item_childIndex_encoded = Encoder.encodeUint8(item.getChildIndex());
            System.arraycopy(item_childIndex_encoded, 0, buffer, offset, item_childIndex_encoded.length);
            offset += item_childIndex_encoded.length;
    byte[] item_childName_encoded = Encoder.encodeString(item.getChildName(), ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_childName_encoded, 0, buffer, offset, item_childName_encoded.length);
            offset += item_childName_encoded.length;
        }


        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 68;

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

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childType = Decoder.decodeUint8(data, offset);
        offset += 1;
        int childrenCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_children = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<Children> children_list = new ArrayList<>();
        for (int i = 0; i < count_children; i++) {
    int item_childIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_childName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_childName.length();
            children_list.add(new Children(item_childIndex, item_childName));
        }


        return new DeviceChildrenMessage(deviceIndex, childType, childrenCount, children_list);
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
        }
        return sb.toString();
    }
}  // class Message
