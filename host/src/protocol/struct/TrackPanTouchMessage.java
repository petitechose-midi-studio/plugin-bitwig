package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackPanTouchMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_PAN_TOUCH message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackPanTouchMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_PAN_TOUCH;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackPanTouch";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int trackIndex;
    private final boolean isTouched;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackPanTouchMessage
     *
     * @param trackIndex The trackIndex value
     * @param isTouched The isTouched value
     */
    public TrackPanTouchMessage(int trackIndex, boolean isTouched) {
        this.trackIndex = trackIndex;
        this.isTouched = isTouched;
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
     * Get the isTouched value
     *
     * @return isTouched
     */
    public boolean isTouched() {
        return isTouched;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 16;

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
        offset += Encoder.encodeBool(buffer, offset, isTouched);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 16;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackPanTouchMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackPanTouchMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackPanTouchMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isTouched = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackPanTouchMessage(trackIndex, isTouched);
    }

}  // class Message
