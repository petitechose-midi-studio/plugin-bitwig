package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * DeviceRemoteControlModulatedValueChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_MODULATED_VALUE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceRemoteControlModulatedValueChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROL_MODULATED_VALUE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int remoteControlIndex;
    private final float modulatedValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlModulatedValueChangeMessage
     *
     * @param remoteControlIndex The remoteControlIndex value
     * @param modulatedValue The modulatedValue value
     */
    public DeviceRemoteControlModulatedValueChangeMessage(int remoteControlIndex, float modulatedValue) {
        this.remoteControlIndex = remoteControlIndex;
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
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 6;

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
    private static final int MIN_PAYLOAD_SIZE = 6;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceRemoteControlModulatedValueChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlModulatedValueChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlModulatedValueChangeMessage decode");
        }

        int offset = 0;

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float modulatedValue = Decoder.decodeFloat32(data, offset);
        offset += 5;

        return new DeviceRemoteControlModulatedValueChangeMessage(remoteControlIndex, modulatedValue);
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
        sb.append("# DeviceRemoteControlModulatedValueChange\n");
        sb.append("deviceRemoteControlModulatedValueChange:\n");
        sb.append("  remoteControlIndex: ").append(getRemoteControlIndex()).append("\n");
        sb.append("  modulatedValue: ").append(formatFloat(getModulatedValue())).append("\n");
        return sb.toString();
    }
}  // class Message
