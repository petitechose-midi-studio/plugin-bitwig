package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportTempoMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_TEMPO message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportTempoMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_TEMPO;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportTempo";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final float tempo;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportTempoMessage
     *
     * @param tempo The tempo value
     */
    public TransportTempoMessage(float tempo) {
        this.tempo = tempo;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the tempo value
     *
     * @return tempo
     */
    public float getTempo() {
        return tempo;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 19;

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

        byte[] tempo_encoded = Encoder.encodeFloat32(tempo);
        System.arraycopy(tempo_encoded, 0, buffer, offset, tempo_encoded.length);
        offset += tempo_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 19;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportTempoMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportTempoMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportTempoMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        float tempo = Decoder.decodeFloat32(data, offset);
        offset += 4;

        return new TransportTempoMessage(tempo);
    }

    // ============================================================================
    // Logging
    // ============================================================================
    
    /**
     * Format float with 4 decimal places, handling edge cases.
     * 
     * @param value Float value to format
     * @return Formatted string (e.g., "3.1416", "NaN", "Inf")
     */
    private static String formatFloat(float value) {
        if (Float.isNaN(value)) return "NaN";
        if (Float.isInfinite(value)) return value > 0 ? "Inf" : "-Inf";
        return String.format("%.4f", value);
    }
    
    /**
     * Convert message to YAML format for logging.
     * 
     * @return YAML string representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(256);
        sb.append("# TransportTempo\n");
        sb.append("transportTempo:\n");
        sb.append("  tempo: ").append(formatFloat(getTempo())).append("\n");
        return sb.toString();
    }
}  // class Message
