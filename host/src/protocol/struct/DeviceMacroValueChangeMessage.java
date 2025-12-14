package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * DeviceMacroValueChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_MACRO_VALUE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceMacroValueChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_MACRO_VALUE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int parameterIndex;
    private final float parameterValue;
    private final String displayValue;
    private final boolean isEcho;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceMacroValueChangeMessage
     *
     * @param parameterIndex The parameterIndex value
     * @param parameterValue The parameterValue value
     * @param displayValue The displayValue value
     * @param isEcho The isEcho value
     */
    public DeviceMacroValueChangeMessage(int parameterIndex, float parameterValue, String displayValue, boolean isEcho) {
        this.parameterIndex = parameterIndex;
        this.parameterValue = parameterValue;
        this.displayValue = displayValue;
        this.isEcho = isEcho;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the parameterIndex value
     *
     * @return parameterIndex
     */
    public int getParameterIndex() {
        return parameterIndex;
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
     * Get the isEcho value
     *
     * @return isEcho
     */
    public boolean isEcho() {
        return isEcho;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 40;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] parameterIndex_encoded = Encoder.encodeUint8(parameterIndex);
        System.arraycopy(parameterIndex_encoded, 0, buffer, offset, parameterIndex_encoded.length);
        offset += parameterIndex_encoded.length;
        byte[] parameterValue_encoded = Encoder.encodeFloat32(parameterValue);
        System.arraycopy(parameterValue_encoded, 0, buffer, offset, parameterValue_encoded.length);
        offset += parameterValue_encoded.length;
        byte[] displayValue_encoded = Encoder.encodeString(displayValue, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(displayValue_encoded, 0, buffer, offset, displayValue_encoded.length);
        offset += displayValue_encoded.length;
        byte[] isEcho_encoded = Encoder.encodeBool(isEcho);
        System.arraycopy(isEcho_encoded, 0, buffer, offset, isEcho_encoded.length);
        offset += isEcho_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 8;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceMacroValueChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceMacroValueChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceMacroValueChangeMessage decode");
        }

        int offset = 0;

        int parameterIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float parameterValue = Decoder.decodeFloat32(data, offset);
        offset += 5;
        String displayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + displayValue.length();
        boolean isEcho = Decoder.decodeBool(data, offset);
        offset += 1;

        return new DeviceMacroValueChangeMessage(parameterIndex, parameterValue, displayValue, isEcho);
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
        sb.append("# DeviceMacroValueChange\n");
        sb.append("deviceMacroValueChange:\n");
        sb.append("  parameterIndex: ").append(getParameterIndex()).append("\n");
        sb.append("  parameterValue: ").append(formatFloat(getParameterValue())).append("\n");
        sb.append("  displayValue: \"").append(getDisplayValue()).append("\"\n");
        sb.append("  isEcho: ").append(isEcho() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
