package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

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


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int remoteControlIndex;
    private final List<String> discreteValueNames;
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
    public DeviceRemoteControlDiscreteValuesMessage(int remoteControlIndex, List<String> discreteValueNames, int currentValueIndex) {
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
    public List<String> getDiscreteValueNames() {
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
    public static final int MAX_PAYLOAD_SIZE = 1059;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] remoteControlIndex_encoded = Encoder.encodeUint8(remoteControlIndex);
        System.arraycopy(remoteControlIndex_encoded, 0, buffer, offset, remoteControlIndex_encoded.length);
        offset += remoteControlIndex_encoded.length;
        byte[] discreteValueNames_count = Encoder.encodeUint8(discreteValueNames.size());
        System.arraycopy(discreteValueNames_count, 0, buffer, offset, 1);
        offset += 1;

        for (String item : discreteValueNames) {
    byte[] item_encoded = Encoder.encodeString(item, ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_encoded, 0, buffer, offset, item_encoded.length);
            offset += item_encoded.length;
        }

        byte[] currentValueIndex_encoded = Encoder.encodeUint8(currentValueIndex);
        System.arraycopy(currentValueIndex_encoded, 0, buffer, offset, currentValueIndex_encoded.length);
        offset += currentValueIndex_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 3;

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

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_discreteValueNames = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<String> discreteValueNames_list = new ArrayList<>();
        for (int i = 0; i < count_discreteValueNames; i++) {
    String item_discreteValueNames = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_discreteValueNames.length();
            discreteValueNames_list.add(item_discreteValueNames);
        }

        int currentValueIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new DeviceRemoteControlDiscreteValuesMessage(remoteControlIndex, discreteValueNames_list, currentValueIndex);
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
        if (getDiscreteValueNames().isEmpty()) {
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
