package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportAutomationWriteModeStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_AUTOMATION_WRITE_MODE_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportAutomationWriteModeStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_AUTOMATION_WRITE_MODE_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportAutomationWriteModeState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int automationWriteMode;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportAutomationWriteModeStateMessage
     *
     * @param automationWriteMode The automationWriteMode value
     */
    public TransportAutomationWriteModeStateMessage(int automationWriteMode) {
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
    public int getAutomationWriteMode() {
        return automationWriteMode;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 35;

    /**
     * Encode message directly into provided buffer (zero allocation)
     *
     * @param buffer Output buffer (must have enough space)
     * @param startOffset Starting position in buffer
     * @return Number of bytes written
     */
    public int encode(byte[] buffer, int startOffset) {
        int offset = startOffset;

        // Encode MESSAGE_NAME prefix
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.encodeUint8(buffer, offset, automationWriteMode);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 35;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportAutomationWriteModeStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportAutomationWriteModeStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportAutomationWriteModeStateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int automationWriteMode = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new TransportAutomationWriteModeStateMessage(automationWriteMode);
    }

}  // class Message
