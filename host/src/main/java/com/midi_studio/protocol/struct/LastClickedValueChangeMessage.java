package com.midi_studio.protocol.struct;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.Encoder;
import com.midi_studio.protocol.Decoder;
import com.midi_studio.protocol.ProtocolConstants;

/**
 * LastClickedValueChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: LAST_CLICKED_VALUE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class LastClickedValueChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.LAST_CLICKED_VALUE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final float parameterValue;
    private final String displayValue;
    private final boolean isEcho;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new LastClickedValueChangeMessage
     *
     * @param parameterValue The parameterValue value
     * @param displayValue The displayValue value
     * @param isEcho The isEcho value
     */
    public LastClickedValueChangeMessage(float parameterValue, String displayValue, boolean isEcho) {
        this.parameterValue = parameterValue;
        this.displayValue = displayValue;
        this.isEcho = isEcho;
    }

    // ============================================================================
    // Getters
    // ============================================================================

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
    public static final int MAX_PAYLOAD_SIZE = 23;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] parameterValue_encoded = Encoder.encodeFloat32(parameterValue);
        System.arraycopy(parameterValue_encoded, 0, buffer, offset, parameterValue_encoded.length);
        offset += parameterValue_encoded.length;
        byte[] displayValue_encoded = Encoder.encodeString(displayValue, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(displayValue_encoded, 0, buffer, offset, displayValue_encoded.length);
        offset += displayValue_encoded.length;
        byte[] isEcho_encoded = Encoder.encodeBool(isEcho);
        System.arraycopy(isEcho_encoded, 0, buffer, offset, isEcho_encoded.length);
        offset += isEcho_encoded.length;

        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 7;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded LastClickedValueChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static LastClickedValueChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for LastClickedValueChangeMessage decode");
        }

        int offset = 0;

        float parameterValue = Decoder.decodeFloat32(data, offset);
        offset += 5;
        String displayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + displayValue.length();
        boolean isEcho = Decoder.decodeBool(data, offset);
        offset += 1;

        return new LastClickedValueChangeMessage(parameterValue, displayValue, isEcho);
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
        sb.append("# LastClickedValueChange\n");
        sb.append("lastClickedValueChange:\n");
        sb.append("  parameterValue: ").append(formatFloat(getParameterValue())).append("\n");
        sb.append("  displayValue: \"").append(getDisplayValue()).append("\"\n");
        sb.append("  isEcho: ").append(isEcho() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
