package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackSendHasAutomationStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_HAS_AUTOMATION_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSendHasAutomationStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SEND_HAS_AUTOMATION_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackSendHasAutomationState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int trackIndex;
    private final int sendIndex;
    private final boolean hasAutomation;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSendHasAutomationStateMessage
     *
     * @param trackIndex The trackIndex value
     * @param sendIndex The sendIndex value
     * @param hasAutomation The hasAutomation value
     */
    public TrackSendHasAutomationStateMessage(int trackIndex, int sendIndex, boolean hasAutomation) {
        this.trackIndex = trackIndex;
        this.sendIndex = sendIndex;
        this.hasAutomation = hasAutomation;
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
     * Get the sendIndex value
     *
     * @return sendIndex
     */
    public int getSendIndex() {
        return sendIndex;
    }

    /**
     * Get the hasAutomation value
     *
     * @return hasAutomation
     */
    public boolean getHasAutomation() {
        return hasAutomation;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 31;

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
        offset += Encoder.encodeUint8(buffer, offset, sendIndex);
        offset += Encoder.encodeBool(buffer, offset, hasAutomation);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 31;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSendHasAutomationStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSendHasAutomationStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSendHasAutomationStateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int sendIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean hasAutomation = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackSendHasAutomationStateMessage(trackIndex, sendIndex, hasAutomation);
    }

}  // class Message
