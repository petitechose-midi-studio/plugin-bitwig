package protocol.struct;

import protocol.MessageID;

/**
 * RequestTrackListMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_TRACK_LIST message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class RequestTrackListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_TRACK_LIST;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;


    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestTrackListMessage
     *
     */
    public RequestTrackListMessage() {
    }

    // ============================================================================
    // Getters
    // ============================================================================

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 0;

    /**
     * Encode message to MIDI-safe bytes (empty message)
     * @return Empty byte array
     */
    public byte[] encode() { return new byte[0]; }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Decode message from MIDI-safe bytes (empty message)
     * @param data Input buffer (unused)
     * @return New RequestTrackListMessage instance
     */
    public static RequestTrackListMessage decode(byte[] data) { return new RequestTrackListMessage(); }

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
        sb.append("# RequestTrackList\n");
        sb.append("requestTrackList:\n");
        return sb.toString();
    }
}  // class Message
