package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackMutedBySoloChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_MUTED_BY_SOLO_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackMutedBySoloChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_MUTED_BY_SOLO_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackMutedBySoloChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final boolean isMutedBySolo;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackMutedBySoloChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param isMutedBySolo The isMutedBySolo value
     */
    public TrackMutedBySoloChangeMessage(int trackIndex, boolean isMutedBySolo) {
        this.trackIndex = trackIndex;
        this.isMutedBySolo = isMutedBySolo;
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
     * Get the isMutedBySolo value
     *
     * @return isMutedBySolo
     */
    public boolean isMutedBySolo() {
        return isMutedBySolo;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 25;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        // Encode message name (length-prefixed string for bridge logging)
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] isMutedBySolo_encoded = Encoder.encodeBool(isMutedBySolo);
        System.arraycopy(isMutedBySolo_encoded, 0, buffer, offset, isMutedBySolo_encoded.length);
        offset += isMutedBySolo_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
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
     * @return Decoded TrackMutedBySoloChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackMutedBySoloChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackMutedBySoloChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isMutedBySolo = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackMutedBySoloChangeMessage(trackIndex, isMutedBySolo);
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
        sb.append("# TrackMutedBySoloChange\n");
        sb.append("trackMutedBySoloChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  isMutedBySolo: ").append(isMutedBySolo() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
