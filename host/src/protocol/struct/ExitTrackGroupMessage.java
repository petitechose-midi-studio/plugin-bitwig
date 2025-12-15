package protocol.struct;

import protocol.MessageID;

/**
 * ExitTrackGroupMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: EXIT_TRACK_GROUP message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class ExitTrackGroupMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.EXIT_TRACK_GROUP;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;


    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new ExitTrackGroupMessage
     *
     */
    public ExitTrackGroupMessage() {
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
     * @return New ExitTrackGroupMessage instance
     */
    public static ExitTrackGroupMessage decode(byte[] data) { return new ExitTrackGroupMessage(); }

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
        sb.append("# ExitTrackGroup\n");
        sb.append("exitTrackGroup:\n");
        return sb.toString();
    }
}  // class Message
