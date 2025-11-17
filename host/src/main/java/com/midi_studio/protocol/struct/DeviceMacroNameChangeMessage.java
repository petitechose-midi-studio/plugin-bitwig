package com.midi_studio.protocol.struct;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.Encoder;
import com.midi_studio.protocol.Decoder;
import com.midi_studio.protocol.ProtocolConstants;

/**
 * DeviceMacroNameChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_MACRO_NAME_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceMacroNameChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_MACRO_NAME_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int parameterIndex;
    private final String parameterName;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceMacroNameChangeMessage
     *
     * @param parameterIndex The parameterIndex value
     * @param parameterName The parameterName value
     */
    public DeviceMacroNameChangeMessage(int parameterIndex, String parameterName) {
        this.parameterIndex = parameterIndex;
        this.parameterName = parameterName;
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
     * Get the parameterName value
     *
     * @return parameterName
     */
    public String getParameterName() {
        return parameterName;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 18;

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
        byte[] parameterName_encoded = Encoder.encodeString(parameterName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(parameterName_encoded, 0, buffer, offset, parameterName_encoded.length);
        offset += parameterName_encoded.length;

        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 2;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceMacroNameChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceMacroNameChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceMacroNameChangeMessage decode");
        }

        int offset = 0;

        int parameterIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        String parameterName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parameterName.length();

        return new DeviceMacroNameChangeMessage(parameterIndex, parameterName);
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
        sb.append("# DeviceMacroNameChange\n");
        sb.append("deviceMacroNameChange:\n");
        sb.append("  parameterIndex: ").append(getParameterIndex()).append("\n");
        sb.append("  parameterName: \"").append(getParameterName()).append("\"\n");
        return sb.toString();
    }
}  // class Message
