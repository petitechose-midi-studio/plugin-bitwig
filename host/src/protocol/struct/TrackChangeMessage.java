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
 * All encoding is 7-bit MIDI-safe.
 */
public final class TrackChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final String trackName;
    private final long color;
    private final int trackIndex;
    private final int trackType;

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
     */
    public TrackChangeMessage(String trackName, long color, int trackIndex, int trackType) {
        this.trackName = trackName;
        this.color = color;
        this.trackIndex = trackIndex;
        this.trackType = trackType;
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

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 24;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] trackName_encoded = Encoder.encodeString(trackName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(trackName_encoded, 0, buffer, offset, trackName_encoded.length);
        offset += trackName_encoded.length;
        byte[] color_encoded = Encoder.encodeUint32(color);
        System.arraycopy(color_encoded, 0, buffer, offset, color_encoded.length);
        offset += color_encoded.length;
        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] trackType_encoded = Encoder.encodeUint8(trackType);
        System.arraycopy(trackType_encoded, 0, buffer, offset, trackType_encoded.length);
        offset += trackType_encoded.length;

        return buffer;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 8;

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

        String trackName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + trackName.length();
        long color = Decoder.decodeUint32(data, offset);
        offset += 5;
        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int trackType = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new TrackChangeMessage(trackName, color, trackIndex, trackType);
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
        sb.append("# TrackChange\n");
        sb.append("trackChange:\n");
        sb.append("  trackName: \"").append(getTrackName()).append("\"\n");
        sb.append("  color: ").append(getColor()).append("\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  trackType: ").append(getTrackType()).append("\n");
        return sb.toString();
    }
}  // class Message
