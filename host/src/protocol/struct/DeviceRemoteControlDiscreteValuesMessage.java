package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * DeviceRemoteControlDiscreteValuesMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_DISCRETE_VALUES message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceRemoteControlDiscreteValuesMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROL_DISCRETE_VALUES;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceRemoteControlDiscreteValues";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int remoteControlIndex;
    private final String[] discreteValueNames;
    private final int currentValueIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlDiscreteValuesMessage
     *
     * @param remoteControlIndex The remoteControlIndex value
     * @param discreteValueNames The discreteValueNames value
     * @param currentValueIndex The currentValueIndex value
     */
    public DeviceRemoteControlDiscreteValuesMessage(int remoteControlIndex, String[] discreteValueNames, int currentValueIndex) {
        this.remoteControlIndex = remoteControlIndex;
        this.discreteValueNames = discreteValueNames;
        this.currentValueIndex = currentValueIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the remoteControlIndex value
     *
     * @return remoteControlIndex
     */
    public int getRemoteControlIndex() {
        return remoteControlIndex;
    }

    /**
     * Get the discreteValueNames value
     *
     * @return discreteValueNames
     */
    public String[] getDiscreteValueNames() {
        return discreteValueNames;
    }

    /**
     * Get the currentValueIndex value
     *
     * @return currentValueIndex
     */
    public int getCurrentValueIndex() {
        return currentValueIndex;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 1093;

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

        offset += Encoder.writeUint8(buffer, offset, remoteControlIndex);
        offset += Encoder.writeUint8(buffer, offset, discreteValueNames.length);

        for (String item : discreteValueNames) {
            offset += Encoder.writeString(buffer, offset, item, ProtocolConstants.STRING_MAX_LENGTH);
        }

        offset += Encoder.writeUint8(buffer, offset, currentValueIndex);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 37;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceRemoteControlDiscreteValuesMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlDiscreteValuesMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlDiscreteValuesMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_discreteValueNames = Decoder.decodeUint8(data, offset);
        offset += 1;

        String[] discreteValueNames = new String[count_discreteValueNames];
        for (int i = 0; i < count_discreteValueNames; i++) {
            discreteValueNames[i] = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + discreteValueNames[i].length();
        }

        int currentValueIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new DeviceRemoteControlDiscreteValuesMessage(remoteControlIndex, discreteValueNames, currentValueIndex);
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
        sb.append("# DeviceRemoteControlDiscreteValues\n");
        sb.append("deviceRemoteControlDiscreteValues:\n");
        sb.append("  remoteControlIndex: ").append(getRemoteControlIndex()).append("\n");
        sb.append("  discreteValueNames:");
        if (getDiscreteValueNames().length == 0) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (String item : getDiscreteValueNames()) {
                sb.append("    - \"").append(item).append("\"\n");
            }
        }
        sb.append("  currentValueIndex: ").append(getCurrentValueIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
