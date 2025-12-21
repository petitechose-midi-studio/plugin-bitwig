package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * TrackPanChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_PAN_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TrackPanChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_PAN_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TrackPanChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final float pan;
    private final String panDisplay;
    private final boolean isEcho;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackPanChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param pan The pan value
     * @param panDisplay The panDisplay value
     * @param isEcho The isEcho value
     */
    public TrackPanChangeMessage(int trackIndex, float pan, String panDisplay, boolean isEcho) {
        this.trackIndex = trackIndex;
        this.pan = pan;
        this.panDisplay = panDisplay;
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
     * Get the pan value
     *
     * @return pan
     */
    public float getPan() {
        return pan;
    }

    /**
     * Get the panDisplay value
     *
     * @return panDisplay
     */
    public String getPanDisplay() {
        return panDisplay;
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
    public static final int MAX_PAYLOAD_SIZE = 54;

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
        byte[] pan_encoded = Encoder.encodeFloat32(pan);
        System.arraycopy(pan_encoded, 0, buffer, offset, pan_encoded.length);
        offset += pan_encoded.length;
        byte[] panDisplay_encoded = Encoder.encodeString(panDisplay, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(panDisplay_encoded, 0, buffer, offset, panDisplay_encoded.length);
        offset += panDisplay_encoded.length;
        byte[] isEcho_encoded = Encoder.encodeBool(isEcho);
        System.arraycopy(isEcho_encoded, 0, buffer, offset, isEcho_encoded.length);
        offset += isEcho_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 22;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackPanChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackPanChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackPanChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float pan = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String panDisplay = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + panDisplay.length();
        boolean isEcho = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackPanChangeMessage(trackIndex, pan, panDisplay, isEcho);
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
        sb.append("# TrackPanChange\n");
        sb.append("trackPanChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  pan: ").append(formatFloat(getPan())).append("\n");
        sb.append("  panDisplay: \"").append(getPanDisplay()).append("\"\n");
        sb.append("  isEcho: ").append(isEcho() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
