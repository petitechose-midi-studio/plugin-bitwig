package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * HostDeactivatedMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: HOST_DEACTIVATED message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Binary).
 */
public final class HostDeactivatedMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.HOST_DEACTIVATED;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "HostDeactivated";


    // ============================================================================
    // Fields
    // ============================================================================

    private final boolean isHostActive;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new HostDeactivatedMessage
     *
     * @param isHostActive The isHostActive value
     */
    public HostDeactivatedMessage(boolean isHostActive) {
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
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 17;

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

        offset += Encoder.encodeBool(buffer, offset, isHostActive);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 17;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded HostDeactivatedMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static HostDeactivatedMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for HostDeactivatedMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        boolean isHostActive = Decoder.decodeBool(data, offset);
        offset += 1;

        return new HostDeactivatedMessage(isHostActive);
    }

}  // class Message
