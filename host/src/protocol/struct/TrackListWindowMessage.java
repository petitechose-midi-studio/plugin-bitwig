package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackListWindowMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_LIST_WINDOW message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackListWindowMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_LIST_WINDOW;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackListWindow";

    // ============================================================================
    // Inner Class: Tracks
    // ============================================================================

    public static final class Tracks {
        private final int trackIndex;
        private final String trackName;
        private final long color;
        private final boolean isActivated;
        private final boolean isMute;
        private final boolean isSolo;
        private final boolean isMutedBySolo;
        private final boolean isArm;
        private final boolean isGroup;
        private final int trackType;
        private final float volume;
        private final float pan;

        public Tracks(int trackIndex, String trackName, long color, boolean isActivated, boolean isMute, boolean isSolo, boolean isMutedBySolo, boolean isArm, boolean isGroup, int trackType, float volume, float pan) {
            this.trackIndex = trackIndex;
            this.trackName = trackName;
            this.color = color;
            this.isActivated = isActivated;
            this.isMute = isMute;
            this.isSolo = isSolo;
            this.isMutedBySolo = isMutedBySolo;
            this.isArm = isArm;
            this.isGroup = isGroup;
            this.trackType = trackType;
            this.volume = volume;
            this.pan = pan;
        }

        public int getTrackIndex() {
            return trackIndex;
        }

        public String getTrackName() {
            return trackName;
        }

        public long getColor() {
            return color;
        }

        public boolean isActivated() {
            return isActivated;
        }

        public boolean isMute() {
            return isMute;
        }

        public boolean isSolo() {
            return isSolo;
        }

        public boolean isMutedBySolo() {
            return isMutedBySolo;
        }

        public boolean isArm() {
            return isArm;
        }

        public boolean isGroup() {
            return isGroup;
        }

        public int getTrackType() {
            return trackType;
        }

        public float getVolume() {
            return volume;
        }

        public float getPan() {
            return pan;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackCount;
    private final int trackStartIndex;
    private final int trackIndex;
    private final boolean isNested;
    private final String parentGroupName;
    private final Tracks[] tracks;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackListWindowMessage
     *
     * @param trackCount The trackCount value
     * @param trackStartIndex The trackStartIndex value
     * @param trackIndex The trackIndex value
     * @param isNested The isNested value
     * @param parentGroupName The parentGroupName value
     * @param tracks The tracks value
     */
    public TrackListWindowMessage(int trackCount, int trackStartIndex, int trackIndex, boolean isNested, String parentGroupName, Tracks[] tracks) {
        this.trackCount = trackCount;
        this.trackStartIndex = trackStartIndex;
        this.trackIndex = trackIndex;
        this.isNested = isNested;
        this.parentGroupName = parentGroupName;
        this.tracks = tracks;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the trackCount value
     *
     * @return trackCount
     */
    public int getTrackCount() {
        return trackCount;
    }

    /**
     * Get the trackStartIndex value
     *
     * @return trackStartIndex
     */
    public int getTrackStartIndex() {
        return trackStartIndex;
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
     * Get the isNested value
     *
     * @return isNested
     */
    public boolean isNested() {
        return isNested;
    }

    /**
     * Get the parentGroupName value
     *
     * @return parentGroupName
     */
    public String getParentGroupName() {
        return parentGroupName;
    }

    /**
     * Get the tracks value
     *
     * @return tracks
     */
    public Tracks[] getTracks() {
        return tracks;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 902;

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

        offset += Encoder.writeUint8(buffer, offset, trackCount);
        offset += Encoder.writeUint8(buffer, offset, trackStartIndex);
        offset += Encoder.writeUint8(buffer, offset, trackIndex);
        offset += Encoder.writeBool(buffer, offset, isNested);
        offset += Encoder.writeString(buffer, offset, parentGroupName, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeUint8(buffer, offset, tracks.length);

        for (Tracks item : tracks) {
            offset += Encoder.writeUint8(buffer, offset, item.getTrackIndex());
            offset += Encoder.writeString(buffer, offset, item.getTrackName(), ProtocolConstants.STRING_MAX_LENGTH);
            offset += Encoder.writeUint32(buffer, offset, item.getColor());
            offset += Encoder.writeBool(buffer, offset, item.isActivated());
            offset += Encoder.writeBool(buffer, offset, item.isMute());
            offset += Encoder.writeBool(buffer, offset, item.isSolo());
            offset += Encoder.writeBool(buffer, offset, item.isMutedBySolo());
            offset += Encoder.writeBool(buffer, offset, item.isArm());
            offset += Encoder.writeBool(buffer, offset, item.isGroup());
            offset += Encoder.writeUint8(buffer, offset, item.getTrackType());
            offset += Encoder.writeFloat32(buffer, offset, item.getVolume());
            offset += Encoder.writeFloat32(buffer, offset, item.getPan());
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 358;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackListWindowMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackListWindowMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackListWindowMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int trackStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isNested = Decoder.decodeBool(data, offset);
        offset += 1;
        String parentGroupName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parentGroupName.length();
        int count_tracks = Decoder.decodeUint8(data, offset);
        offset += 1;

        Tracks[] tracks = new Tracks[count_tracks];
        for (int i = 0; i < count_tracks; i++) {
    int item_trackIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_trackName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_trackName.length();
    long item_color = Decoder.decodeUint32(data, offset);
            offset += 4;
    boolean item_isActivated = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isMute = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isSolo = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isMutedBySolo = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isArm = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isGroup = Decoder.decodeBool(data, offset);
            offset += 1;
    int item_trackType = Decoder.decodeUint8(data, offset);
            offset += 1;
    float item_volume = Decoder.decodeFloat32(data, offset);
            offset += 4;
    float item_pan = Decoder.decodeFloat32(data, offset);
            offset += 4;
            tracks[i] = new Tracks(item_trackIndex, item_trackName, item_color, item_isActivated, item_isMute, item_isSolo, item_isMutedBySolo, item_isArm, item_isGroup, item_trackType, item_volume, item_pan);
        }


        return new TrackListWindowMessage(trackCount, trackStartIndex, trackIndex, isNested, parentGroupName, tracks);
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
        sb.append("# TrackListWindow\n");
        sb.append("trackListWindow:\n");
        sb.append("  trackCount: ").append(getTrackCount()).append("\n");
        sb.append("  trackStartIndex: ").append(getTrackStartIndex()).append("\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  isNested: ").append(isNested() ? "true" : "false").append("\n");
        sb.append("  parentGroupName: \"").append(getParentGroupName()).append("\"\n");
        sb.append("  tracks:\n");
        for (Tracks item : getTracks()) {
            sb.append("    - trackIndex: ").append(item.getTrackIndex()).append("\n");
            sb.append("      trackName: \"").append(item.getTrackName()).append("\"\n");
            sb.append("      color: ").append(item.getColor()).append("\n");
            sb.append("      isActivated: ").append(item.isActivated() ? "true" : "false").append("\n");
            sb.append("      isMute: ").append(item.isMute() ? "true" : "false").append("\n");
            sb.append("      isSolo: ").append(item.isSolo() ? "true" : "false").append("\n");
            sb.append("      isMutedBySolo: ").append(item.isMutedBySolo() ? "true" : "false").append("\n");
            sb.append("      isArm: ").append(item.isArm() ? "true" : "false").append("\n");
            sb.append("      isGroup: ").append(item.isGroup() ? "true" : "false").append("\n");
            sb.append("      trackType: ").append(item.getTrackType()).append("\n");
            sb.append("      volume: ").append(formatFloat(item.getVolume())).append("\n");
            sb.append("      pan: ").append(formatFloat(item.getPan())).append("\n");
        }
        return sb.toString();
    }
}  // class Message
