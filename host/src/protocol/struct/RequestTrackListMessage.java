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
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestTrackListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_TRACK_LIST;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "RequestTrackList";


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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 17;

    /**
     * Encode message to MIDI-safe bytes (message name only, no fields)
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

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (message name only)
     */
    private static final int MIN_PAYLOAD_SIZE = 17;

    /**
     * Decode message from MIDI-safe bytes (message name only, no fields)
     * @param data Input buffer
     * @return New RequestTrackListMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RequestTrackListMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RequestTrackListMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        return new RequestTrackListMessage();
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
        sb.append("# RequestTrackList\n");
        sb.append("requestTrackList:\n");
        return sb.toString();
    }
}  // class Message
