package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * HostInitializedMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: HOST_INITIALIZED message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class HostInitializedMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.HOST_INITIALIZED;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isHostActive;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new HostInitializedMessage
     *
     * @param isHostActive The isHostActive value
     */
    public HostInitializedMessage(boolean isHostActive) {
        this.isHostActive = isHostActive;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isHostActive value
     *
     * @return isHostActive
     */
    public boolean isHostActive() {
        return isHostActive;
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

        byte[] isHostActive_encoded = Encoder.encodeBool(isHostActive);
        System.arraycopy(isHostActive_encoded, 0, buffer, offset, isHostActive_encoded.length);
        offset += isHostActive_encoded.length;

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
     * @return Decoded HostInitializedMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static HostInitializedMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for HostInitializedMessage decode");
        }

        int offset = 0;

        boolean isHostActive = Decoder.decodeBool(data, offset);
        offset += 1;

        return new HostInitializedMessage(isHostActive);
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
        sb.append("# HostInitialized\n");
        sb.append("hostInitialized:\n");
        sb.append("  isHostActive: ").append(isHostActive() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
