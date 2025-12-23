package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackSendValueChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_VALUE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSendValueChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SEND_VALUE_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackSendValueChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final int sendIndex;
    private final float sendValue;
    private final String sendDisplayValue;
    private final boolean isEcho;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSendValueChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param sendIndex The sendIndex value
     * @param sendValue The sendValue value
     * @param sendDisplayValue The sendDisplayValue value
     * @param isEcho The isEcho value
     */
    public TrackSendValueChangeMessage(int trackIndex, int sendIndex, float sendValue, String sendDisplayValue, boolean isEcho) {
        this.trackIndex = trackIndex;
        this.sendIndex = sendIndex;
        this.sendValue = sendValue;
        this.sendDisplayValue = sendDisplayValue;
        this.isEcho = isEcho;
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
     * Get the sendIndex value
     *
     * @return sendIndex
     */
    public int getSendIndex() {
        return sendIndex;
    }

    /**
     * Get the sendValue value
     *
     * @return sendValue
     */
    public float getSendValue() {
        return sendValue;
    }

    /**
     * Get the sendDisplayValue value
     *
     * @return sendDisplayValue
     */
    public String getSendDisplayValue() {
        return sendDisplayValue;
    }

    /**
     * Get the isEcho value
     *
     * @return isEcho
     */
    public boolean isEcho() {
        return isEcho;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 61;

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

        offset += Encoder.writeUint8(buffer, offset, trackIndex);
        offset += Encoder.writeUint8(buffer, offset, sendIndex);
        offset += Encoder.writeFloat32(buffer, offset, sendValue);
        offset += Encoder.writeString(buffer, offset, sendDisplayValue, ProtocolConstants.STRING_MAX_LENGTH);
        offset += Encoder.writeBool(buffer, offset, isEcho);

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 29;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSendValueChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSendValueChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSendValueChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int sendIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float sendValue = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String sendDisplayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + sendDisplayValue.length();
        boolean isEcho = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackSendValueChangeMessage(trackIndex, sendIndex, sendValue, sendDisplayValue, isEcho);
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
        sb.append("# TrackSendValueChange\n");
        sb.append("trackSendValueChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  sendIndex: ").append(getSendIndex()).append("\n");
        sb.append("  sendValue: ").append(formatFloat(getSendValue())).append("\n");
        sb.append("  sendDisplayValue: \"").append(getSendDisplayValue()).append("\"\n");
        sb.append("  isEcho: ").append(isEcho() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
