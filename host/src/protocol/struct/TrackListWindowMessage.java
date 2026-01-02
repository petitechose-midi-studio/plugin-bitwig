package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import protocol.TrackType;

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
        private final TrackType trackType;
        private final float volume;
        private final float pan;

        public Tracks(int trackIndex, String trackName, long color, boolean isActivated, boolean isMute, boolean isSolo, boolean isMutedBySolo, boolean isArm, boolean isGroup, TrackType trackType, float volume, float pan) {
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

        public TrackType getTrackType() {
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
     * Maximum payload size in bytes (8-bit binary)
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

        // Encode MESSAGE_NAME prefix
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.encodeUint8(buffer, offset, trackCount);
        offset += Encoder.encodeUint8(buffer, offset, trackStartIndex);
        offset += Encoder.encodeUint8(buffer, offset, trackIndex);
        offset += Encoder.encodeBool(buffer, offset, isNested);
        offset += Encoder.encodeString(buffer, offset, parentGroupName);
        offset += Encoder.encodeUint8(buffer, offset, tracks.length);

        for (Tracks item : tracks) {
            offset += Encoder.encodeUint8(buffer, offset, item.getTrackIndex());
            offset += Encoder.encodeString(buffer, offset, item.getTrackName());
            offset += Encoder.encodeUint32(buffer, offset, item.getColor());
            offset += Encoder.encodeBool(buffer, offset, item.isActivated());
            offset += Encoder.encodeBool(buffer, offset, item.isMute());
            offset += Encoder.encodeBool(buffer, offset, item.isSolo());
            offset += Encoder.encodeBool(buffer, offset, item.isMutedBySolo());
            offset += Encoder.encodeBool(buffer, offset, item.isArm());
            offset += Encoder.encodeBool(buffer, offset, item.isGroup());
            offset += Encoder.encodeUint8(buffer, offset, item.getTrackType().getValue());
            offset += Encoder.encodeFloat32(buffer, offset, item.getVolume());
            offset += Encoder.encodeFloat32(buffer, offset, item.getPan());
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 22;

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

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

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
            TrackType item_trackType = TrackType.fromValue(Decoder.decodeUint8(data, offset));
            offset += 1;
    float item_volume = Decoder.decodeFloat32(data, offset);
            offset += 4;
    float item_pan = Decoder.decodeFloat32(data, offset);
            offset += 4;
            tracks[i] = new Tracks(item_trackIndex, item_trackName, item_color, item_isActivated, item_isMute, item_isSolo, item_isMutedBySolo, item_isArm, item_isGroup, item_trackType, item_volume, item_pan);
        }


        return new TrackListWindowMessage(trackCount, trackStartIndex, trackIndex, isNested, parentGroupName, tracks);
    }

}  // class Message
