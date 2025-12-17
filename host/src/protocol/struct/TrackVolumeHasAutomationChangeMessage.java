package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TrackVolumeHasAutomationChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_VOLUME_HAS_AUTOMATION_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TrackVolumeHasAutomationChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_VOLUME_HAS_AUTOMATION_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final boolean hasAutomation;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackVolumeHasAutomationChangeMessage
     *
     * @param trackIndex The trackIndex value
     * @param hasAutomation The hasAutomation value
     */
    public TrackVolumeHasAutomationChangeMessage(int trackIndex, boolean hasAutomation) {
        this.trackIndex = trackIndex;
        this.hasAutomation = hasAutomation;
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
     * Get the hasAutomation value
     *
     * @return hasAutomation
     */
    public boolean getHasAutomation() {
        return hasAutomation;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 2;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] hasAutomation_encoded = Encoder.encodeBool(hasAutomation);
        System.arraycopy(hasAutomation_encoded, 0, buffer, offset, hasAutomation_encoded.length);
        offset += hasAutomation_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 2;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TrackVolumeHasAutomationChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackVolumeHasAutomationChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackVolumeHasAutomationChangeMessage decode");
        }

        int offset = 0;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean hasAutomation = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackVolumeHasAutomationChangeMessage(trackIndex, hasAutomation);
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
        sb.append("# TrackVolumeHasAutomationChange\n");
        sb.append("trackVolumeHasAutomationChange:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  hasAutomation: ").append(getHasAutomation() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
