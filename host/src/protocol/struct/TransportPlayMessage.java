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
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportPlayMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_PLAY;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportPlay";


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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 15;

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
    private static final int MIN_PAYLOAD_SIZE = 15;

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

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

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
