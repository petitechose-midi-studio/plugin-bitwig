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
 * All encoding is 8-bit binary (Serial8).
 */
public final class ViewStateChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.VIEW_STATE_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "ViewStateChange";


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
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 18;

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

        offset += Encoder.writeUint8(buffer, offset, viewType);
        offset += Encoder.writeBool(buffer, offset, selectorActive);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 18;

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

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

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
