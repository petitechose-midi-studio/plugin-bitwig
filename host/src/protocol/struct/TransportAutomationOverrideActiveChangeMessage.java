package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportAutomationOverrideActiveChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TransportAutomationOverrideActiveChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isAutomationOverrideActive;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportAutomationOverrideActiveChangeMessage
     *
     * @param isAutomationOverrideActive The isAutomationOverrideActive value
     */
    public TransportAutomationOverrideActiveChangeMessage(boolean isAutomationOverrideActive) {
        this.isAutomationOverrideActive = isAutomationOverrideActive;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isAutomationOverrideActive value
     *
     * @return isAutomationOverrideActive
     */
    public boolean isAutomationOverrideActive() {
        return isAutomationOverrideActive;
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

        byte[] isAutomationOverrideActive_encoded = Encoder.encodeBool(isAutomationOverrideActive);
        System.arraycopy(isAutomationOverrideActive_encoded, 0, buffer, offset, isAutomationOverrideActive_encoded.length);
        offset += isAutomationOverrideActive_encoded.length;

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
     * @return Decoded TransportAutomationOverrideActiveChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportAutomationOverrideActiveChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportAutomationOverrideActiveChangeMessage decode");
        }

        int offset = 0;

        boolean isAutomationOverrideActive = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportAutomationOverrideActiveChangeMessage(isAutomationOverrideActive);
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
        sb.append("# TransportAutomationOverrideActiveChange\n");
        sb.append("transportAutomationOverrideActiveChange:\n");
        sb.append("  isAutomationOverrideActive: ").append(isAutomationOverrideActive() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
