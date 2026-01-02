package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportArrangerAutomationWriteEnabledMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportArrangerAutomationWriteEnabledMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportArrangerAutomationWriteEnabled";


    // ============================================================================
    // Fields
    // ============================================================================

    private final boolean isArrangerAutomationWriteEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportArrangerAutomationWriteEnabledMessage
     *
     * @param isArrangerAutomationWriteEnabled The isArrangerAutomationWriteEnabled value
     */
    public TransportArrangerAutomationWriteEnabledMessage(boolean isArrangerAutomationWriteEnabled) {
        this.isArrangerAutomationWriteEnabled = isArrangerAutomationWriteEnabled;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isArrangerAutomationWriteEnabled value
     *
     * @return isArrangerAutomationWriteEnabled
     */
    public boolean isArrangerAutomationWriteEnabled() {
        return isArrangerAutomationWriteEnabled;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 41;

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

        offset += Encoder.encodeBool(buffer, offset, isArrangerAutomationWriteEnabled);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 41;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportArrangerAutomationWriteEnabledMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportArrangerAutomationWriteEnabledMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportArrangerAutomationWriteEnabledMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        boolean isArrangerAutomationWriteEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportArrangerAutomationWriteEnabledMessage(isArrangerAutomationWriteEnabled);
    }

}  // class Message
