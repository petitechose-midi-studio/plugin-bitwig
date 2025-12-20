package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportArrangerOverdubEnabledChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportArrangerOverdubEnabledChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isArrangerOverdubEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportArrangerOverdubEnabledChangeMessage
     *
     * @param isArrangerOverdubEnabled The isArrangerOverdubEnabled value
     */
    public TransportArrangerOverdubEnabledChangeMessage(boolean isArrangerOverdubEnabled) {
        this.isArrangerOverdubEnabled = isArrangerOverdubEnabled;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isArrangerOverdubEnabled value
     *
     * @return isArrangerOverdubEnabled
     */
    public boolean isArrangerOverdubEnabled() {
        return isArrangerOverdubEnabled;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
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

        byte[] isArrangerOverdubEnabled_encoded = Encoder.encodeBool(isArrangerOverdubEnabled);
        System.arraycopy(isArrangerOverdubEnabled_encoded, 0, buffer, offset, isArrangerOverdubEnabled_encoded.length);
        offset += isArrangerOverdubEnabled_encoded.length;

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
     * @return Decoded TransportArrangerOverdubEnabledChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportArrangerOverdubEnabledChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportArrangerOverdubEnabledChangeMessage decode");
        }

        int offset = 0;

        boolean isArrangerOverdubEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportArrangerOverdubEnabledChangeMessage(isArrangerOverdubEnabled);
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
        sb.append("# TransportArrangerOverdubEnabledChange\n");
        sb.append("transportArrangerOverdubEnabledChange:\n");
        sb.append("  isArrangerOverdubEnabled: ").append(isArrangerOverdubEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
