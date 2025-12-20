package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackSendEnabledChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_ENABLED_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSendEnabledChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SEND_ENABLED_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final int sendIndex;
    private final boolean sendIsEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSendEnabledChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param sendIndex The sendIndex value
     * @param sendIsEnabled The sendIsEnabled value
     */
    public TrackSendEnabledChangeMessage(int trackIndex, int sendIndex, boolean sendIsEnabled) {
        this.trackIndex = trackIndex;
        this.sendIndex = sendIndex;
        this.sendIsEnabled = sendIsEnabled;
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
     * Get the sendIsEnabled value
     *
     * @return sendIsEnabled
     */
    public boolean getSendIsEnabled() {
        return sendIsEnabled;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 3;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] sendIndex_encoded = Encoder.encodeUint8(sendIndex);
        System.arraycopy(sendIndex_encoded, 0, buffer, offset, sendIndex_encoded.length);
        offset += sendIndex_encoded.length;
        byte[] sendIsEnabled_encoded = Encoder.encodeBool(sendIsEnabled);
        System.arraycopy(sendIsEnabled_encoded, 0, buffer, offset, sendIsEnabled_encoded.length);
        offset += sendIsEnabled_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 3;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSendEnabledChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSendEnabledChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSendEnabledChangeMessage decode");
        }

        int offset = 0;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int sendIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean sendIsEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackSendEnabledChangeMessage(trackIndex, sendIndex, sendIsEnabled);
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
        sb.append("# TrackSendEnabledChange\n");
        sb.append("trackSendEnabledChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  sendIndex: ").append(getSendIndex()).append("\n");
        sb.append("  sendIsEnabled: ").append(getSendIsEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
