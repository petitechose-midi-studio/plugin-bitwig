package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackSelectMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SELECT message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSelectMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SELECT;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackSelect";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSelectMessage
     *
     * @param trackIndex The trackIndex value
     */
    public TrackSelectMessage(int trackIndex) {
        this.trackIndex = trackIndex;
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

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 13;

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

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 13;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSelectMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSelectMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSelectMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new TrackSelectMessage(trackIndex);
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
        sb.append("# TrackSelect\n");
        sb.append("trackSelect:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
