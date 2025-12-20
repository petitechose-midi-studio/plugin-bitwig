package protocol.struct;

import protocol.MessageID;

/**
 * RequestHostStatusMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_HOST_STATUS message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestHostStatusMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_HOST_STATUS;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;


    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestHostStatusMessage
     *
     */
    public RequestHostStatusMessage() {
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
     * @return New RequestHostStatusMessage instance
     */
    public static RequestHostStatusMessage decode(byte[] data) { return new RequestHostStatusMessage(); }

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
        sb.append("# RequestHostStatus\n");
        sb.append("requestHostStatus:\n");
        return sb.toString();
    }
}  // class Message
