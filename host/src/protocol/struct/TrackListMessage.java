package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * TrackListMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_LIST message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TrackListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_LIST;

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
        private final boolean isGroup;
        private final int trackType;

        public Tracks(int trackIndex, String trackName, long color, boolean isActivated, boolean isMute, boolean isSolo, boolean isGroup, int trackType) {
            this.trackIndex = trackIndex;
            this.trackName = trackName;
            this.color = color;
            this.isActivated = isActivated;
            this.isMute = isMute;
            this.isSolo = isSolo;
            this.isGroup = isGroup;
            this.trackType = trackType;
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

        public boolean isGroup() {
            return isGroup;
        }

        public int getTrackType() {
            return trackType;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackCount;
    private final int trackIndex;
    private final boolean isNested;
    private final String parentGroupName;
    private final List<Tracks> tracks;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackListMessage
     *
     * @param trackCount The trackCount value
     * @param trackIndex The trackIndex value
     * @param isNested The isNested value
     * @param parentGroupName The parentGroupName value
     * @param tracks The tracks value
     */
    public TrackListMessage(int trackCount, int trackIndex, boolean isNested, String parentGroupName, List<Tracks> tracks) {
        this.trackCount = trackCount;
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
    public List<Tracks> getTracks() {
        return tracks;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 1445;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] trackCount_encoded = Encoder.encodeUint8(trackCount);
        System.arraycopy(trackCount_encoded, 0, buffer, offset, trackCount_encoded.length);
        offset += trackCount_encoded.length;
        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] isNested_encoded = Encoder.encodeBool(isNested);
        System.arraycopy(isNested_encoded, 0, buffer, offset, isNested_encoded.length);
        offset += isNested_encoded.length;
        byte[] parentGroupName_encoded = Encoder.encodeString(parentGroupName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(parentGroupName_encoded, 0, buffer, offset, parentGroupName_encoded.length);
        offset += parentGroupName_encoded.length;
        byte[] tracks_count = Encoder.encodeUint8(tracks.size());
        System.arraycopy(tracks_count, 0, buffer, offset, 1);
        offset += 1;

        for (Tracks item : tracks) {
    byte[] item_trackIndex_encoded = Encoder.encodeUint8(item.getTrackIndex());
            System.arraycopy(item_trackIndex_encoded, 0, buffer, offset, item_trackIndex_encoded.length);
            offset += item_trackIndex_encoded.length;
    byte[] item_trackName_encoded = Encoder.encodeString(item.getTrackName(), ProtocolConstants.STRING_MAX_LENGTH);
            System.arraycopy(item_trackName_encoded, 0, buffer, offset, item_trackName_encoded.length);
            offset += item_trackName_encoded.length;
    byte[] item_color_encoded = Encoder.encodeUint32(item.getColor());
            System.arraycopy(item_color_encoded, 0, buffer, offset, item_color_encoded.length);
            offset += item_color_encoded.length;
    byte[] item_isActivated_encoded = Encoder.encodeBool(item.isActivated());
            System.arraycopy(item_isActivated_encoded, 0, buffer, offset, item_isActivated_encoded.length);
            offset += item_isActivated_encoded.length;
    byte[] item_isMute_encoded = Encoder.encodeBool(item.isMute());
            System.arraycopy(item_isMute_encoded, 0, buffer, offset, item_isMute_encoded.length);
            offset += item_isMute_encoded.length;
    byte[] item_isSolo_encoded = Encoder.encodeBool(item.isSolo());
            System.arraycopy(item_isSolo_encoded, 0, buffer, offset, item_isSolo_encoded.length);
            offset += item_isSolo_encoded.length;
    byte[] item_isGroup_encoded = Encoder.encodeBool(item.isGroup());
            System.arraycopy(item_isGroup_encoded, 0, buffer, offset, item_isGroup_encoded.length);
            offset += item_isGroup_encoded.length;
    byte[] item_trackType_encoded = Encoder.encodeUint8(item.getTrackType());
            System.arraycopy(item_trackType_encoded, 0, buffer, offset, item_trackType_encoded.length);
            offset += item_trackType_encoded.length;
        }


        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 389;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackListMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackListMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackListMessage decode");
        }

        int offset = 0;

        int trackCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isNested = Decoder.decodeBool(data, offset);
        offset += 1;
        String parentGroupName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parentGroupName.length();
        int count_tracks = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<Tracks> tracks_list = new ArrayList<>();
        for (int i = 0; i < count_tracks; i++) {
    int item_trackIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_trackName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_trackName.length();
    long item_color = Decoder.decodeUint32(data, offset);
            offset += 5;
    boolean item_isActivated = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isMute = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isSolo = Decoder.decodeBool(data, offset);
            offset += 1;
    boolean item_isGroup = Decoder.decodeBool(data, offset);
            offset += 1;
    int item_trackType = Decoder.decodeUint8(data, offset);
            offset += 1;
            tracks_list.add(new Tracks(item_trackIndex, item_trackName, item_color, item_isActivated, item_isMute, item_isSolo, item_isGroup, item_trackType));
        }


        return new TrackListMessage(trackCount, trackIndex, isNested, parentGroupName, tracks_list);
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
        sb.append("# TrackList\n");
        sb.append("trackList:\n");
        sb.append("  trackCount: ").append(getTrackCount()).append("\n");
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
            sb.append("      isGroup: ").append(item.isGroup() ? "true" : "false").append("\n");
            sb.append("      trackType: ").append(item.getTrackType()).append("\n");
        }
        return sb.toString();
    }
}  // class Message
