package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * RequestTrackListWindowMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_TRACK_LIST_WINDOW message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestTrackListWindowMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_TRACK_LIST_WINDOW;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackStartIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestTrackListWindowMessage
     *
     * @param trackStartIndex The trackStartIndex value
     */
    public RequestTrackListWindowMessage(int trackStartIndex) {
        this.trackStartIndex = trackStartIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the trackStartIndex value
     *
     * @return trackStartIndex
     */
    public int getTrackStartIndex() {
        return trackStartIndex;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 1;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] trackStartIndex_encoded = Encoder.encodeUint8(trackStartIndex);
        System.arraycopy(trackStartIndex_encoded, 0, buffer, offset, trackStartIndex_encoded.length);
        offset += trackStartIndex_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 1;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded RequestTrackListWindowMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RequestTrackListWindowMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RequestTrackListWindowMessage decode");
        }

        int offset = 0;

        int trackStartIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new RequestTrackListWindowMessage(trackStartIndex);
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
        sb.append("# RequestTrackListWindow\n");
        sb.append("requestTrackListWindow:\n");
        sb.append("  trackStartIndex: ").append(getTrackStartIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
