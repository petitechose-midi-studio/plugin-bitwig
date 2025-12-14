package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * DeviceMacroTouchMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_MACRO_TOUCH message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceMacroTouchMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_MACRO_TOUCH;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int parameterIndex;
    private final boolean isTouched;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceMacroTouchMessage
     *
     * @param parameterIndex The parameterIndex value
     * @param isTouched The isTouched value
     */
    public DeviceMacroTouchMessage(int parameterIndex, boolean isTouched) {
        this.parameterIndex = parameterIndex;
        this.isTouched = isTouched;
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
     * Get the isTouched value
     *
     * @return isTouched
     */
    public boolean isTouched() {
        return isTouched;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 2;

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
        byte[] isTouched_encoded = Encoder.encodeBool(isTouched);
        System.arraycopy(isTouched_encoded, 0, buffer, offset, isTouched_encoded.length);
        offset += isTouched_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
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
     * @return Decoded DeviceMacroTouchMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceMacroTouchMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceMacroTouchMessage decode");
        }

        int offset = 0;

        int parameterIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isTouched = Decoder.decodeBool(data, offset);
        offset += 1;

        return new DeviceMacroTouchMessage(parameterIndex, isTouched);
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
        sb.append("# DeviceMacroTouch\n");
        sb.append("deviceMacroTouch:\n");
        sb.append("  parameterIndex: ").append(getParameterIndex()).append("\n");
        sb.append("  isTouched: ").append(isTouched() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
