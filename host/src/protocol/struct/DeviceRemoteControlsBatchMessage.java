package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

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
    private final int hasAutomationMask;
    private final float[] values;
    private final float[] modulatedValues;
    private final String[] displayValues;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlsBatchMessage
     *
     * @param sequenceNumber The sequenceNumber value
     * @param dirtyMask The dirtyMask value
     * @param echoMask The echoMask value
     * @param hasAutomationMask The hasAutomationMask value
     * @param values The values value
     * @param modulatedValues The modulatedValues value
     * @param displayValues The displayValues value
     */
    public DeviceRemoteControlsBatchMessage(int sequenceNumber, int dirtyMask, int echoMask, int hasAutomationMask, float[] values, float[] modulatedValues, String[] displayValues) {
        this.sequenceNumber = sequenceNumber;
        this.dirtyMask = dirtyMask;
        this.echoMask = echoMask;
        this.hasAutomationMask = hasAutomationMask;
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
     * Get the hasAutomationMask value
     *
     * @return hasAutomationMask
     */
    public int getHasAutomationMask() {
        return hasAutomationMask;
    }

    /**
     * Get the values value
     *
     * @return values
     */
    public float[] getValues() {
        return values;
    }

    /**
     * Get the modulatedValues value
     *
     * @return modulatedValues
     */
    public float[] getModulatedValues() {
        return modulatedValues;
    }

    /**
     * Get the displayValues value
     *
     * @return displayValues
     */
    public String[] getDisplayValues() {
        return displayValues;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 313;

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

        offset += Encoder.writeUint8(buffer, offset, sequenceNumber);
        offset += Encoder.writeUint8(buffer, offset, dirtyMask);
        offset += Encoder.writeUint8(buffer, offset, echoMask);
        offset += Encoder.writeUint8(buffer, offset, hasAutomationMask);
        offset += Encoder.writeUint8(buffer, offset, values.length);

        for (float item : values) {
            offset += Encoder.writeNorm8(buffer, offset, item);
        }

        offset += Encoder.writeUint8(buffer, offset, modulatedValues.length);

        for (float item : modulatedValues) {
            offset += Encoder.writeNorm8(buffer, offset, item);
        }

        offset += Encoder.writeUint8(buffer, offset, displayValues.length);

        for (String item : displayValues) {
            offset += Encoder.writeString(buffer, offset, item, ProtocolConstants.STRING_MAX_LENGTH);
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 33;

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
        int hasAutomationMask = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_values = Decoder.decodeUint8(data, offset);
        offset += 1;

        float[] values = new float[count_values];
        for (int i = 0; i < count_values; i++) {
            values[i] = Decoder.decodeNorm8(data, offset);
            offset += 1;
        }

        int count_modulatedValues = Decoder.decodeUint8(data, offset);
        offset += 1;

        float[] modulatedValues = new float[count_modulatedValues];
        for (int i = 0; i < count_modulatedValues; i++) {
            modulatedValues[i] = Decoder.decodeNorm8(data, offset);
            offset += 1;
        }

        int count_displayValues = Decoder.decodeUint8(data, offset);
        offset += 1;

        String[] displayValues = new String[count_displayValues];
        for (int i = 0; i < count_displayValues; i++) {
            displayValues[i] = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + displayValues[i].length();
        }


        return new DeviceRemoteControlsBatchMessage(sequenceNumber, dirtyMask, echoMask, hasAutomationMask, values, modulatedValues, displayValues);
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
        sb.append("  hasAutomationMask: ").append(getHasAutomationMask()).append("\n");
        sb.append("  values:");
        if (getValues().length == 0) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (float item : getValues()) {
                sb.append("    - ").append(formatFloat(item)).append("\n");
            }
        }
        sb.append("  modulatedValues:");
        if (getModulatedValues().length == 0) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (float item : getModulatedValues()) {
                sb.append("    - ").append(formatFloat(item)).append("\n");
            }
        }
        sb.append("  displayValues:");
        if (getDisplayValues().length == 0) {
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
