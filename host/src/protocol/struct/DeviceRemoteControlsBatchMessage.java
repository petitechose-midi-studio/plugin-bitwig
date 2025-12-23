package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceRemoteControlsBatchMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROLS_BATCH message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceRemoteControlsBatchMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROLS_BATCH;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceRemoteControlsBatch";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int sequenceNumber;
    private final int dirtyMask;
    private final int echoMask;
    private final List<Float> values;
    private final List<Float> modulatedValues;
    private final List<String> displayValues;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlsBatchMessage
     *
     * @param sequenceNumber The sequenceNumber value
     * @param dirtyMask The dirtyMask value
     * @param echoMask The echoMask value
     * @param values The values value
     * @param modulatedValues The modulatedValues value
     * @param displayValues The displayValues value
     */
    public DeviceRemoteControlsBatchMessage(int sequenceNumber, int dirtyMask, int echoMask, List<Float> values, List<Float> modulatedValues, List<String> displayValues) {
        this.sequenceNumber = sequenceNumber;
        this.dirtyMask = dirtyMask;
        this.echoMask = echoMask;
        this.values = values;
        this.modulatedValues = modulatedValues;
        this.displayValues = displayValues;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the sequenceNumber value
     *
     * @return sequenceNumber
     */
    public int getSequenceNumber() {
        return sequenceNumber;
    }

    /**
     * Get the dirtyMask value
     *
     * @return dirtyMask
     */
    public int getDirtyMask() {
        return dirtyMask;
    }

    /**
     * Get the echoMask value
     *
     * @return echoMask
     */
    public int getEchoMask() {
        return echoMask;
    }

    /**
     * Get the values value
     *
     * @return values
     */
    public List<Float> getValues() {
        return values;
    }

    /**
     * Get the modulatedValues value
     *
     * @return modulatedValues
     */
    public List<Float> getModulatedValues() {
        return modulatedValues;
    }

    /**
     * Get the displayValues value
     *
     * @return displayValues
     */
    public List<String> getDisplayValues() {
        return displayValues;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 312;

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

        offset += Encoder.writeUint8(buffer, offset, sequenceNumber);
        offset += Encoder.writeUint8(buffer, offset, dirtyMask);
        offset += Encoder.writeUint8(buffer, offset, echoMask);
        offset += Encoder.writeUint8(buffer, offset, values.size());

        for (float item : values) {
            offset += Encoder.writeNorm8(buffer, offset, item);
        }

        offset += Encoder.writeUint8(buffer, offset, modulatedValues.size());

        for (float item : modulatedValues) {
            offset += Encoder.writeNorm8(buffer, offset, item);
        }

        offset += Encoder.writeUint8(buffer, offset, displayValues.size());

        for (String item : displayValues) {
            offset += Encoder.writeString(buffer, offset, item, ProtocolConstants.STRING_MAX_LENGTH);
        }


        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 32;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceRemoteControlsBatchMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlsBatchMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlsBatchMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int sequenceNumber = Decoder.decodeUint8(data, offset);
        offset += 1;
        int dirtyMask = Decoder.decodeUint8(data, offset);
        offset += 1;
        int echoMask = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_values = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<Float> values_list = new ArrayList<>();
        for (int i = 0; i < count_values; i++) {
    float item_values = Decoder.decodeNorm8(data, offset);
            offset += 1;
            values_list.add(item_values);
        }

        int count_modulatedValues = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<Float> modulatedValues_list = new ArrayList<>();
        for (int i = 0; i < count_modulatedValues; i++) {
    float item_modulatedValues = Decoder.decodeNorm8(data, offset);
            offset += 1;
            modulatedValues_list.add(item_modulatedValues);
        }

        int count_displayValues = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<String> displayValues_list = new ArrayList<>();
        for (int i = 0; i < count_displayValues; i++) {
    String item_displayValues = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_displayValues.length();
            displayValues_list.add(item_displayValues);
        }


        return new DeviceRemoteControlsBatchMessage(sequenceNumber, dirtyMask, echoMask, values_list, modulatedValues_list, displayValues_list);
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
        sb.append("# DeviceRemoteControlsBatch\n");
        sb.append("deviceRemoteControlsBatch:\n");
        sb.append("  sequenceNumber: ").append(getSequenceNumber()).append("\n");
        sb.append("  dirtyMask: ").append(getDirtyMask()).append("\n");
        sb.append("  echoMask: ").append(getEchoMask()).append("\n");
        sb.append("  values:");
        if (getValues().isEmpty()) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (float item : getValues()) {
                sb.append("    - ").append(formatFloat(item)).append("\n");
            }
        }
        sb.append("  modulatedValues:");
        if (getModulatedValues().isEmpty()) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (float item : getModulatedValues()) {
                sb.append("    - ").append(formatFloat(item)).append("\n");
            }
        }
        sb.append("  displayValues:");
        if (getDisplayValues().isEmpty()) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (String item : getDisplayValues()) {
                sb.append("    - \"").append(item).append("\"\n");
            }
        }
        return sb.toString();
    }
}  // class Message
