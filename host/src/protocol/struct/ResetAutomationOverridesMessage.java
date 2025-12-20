package protocol.struct;

import protocol.MessageID;

/**
 * ResetAutomationOverridesMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: RESET_AUTOMATION_OVERRIDES message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class ResetAutomationOverridesMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.RESET_AUTOMATION_OVERRIDES;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;


    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new ResetAutomationOverridesMessage
     *
     */
    public ResetAutomationOverridesMessage() {
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
     * @return New ResetAutomationOverridesMessage instance
     */
    public static ResetAutomationOverridesMessage decode(byte[] data) { return new ResetAutomationOverridesMessage(); }

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
        sb.append("# ResetAutomationOverrides\n");
        sb.append("resetAutomationOverrides:\n");
        return sb.toString();
    }
}  // class Message
