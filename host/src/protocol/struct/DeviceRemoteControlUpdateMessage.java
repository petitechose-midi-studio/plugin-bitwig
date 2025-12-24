package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * DeviceRemoteControlUpdateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_UPDATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceRemoteControlUpdateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROL_UPDATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "DeviceRemoteControlUpdate";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int remoteControlIndex;
    private final String parameterName;
    private final float parameterValue;
    private final String displayValue;
    private final float parameterOrigin;
    private final boolean parameterExists;
    private final int parameterType;
    private final short discreteValueCount;
    private final int currentValueIndex;
    private final boolean hasAutomation;
    private final float modulatedValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlUpdateMessage
     *
     * @param remoteControlIndex The remoteControlIndex value
     * @param parameterName The parameterName value
     * @param parameterValue The parameterValue value
     * @param displayValue The displayValue value
     * @param parameterOrigin The parameterOrigin value
     * @param parameterExists The parameterExists value
     * @param parameterType The parameterType value
     * @param discreteValueCount The discreteValueCount value
     * @param currentValueIndex The currentValueIndex value
     * @param hasAutomation The hasAutomation value
     * @param modulatedValue The modulatedValue value
     */
    public DeviceRemoteControlUpdateMessage(int remoteControlIndex, String parameterName, float parameterValue, String displayValue, float parameterOrigin, boolean parameterExists, int parameterType, short discreteValueCount, int currentValueIndex, boolean hasAutomation, float modulatedValue) {
        this.remoteControlIndex = remoteControlIndex;
        this.parameterName = parameterName;
        this.parameterValue = parameterValue;
        this.displayValue = displayValue;
        this.parameterOrigin = parameterOrigin;
        this.parameterExists = parameterExists;
        this.parameterType = parameterType;
        this.discreteValueCount = discreteValueCount;
        this.currentValueIndex = currentValueIndex;
        this.hasAutomation = hasAutomation;
        this.modulatedValue = modulatedValue;
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
     * Get the parameterName value
     *
     * @return parameterName
     */
    public String getParameterName() {
        return parameterName;
    }

    /**
     * Get the parameterValue value
     *
     * @return parameterValue
     */
    public float getParameterValue() {
        return parameterValue;
    }

    /**
     * Get the displayValue value
     *
     * @return displayValue
     */
    public String getDisplayValue() {
        return displayValue;
    }

    /**
     * Get the parameterOrigin value
     *
     * @return parameterOrigin
     */
    public float getParameterOrigin() {
        return parameterOrigin;
    }

    /**
     * Get the parameterExists value
     *
     * @return parameterExists
     */
    public boolean getParameterExists() {
        return parameterExists;
    }

    /**
     * Get the parameterType value
     *
     * @return parameterType
     */
    public int getParameterType() {
        return parameterType;
    }

    /**
     * Get the discreteValueCount value
     *
     * @return discreteValueCount
     */
    public short getDiscreteValueCount() {
        return discreteValueCount;
    }

    /**
     * Get the currentValueIndex value
     *
     * @return currentValueIndex
     */
    public int getCurrentValueIndex() {
        return currentValueIndex;
    }

    /**
     * Get the hasAutomation value
     *
     * @return hasAutomation
     */
    public boolean getHasAutomation() {
        return hasAutomation;
    }

    /**
     * Get the modulatedValue value
     *
     * @return modulatedValue
     */
    public float getModulatedValue() {
        return modulatedValue;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 111;

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
        offset += Encoder.writeString(buffer, offset, parameterName, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeFloat32(buffer, offset, parameterValue);
        offset += Encoder.writeString(buffer, offset, displayValue, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeFloat32(buffer, offset, parameterOrigin);
        offset += Encoder.writeBool(buffer, offset, parameterExists);
        offset += Encoder.writeUint8(buffer, offset, parameterType);
        offset += Encoder.writeInt16(buffer, offset, discreteValueCount);
        offset += Encoder.writeUint8(buffer, offset, currentValueIndex);
        offset += Encoder.writeBool(buffer, offset, hasAutomation);
        offset += Encoder.writeFloat32(buffer, offset, modulatedValue);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 47;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceRemoteControlUpdateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlUpdateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlUpdateMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        String parameterName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parameterName.length();
        float parameterValue = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String displayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + displayValue.length();
        float parameterOrigin = Decoder.decodeFloat32(data, offset);
        offset += 4;
        boolean parameterExists = Decoder.decodeBool(data, offset);
        offset += 1;
        int parameterType = Decoder.decodeUint8(data, offset);
        offset += 1;
        short discreteValueCount = Decoder.decodeInt16(data, offset);
        offset += 2;
        int currentValueIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean hasAutomation = Decoder.decodeBool(data, offset);
        offset += 1;
        float modulatedValue = Decoder.decodeFloat32(data, offset);
        offset += 4;

        return new DeviceRemoteControlUpdateMessage(remoteControlIndex, parameterName, parameterValue, displayValue, parameterOrigin, parameterExists, parameterType, discreteValueCount, currentValueIndex, hasAutomation, modulatedValue);
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
        sb.append("# DeviceRemoteControlUpdate\n");
        sb.append("deviceRemoteControlUpdate:\n");
        sb.append("  remoteControlIndex: ").append(getRemoteControlIndex()).append("\n");
        sb.append("  parameterName: \"").append(getParameterName()).append("\"\n");
        sb.append("  parameterValue: ").append(formatFloat(getParameterValue())).append("\n");
        sb.append("  displayValue: \"").append(getDisplayValue()).append("\"\n");
        sb.append("  parameterOrigin: ").append(formatFloat(getParameterOrigin())).append("\n");
        sb.append("  parameterExists: ").append(getParameterExists() ? "true" : "false").append("\n");
        sb.append("  parameterType: ").append(getParameterType()).append("\n");
        sb.append("  discreteValueCount: ").append(getDiscreteValueCount()).append("\n");
        sb.append("  currentValueIndex: ").append(getCurrentValueIndex()).append("\n");
        sb.append("  hasAutomation: ").append(getHasAutomation() ? "true" : "false").append("\n");
        sb.append("  modulatedValue: ").append(formatFloat(getModulatedValue())).append("\n");
        return sb.toString();
    }
}  // class Message
