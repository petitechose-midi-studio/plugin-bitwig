package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportPlayMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_PLAY message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TransportPlayMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_PLAY;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isPlaying;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportPlayMessage
     *
     * @param isPlaying The isPlaying value
     */
    public TransportPlayMessage(boolean isPlaying) {
        this.isPlaying = isPlaying;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isPlaying value
     *
     * @return isPlaying
     */
    public boolean isPlaying() {
        return isPlaying;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 1;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] isPlaying_encoded = Encoder.encodeBool(isPlaying);
        System.arraycopy(isPlaying_encoded, 0, buffer, offset, isPlaying_encoded.length);
        offset += isPlaying_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 1;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportPlayMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportPlayMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportPlayMessage decode");
        }

        int offset = 0;

        boolean isPlaying = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportPlayMessage(isPlaying);
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
        sb.append("# TransportPlay\n");
        sb.append("transportPlay:\n");
        sb.append("  isPlaying: ").append(isPlaying() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
