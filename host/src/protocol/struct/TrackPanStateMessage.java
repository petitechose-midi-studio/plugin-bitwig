package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackPanStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_PAN_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Binary).
 */
public final class TrackPanStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_PAN_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackPanState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int trackIndex;
    private final float pan;
    private final String panDisplay;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackPanStateMessage
     *
     * @param trackIndex The trackIndex value
     * @param pan The pan value
     * @param panDisplay The panDisplay value
     */
    public TrackPanStateMessage(int trackIndex, float pan, String panDisplay) {
        this.trackIndex = trackIndex;
        this.pan = pan;
        this.panDisplay = panDisplay;
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
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 52;

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

        offset += Encoder.encodeUint8(buffer, offset, trackIndex);
        offset += Encoder.encodeFloat32(buffer, offset, pan);
        offset += Encoder.encodeString(buffer, offset, panDisplay);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 20;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackPanStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackPanStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackPanStateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float pan = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String panDisplay = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + panDisplay.length();

        return new TrackPanStateMessage(trackIndex, pan, panDisplay);
    }

}  // class Message
