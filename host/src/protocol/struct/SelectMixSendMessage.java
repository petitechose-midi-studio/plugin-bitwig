package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * SelectMixSendMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: SELECT_MIX_SEND message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class SelectMixSendMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.SELECT_MIX_SEND;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "SelectMixSend";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int sendIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new SelectMixSendMessage
     *
     * @param sendIndex The sendIndex value
     */
    public SelectMixSendMessage(int sendIndex) {
        this.sendIndex = sendIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the sendIndex value
     *
     * @return sendIndex
     */
    public int getSendIndex() {
        return sendIndex;
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

        byte[] sendIndex_encoded = Encoder.encodeUint8(sendIndex);
        System.arraycopy(sendIndex_encoded, 0, buffer, offset, sendIndex_encoded.length);
        offset += sendIndex_encoded.length;

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
     * @return Decoded SelectMixSendMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static SelectMixSendMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for SelectMixSendMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int sendIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new SelectMixSendMessage(sendIndex);
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
        sb.append("# SelectMixSend\n");
        sb.append("selectMixSend:\n");
        sb.append("  sendIndex: ").append(getSendIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
