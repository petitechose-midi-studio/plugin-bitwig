package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackVolumeChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_VOLUME_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackVolumeChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_VOLUME_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackVolumeChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final float volume;
    private final String volumeDisplay;
    private final boolean isEcho;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackVolumeChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param volume The volume value
     * @param volumeDisplay The volumeDisplay value
     * @param isEcho The isEcho value
     */
    public TrackVolumeChangeMessage(int trackIndex, float volume, String volumeDisplay, boolean isEcho) {
        this.trackIndex = trackIndex;
        this.volume = volume;
        this.volumeDisplay = volumeDisplay;
        this.isEcho = isEcho;
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
     * Get the volume value
     *
     * @return volume
     */
    public float getVolume() {
        return volume;
    }

    /**
     * Get the volumeDisplay value
     *
     * @return volumeDisplay
     */
    public String getVolumeDisplay() {
        return volumeDisplay;
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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 57;

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
        offset += Encoder.writeFloat32(buffer, offset, volume);
        offset += Encoder.writeString(buffer, offset, volumeDisplay, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeBool(buffer, offset, isEcho);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 25;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackVolumeChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackVolumeChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackVolumeChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float volume = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String volumeDisplay = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + volumeDisplay.length();
        boolean isEcho = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackVolumeChangeMessage(trackIndex, volume, volumeDisplay, isEcho);
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
        sb.append("# TrackVolumeChange\n");
        sb.append("trackVolumeChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  volume: ").append(formatFloat(getVolume())).append("\n");
        sb.append("  volumeDisplay: \"").append(getVolumeDisplay()).append("\"\n");
        sb.append("  isEcho: ").append(isEcho() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
