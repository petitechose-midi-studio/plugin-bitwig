package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackMutedBySoloStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_MUTED_BY_SOLO_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackMutedBySoloStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_MUTED_BY_SOLO_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackMutedBySoloState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int trackIndex;
    private final boolean isMutedBySolo;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackMutedBySoloStateMessage
     *
     * @param trackIndex The trackIndex value
     * @param isMutedBySolo The isMutedBySolo value
     */
    public TrackMutedBySoloStateMessage(int trackIndex, boolean isMutedBySolo) {
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
    public static final int MAX_PAYLOAD_SIZE = 24;

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
        offset += Encoder.writeBool(buffer, offset, isMutedBySolo);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 24;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackMutedBySoloStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackMutedBySoloStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackMutedBySoloStateMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isMutedBySolo = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackMutedBySoloStateMessage(trackIndex, isMutedBySolo);
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
        sb.append("# TrackMutedBySoloState\n");
        sb.append("trackMutedBySoloState:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  isMutedBySolo: ").append(isMutedBySolo() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
