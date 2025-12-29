package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackSendListMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_LIST message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSendListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SEND_LIST;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackSendList";

    // ============================================================================
    // Inner Class: Sends
    // ============================================================================

    public static final class Sends {
        private final int sendIndex;
        private final String sendName;
        private final long color;
        private final float sendValue;
        private final String sendDisplayValue;
        private final boolean sendIsEnabled;
        private final String sendMode;
        private final boolean sendIsPreFader;

        public Sends(int sendIndex, String sendName, long color, float sendValue, String sendDisplayValue, boolean sendIsEnabled, String sendMode, boolean sendIsPreFader) {
            this.sendIndex = sendIndex;
            this.sendName = sendName;
            this.color = color;
            this.sendValue = sendValue;
            this.sendDisplayValue = sendDisplayValue;
            this.sendIsEnabled = sendIsEnabled;
            this.sendMode = sendMode;
            this.sendIsPreFader = sendIsPreFader;
        }

        public int getSendIndex() {
            return sendIndex;
        }

        public String getSendName() {
            return sendName;
        }

        public long getColor() {
            return color;
        }

        public float getSendValue() {
            return sendValue;
        }

        public String getSendDisplayValue() {
            return sendDisplayValue;
        }

        public boolean getSendIsEnabled() {
            return sendIsEnabled;
        }

        public String getSendMode() {
            return sendMode;
        }

        public boolean getSendIsPreFader() {
            return sendIsPreFader;
        }

    }

    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final int sendCount;
    private final Sends[] sends;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSendListMessage
     *
     * @param trackIndex The trackIndex value
     * @param sendCount The sendCount value
     * @param sends The sends value
     */
    public TrackSendListMessage(int trackIndex, int sendCount, Sends[] sends) {
        this.trackIndex = trackIndex;
        this.sendCount = sendCount;
        this.sends = sends;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the trackIndex value
     *
     * @return trackIndex
     */
    public int getTrackIndex() {
        return trackIndex;
    }

    /**
     * Get the sendCount value
     *
     * @return sendCount
     */
    public int getSendCount() {
        return sendCount;
    }

    /**
     * Get the sends value
     *
     * @return sends
     */
    public Sends[] getSends() {
        return sends;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 897;

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

        offset += Encoder.writeUint8(buffer, offset, trackIndex);
        offset += Encoder.writeUint8(buffer, offset, sendCount);
        offset += Encoder.writeUint8(buffer, offset, sends.length);

        for (Sends item : sends) {
            offset += Encoder.writeUint8(buffer, offset, item.getSendIndex());
            offset += Encoder.writeString(buffer, offset, item.getSendName(), ProtocolConstants.STRING_MAX_LENGTH);
            offset += Encoder.writeUint32(buffer, offset, item.getColor());
            offset += Encoder.writeFloat32(buffer, offset, item.getSendValue());
            offset += Encoder.writeString(buffer, offset, item.getSendDisplayValue(), ProtocolConstants.STRING_MAX_LENGTH);
            offset += Encoder.writeBool(buffer, offset, item.getSendIsEnabled());
            offset += Encoder.writeString(buffer, offset, item.getSendMode(), ProtocolConstants.STRING_MAX_LENGTH);
            offset += Encoder.writeBool(buffer, offset, item.getSendIsPreFader());
        }


        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 129;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSendListMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSendListMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSendListMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int sendCount = Decoder.decodeUint8(data, offset);
        offset += 1;
        int count_sends = Decoder.decodeUint8(data, offset);
        offset += 1;

        Sends[] sends = new Sends[count_sends];
        for (int i = 0; i < count_sends; i++) {
    int item_sendIndex = Decoder.decodeUint8(data, offset);
            offset += 1;
    String item_sendName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_sendName.length();
    long item_color = Decoder.decodeUint32(data, offset);
            offset += 4;
    float item_sendValue = Decoder.decodeFloat32(data, offset);
            offset += 4;
    String item_sendDisplayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_sendDisplayValue.length();
    boolean item_sendIsEnabled = Decoder.decodeBool(data, offset);
            offset += 1;
    String item_sendMode = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
            offset += 1 + item_sendMode.length();
    boolean item_sendIsPreFader = Decoder.decodeBool(data, offset);
            offset += 1;
            sends[i] = new Sends(item_sendIndex, item_sendName, item_color, item_sendValue, item_sendDisplayValue, item_sendIsEnabled, item_sendMode, item_sendIsPreFader);
        }


        return new TrackSendListMessage(trackIndex, sendCount, sends);
    }

    // ============================================================================
    // Logging
    // ============================================================================
    
    /**
     * Format float with 4 decimal places, handling edge cases.
     * 
     * @param value Float value to format
     * @return Formatted string (e.g., "3.1416", "NaN", "Inf")
     */
    private static String formatFloat(float value) {
        if (Float.isNaN(value)) return "NaN";
        if (Float.isInfinite(value)) return value > 0 ? "Inf" : "-Inf";
        return String.format("%.4f", value);
    }
    
    /**
     * Convert message to YAML format for logging.
     * 
     * @return YAML string representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(256);
        sb.append("# TrackSendList\n");
        sb.append("trackSendList:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  sendCount: ").append(getSendCount()).append("\n");
        sb.append("  sends:\n");
        for (Sends item : getSends()) {
            sb.append("    - sendIndex: ").append(item.getSendIndex()).append("\n");
            sb.append("      sendName: \"").append(item.getSendName()).append("\"\n");
            sb.append("      color: ").append(item.getColor()).append("\n");
            sb.append("      sendValue: ").append(formatFloat(item.getSendValue())).append("\n");
            sb.append("      sendDisplayValue: \"").append(item.getSendDisplayValue()).append("\"\n");
            sb.append("      sendIsEnabled: ").append(item.getSendIsEnabled() ? "true" : "false").append("\n");
            sb.append("      sendMode: \"").append(item.getSendMode()).append("\"\n");
            sb.append("      sendIsPreFader: ").append(item.getSendIsPreFader() ? "true" : "false").append("\n");
        }
        return sb.toString();
    }
}  // class Message
