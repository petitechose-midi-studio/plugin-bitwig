package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackChange";


    // ============================================================================
    // Fields
    // ============================================================================

    private final String trackName;
    private final long color;
    private final int trackIndex;
    private final int trackType;
    private final boolean isActivated;
    private final boolean isMute;
    private final boolean isSolo;
    private final boolean isMutedBySolo;
    private final boolean isArm;
    private final float volume;
    private final String volumeDisplay;
    private final float pan;
    private final String panDisplay;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackChangeMessage
     *
     * @param trackName The trackName value
     * @param color The color value
     * @param trackIndex The trackIndex value
     * @param trackType The trackType value
     * @param isActivated The isActivated value
     * @param isMute The isMute value
     * @param isSolo The isSolo value
     * @param isMutedBySolo The isMutedBySolo value
     * @param isArm The isArm value
     * @param volume The volume value
     * @param volumeDisplay The volumeDisplay value
     * @param pan The pan value
     * @param panDisplay The panDisplay value
     */
    public TrackChangeMessage(String trackName, long color, int trackIndex, int trackType, boolean isActivated, boolean isMute, boolean isSolo, boolean isMutedBySolo, boolean isArm, float volume, String volumeDisplay, float pan, String panDisplay) {
        this.trackName = trackName;
        this.color = color;
        this.trackIndex = trackIndex;
        this.trackType = trackType;
        this.isActivated = isActivated;
        this.isMute = isMute;
        this.isSolo = isSolo;
        this.isMutedBySolo = isMutedBySolo;
        this.isArm = isArm;
        this.volume = volume;
        this.volumeDisplay = volumeDisplay;
        this.pan = pan;
        this.panDisplay = panDisplay;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the trackName value
     *
     * @return trackName
     */
    public String getTrackName() {
        return trackName;
    }

    /**
     * Get the color value
     *
     * @return color
     */
    public long getColor() {
        return color;
    }

    /**
     * Get the trackIndex value
     *
     * @return trackIndex
     */
    public int getTrackIndex() {
        return trackIndex;
    }

    /**
     * Get the trackType value
     *
     * @return trackType
     */
    public int getTrackType() {
        return trackType;
    }

    /**
     * Get the isActivated value
     *
     * @return isActivated
     */
    public boolean isActivated() {
        return isActivated;
    }

    /**
     * Get the isMute value
     *
     * @return isMute
     */
    public boolean isMute() {
        return isMute;
    }

    /**
     * Get the isSolo value
     *
     * @return isSolo
     */
    public boolean isSolo() {
        return isSolo;
    }

    /**
     * Get the isMutedBySolo value
     *
     * @return isMutedBySolo
     */
    public boolean isMutedBySolo() {
        return isMutedBySolo;
    }

    /**
     * Get the isArm value
     *
     * @return isArm
     */
    public boolean isArm() {
        return isArm;
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
     * Get the pan value
     *
     * @return pan
     */
    public float getPan() {
        return pan;
    }

    /**
     * Get the panDisplay value
     *
     * @return panDisplay
     */
    public String getPanDisplay() {
        return panDisplay;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 130;

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

        offset += Encoder.writeString(buffer, offset, trackName, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeUint32(buffer, offset, color);
        offset += Encoder.writeUint8(buffer, offset, trackIndex);
        offset += Encoder.writeUint8(buffer, offset, trackType);
        offset += Encoder.writeBool(buffer, offset, isActivated);
        offset += Encoder.writeBool(buffer, offset, isMute);
        offset += Encoder.writeBool(buffer, offset, isSolo);
        offset += Encoder.writeBool(buffer, offset, isMutedBySolo);
        offset += Encoder.writeBool(buffer, offset, isArm);
        offset += Encoder.writeFloat32(buffer, offset, volume);
        offset += Encoder.writeString(buffer, offset, volumeDisplay, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeFloat32(buffer, offset, pan);
        offset += Encoder.writeString(buffer, offset, panDisplay, ProtocolConstants.STRING_MAX_LENGTH);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 34;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        String trackName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + trackName.length();
        long color = Decoder.decodeUint32(data, offset);
        offset += 4;
        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int trackType = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isActivated = Decoder.decodeBool(data, offset);
        offset += 1;
        boolean isMute = Decoder.decodeBool(data, offset);
        offset += 1;
        boolean isSolo = Decoder.decodeBool(data, offset);
        offset += 1;
        boolean isMutedBySolo = Decoder.decodeBool(data, offset);
        offset += 1;
        boolean isArm = Decoder.decodeBool(data, offset);
        offset += 1;
        float volume = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String volumeDisplay = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + volumeDisplay.length();
        float pan = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String panDisplay = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + panDisplay.length();

        return new TrackChangeMessage(trackName, color, trackIndex, trackType, isActivated, isMute, isSolo, isMutedBySolo, isArm, volume, volumeDisplay, pan, panDisplay);
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
        sb.append("# TrackChange\n");
        sb.append("trackChange:\n");
        sb.append("  trackName: \"").append(getTrackName()).append("\"\n");
        sb.append("  color: ").append(getColor()).append("\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  trackType: ").append(getTrackType()).append("\n");
        sb.append("  isActivated: ").append(isActivated() ? "true" : "false").append("\n");
        sb.append("  isMute: ").append(isMute() ? "true" : "false").append("\n");
        sb.append("  isSolo: ").append(isSolo() ? "true" : "false").append("\n");
        sb.append("  isMutedBySolo: ").append(isMutedBySolo() ? "true" : "false").append("\n");
        sb.append("  isArm: ").append(isArm() ? "true" : "false").append("\n");
        sb.append("  volume: ").append(formatFloat(getVolume())).append("\n");
        sb.append("  volumeDisplay: \"").append(getVolumeDisplay()).append("\"\n");
        sb.append("  pan: ").append(formatFloat(getPan())).append("\n");
        sb.append("  panDisplay: \"").append(getPanDisplay()).append("\"\n");
        return sb.toString();
    }
}  // class Message
