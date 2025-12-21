package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackSendModulatedValueChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_MODULATED_VALUE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackSendModulatedValueChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SEND_MODULATED_VALUE_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackSendModulatedValueChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final int sendIndex;
    private final float modulatedValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSendModulatedValueChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param sendIndex The sendIndex value
     * @param modulatedValue The modulatedValue value
     */
    public TrackSendModulatedValueChangeMessage(int trackIndex, int sendIndex, float modulatedValue) {
        this.trackIndex = trackIndex;
        this.sendIndex = sendIndex;
        this.modulatedValue = modulatedValue;
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
     * Get the modulatedValue value
     *
     * @return modulatedValue
     */
    public float getModulatedValue() {
        return modulatedValue;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 36;

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

        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] sendIndex_encoded = Encoder.encodeUint8(sendIndex);
        System.arraycopy(sendIndex_encoded, 0, buffer, offset, sendIndex_encoded.length);
        offset += sendIndex_encoded.length;
        byte[] modulatedValue_encoded = Encoder.encodeFloat32(modulatedValue);
        System.arraycopy(modulatedValue_encoded, 0, buffer, offset, modulatedValue_encoded.length);
        offset += modulatedValue_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 36;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackSendModulatedValueChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSendModulatedValueChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSendModulatedValueChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        int sendIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float modulatedValue = Decoder.decodeFloat32(data, offset);
        offset += 4;

        return new TrackSendModulatedValueChangeMessage(trackIndex, sendIndex, modulatedValue);
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
        sb.append("# TrackSendModulatedValueChange\n");
        sb.append("trackSendModulatedValueChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  sendIndex: ").append(getSendIndex()).append("\n");
        sb.append("  modulatedValue: ").append(formatFloat(getModulatedValue())).append("\n");
        return sb.toString();
    }
}  // class Message
