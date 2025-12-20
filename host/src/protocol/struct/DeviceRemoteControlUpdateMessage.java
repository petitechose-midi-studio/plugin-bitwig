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
    public static final int MAX_PAYLOAD_SIZE = 85;

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
        byte[] parameterName_encoded = Encoder.encodeString(parameterName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(parameterName_encoded, 0, buffer, offset, parameterName_encoded.length);
        offset += parameterName_encoded.length;
        byte[] parameterValue_encoded = Encoder.encodeFloat32(parameterValue);
        System.arraycopy(parameterValue_encoded, 0, buffer, offset, parameterValue_encoded.length);
        offset += parameterValue_encoded.length;
        byte[] displayValue_encoded = Encoder.encodeString(displayValue, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(displayValue_encoded, 0, buffer, offset, displayValue_encoded.length);
        offset += displayValue_encoded.length;
        byte[] parameterOrigin_encoded = Encoder.encodeFloat32(parameterOrigin);
        System.arraycopy(parameterOrigin_encoded, 0, buffer, offset, parameterOrigin_encoded.length);
        offset += parameterOrigin_encoded.length;
        byte[] parameterExists_encoded = Encoder.encodeBool(parameterExists);
        System.arraycopy(parameterExists_encoded, 0, buffer, offset, parameterExists_encoded.length);
        offset += parameterExists_encoded.length;
        byte[] parameterType_encoded = Encoder.encodeUint8(parameterType);
        System.arraycopy(parameterType_encoded, 0, buffer, offset, parameterType_encoded.length);
        offset += parameterType_encoded.length;
        byte[] discreteValueCount_encoded = Encoder.encodeInt16(discreteValueCount);
        System.arraycopy(discreteValueCount_encoded, 0, buffer, offset, discreteValueCount_encoded.length);
        offset += discreteValueCount_encoded.length;
        byte[] currentValueIndex_encoded = Encoder.encodeUint8(currentValueIndex);
        System.arraycopy(currentValueIndex_encoded, 0, buffer, offset, currentValueIndex_encoded.length);
        offset += currentValueIndex_encoded.length;
        byte[] hasAutomation_encoded = Encoder.encodeBool(hasAutomation);
        System.arraycopy(hasAutomation_encoded, 0, buffer, offset, hasAutomation_encoded.length);
        offset += hasAutomation_encoded.length;
        byte[] modulatedValue_encoded = Encoder.encodeFloat32(modulatedValue);
        System.arraycopy(modulatedValue_encoded, 0, buffer, offset, modulatedValue_encoded.length);
        offset += modulatedValue_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 21;

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
