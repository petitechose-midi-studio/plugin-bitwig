package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * ViewStateChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: VIEW_STATE_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class ViewStateChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.VIEW_STATE_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int viewType;
    private final boolean selectorActive;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new ViewStateChangeMessage
     *
     * @param viewType The viewType value
     * @param selectorActive The selectorActive value
     */
    public ViewStateChangeMessage(int viewType, boolean selectorActive) {
        this.viewType = viewType;
        this.selectorActive = selectorActive;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the viewType value
     *
     * @return viewType
     */
    public int getViewType() {
        return viewType;
    }

    /**
     * Get the selectorActive value
     *
     * @return selectorActive
     */
    public boolean getSelectorActive() {
        return selectorActive;
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

        byte[] viewType_encoded = Encoder.encodeUint8(viewType);
        System.arraycopy(viewType_encoded, 0, buffer, offset, viewType_encoded.length);
        offset += viewType_encoded.length;
        byte[] selectorActive_encoded = Encoder.encodeBool(selectorActive);
        System.arraycopy(selectorActive_encoded, 0, buffer, offset, selectorActive_encoded.length);
        offset += selectorActive_encoded.length;

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
     * @return Decoded ViewStateChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static ViewStateChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for ViewStateChangeMessage decode");
        }

        int offset = 0;

        int viewType = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean selectorActive = Decoder.decodeBool(data, offset);
        offset += 1;

        return new ViewStateChangeMessage(viewType, selectorActive);
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
        sb.append("# ViewStateChange\n");
        sb.append("viewStateChange:\n");
        sb.append("  viewType: ").append(getViewType()).append("\n");
        sb.append("  selectorActive: ").append(getSelectorActive() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
