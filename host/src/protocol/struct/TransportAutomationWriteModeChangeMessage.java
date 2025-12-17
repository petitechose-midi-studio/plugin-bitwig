package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TransportAutomationWriteModeChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TransportAutomationWriteModeChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final String automationWriteMode;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportAutomationWriteModeChangeMessage
     *
     * @param automationWriteMode The automationWriteMode value
     */
    public TransportAutomationWriteModeChangeMessage(String automationWriteMode) {
        this.automationWriteMode = automationWriteMode;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the automationWriteMode value
     *
     * @return automationWriteMode
     */
    public String getAutomationWriteMode() {
        return automationWriteMode;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 33;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] automationWriteMode_encoded = Encoder.encodeString(automationWriteMode, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(automationWriteMode_encoded, 0, buffer, offset, automationWriteMode_encoded.length);
        offset += automationWriteMode_encoded.length;

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
     * @return Decoded TransportAutomationWriteModeChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportAutomationWriteModeChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportAutomationWriteModeChangeMessage decode");
        }

        int offset = 0;

        String automationWriteMode = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + automationWriteMode.length();

        return new TransportAutomationWriteModeChangeMessage(automationWriteMode);
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
        sb.append("# TransportAutomationWriteModeChange\n");
        sb.append("transportAutomationWriteModeChange:\n");
        sb.append("  automationWriteMode: \"").append(getAutomationWriteMode()).append("\"\n");
        return sb.toString();
    }
}  // class Message
