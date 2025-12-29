package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackPanModulatedValueStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_PAN_MODULATED_VALUE_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackPanModulatedValueStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_PAN_MODULATED_VALUE_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackPanModulatedValueState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int trackIndex;
    private final float modulatedValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackPanModulatedValueStateMessage
     *
     * @param trackIndex The trackIndex value
     * @param modulatedValue The modulatedValue value
     */
    public TrackPanModulatedValueStateMessage(int trackIndex, float modulatedValue) {
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
    public static final int MAX_PAYLOAD_SIZE = 33;

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

        offset += Encoder.writeUint8(buffer, offset, trackIndex);
        offset += Encoder.writeFloat32(buffer, offset, modulatedValue);

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
     * @return Decoded TrackPanModulatedValueStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackPanModulatedValueStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackPanModulatedValueStateMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float modulatedValue = Decoder.decodeFloat32(data, offset);
        offset += 4;

        return new TrackPanModulatedValueStateMessage(trackIndex, modulatedValue);
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
        sb.append("# TrackPanModulatedValueState\n");
        sb.append("trackPanModulatedValueState:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  modulatedValue: ").append(formatFloat(getModulatedValue())).append("\n");
        return sb.toString();
    }
}  // class Message
