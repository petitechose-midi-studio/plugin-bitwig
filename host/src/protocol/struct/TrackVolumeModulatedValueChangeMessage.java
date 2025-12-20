package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackVolumeModulatedValueChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_VOLUME_MODULATED_VALUE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackVolumeModulatedValueChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_VOLUME_MODULATED_VALUE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final float modulatedValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackVolumeModulatedValueChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param modulatedValue The modulatedValue value
     */
    public TrackVolumeModulatedValueChangeMessage(int trackIndex, float modulatedValue) {
        this.trackIndex = trackIndex;
        this.modulatedValue = modulatedValue;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the trackIndex value
     *
     * @return trackIndex
     */
    public int getTrackIndex() {
        return trackIndex;
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
    public static final int MAX_PAYLOAD_SIZE = 5;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
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
    private static final int MIN_PAYLOAD_SIZE = 5;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackVolumeModulatedValueChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackVolumeModulatedValueChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackVolumeModulatedValueChangeMessage decode");
        }

        int offset = 0;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float modulatedValue = Decoder.decodeFloat32(data, offset);
        offset += 4;

        return new TrackVolumeModulatedValueChangeMessage(trackIndex, modulatedValue);
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
        sb.append("# TrackVolumeModulatedValueChange\n");
        sb.append("trackVolumeModulatedValueChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  modulatedValue: ").append(formatFloat(getModulatedValue())).append("\n");
        return sb.toString();
    }
}  // class Message
