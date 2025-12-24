package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import java.util.List;
import java.util.ArrayList;

/**
 * SendDestinationsListMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: SEND_DESTINATIONS_LIST message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class SendDestinationsListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.SEND_DESTINATIONS_LIST;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "SendDestinationsList";

    // ============================================================================
    // Inner Class: SendDestinations
    // ============================================================================

    public static final class SendDestinations {
        private final int sendIndex;
        private final String sendDestinationName;

        public SendDestinations(int sendIndex, String sendDestinationName) {
            this.sendIndex = sendIndex;
            this.sendDestinationName = sendDestinationName;
        }

        public int getSendIndex() {
            return sendIndex;
        }

        public String getSendDestinationName() {
            return sendDestinationName;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int sendCount;
    private final List<SendDestinations> sendDestinations;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new SendDestinationsListMessage
     *
     * @param sendCount The sendCount value
     * @param sendDestinations The sendDestinations value
     */
    public SendDestinationsListMessage(int sendCount, List<SendDestinations> sendDestinations) {
        this.sendCount = sendCount;
        this.sendDestinations = sendDestinations;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the sendCount value
     *
     * @return sendCount
     */
    public int getSendCount() {
        return sendCount;
    }

    /**
     * Get the sendDestinations value
     *
     * @return sendDestinations
     */
    public List<SendDestinations> getSendDestinations() {
        return sendDestinations;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 295;

    /**
     * Encode message directly into provided buffer (zero allocation)
     *
     * @param buffer Output buffer (must have enough space)
     * @param startOffset Starting position in buffer
     * @return Number of bytes written
     */
    public int encode(byte[] buffer, int startOffset) {
        int offset = startOffset;

        // Encode message name (length-prefixed string for bridge logging)
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.writeUint8(buffer, offset, sendCount);
        offset += Encoder.writeUint8(buffer, offset, sendDestinations.size());

        for (SendDestinations item : sendDestinations) {
            offset += Encoder.writeUint8(buffer, offset, item.getSendIndex());
            offset += Encoder.writeString(buffer, offset, item.getSendDestinationName(), ProtocolConstants.STRING_MAX_LENGTH);
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 39;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded SendDestinationsListMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static SendDestinationsListMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for SendDestinationsListMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int sendCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_sendDestinations = Decoder.decodeUint8(data, offset);
        offset += 1;

        List<SendDestinations> sendDestinations_list = new ArrayList<>();
        for (int i = 0; i < count_sendDestinations; i++) {
    int item_sendIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_sendDestinationName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_sendDestinationName.length();
            sendDestinations_list.add(new SendDestinations(item_sendIndex, item_sendDestinationName));
        }


        return new SendDestinationsListMessage(sendCount, sendDestinations_list);
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
        sb.append("# SendDestinationsList\n");
        sb.append("sendDestinationsList:\n");
        sb.append("  sendCount: ").append(getSendCount()).append("\n");
        sb.append("  sendDestinations:\n");
        for (SendDestinations item : getSendDestinations()) {
            sb.append("    - sendIndex: ").append(item.getSendIndex()).append("\n");
            sb.append("      sendDestinationName: \"").append(item.getSendDestinationName()).append("\"\n");
        }
        return sb.toString();
    }
}  // class Message
