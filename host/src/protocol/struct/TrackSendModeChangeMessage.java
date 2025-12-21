package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackSendModeChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_MODE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSendModeChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SEND_MODE_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackSendModeChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final int sendIndex;
    private final String sendMode;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSendModeChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param sendIndex The sendIndex value
     * @param sendMode The sendMode value
     */
    public TrackSendModeChangeMessage(int trackIndex, int sendIndex, String sendMode) {
        this.trackIndex = trackIndex;
        this.sendIndex = sendIndex;
        this.sendMode = sendMode;
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
     * Get the sendMode value
     *
     * @return sendMode
     */
    public String getSendMode() {
        return sendMode;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 55;

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
        byte[] sendIndex_encoded = Encoder.encodeUint8(sendIndex);
        System.arraycopy(sendIndex_encoded, 0, buffer, offset, sendIndex_encoded.length);
        offset += sendIndex_encoded.length;
        byte[] sendMode_encoded = Encoder.encodeString(sendMode, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(sendMode_encoded, 0, buffer, offset, sendMode_encoded.length);
        offset += sendMode_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 23;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSendModeChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSendModeChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSendModeChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int sendIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        String sendMode = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + sendMode.length();

        return new TrackSendModeChangeMessage(trackIndex, sendIndex, sendMode);
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
        sb.append("# TrackSendModeChange\n");
        sb.append("trackSendModeChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  sendIndex: ").append(getSendIndex()).append("\n");
        sb.append("  sendMode: \"").append(getSendMode()).append("\"\n");
        return sb.toString();
    }
}  // class Message
