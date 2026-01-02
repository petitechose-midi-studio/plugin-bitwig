package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ViewType;

/**
 * ViewStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: VIEW_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class ViewStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.VIEW_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "ViewState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final ViewType viewType;
    private final boolean selectorActive;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new ViewStateMessage
     *
     * @param viewType The viewType value
     * @param selectorActive The selectorActive value
     */
    public ViewStateMessage(ViewType viewType, boolean selectorActive) {
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
    public ViewType getViewType() {
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
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 12;

    /**
     * Encode message directly into provided buffer (zero allocation)
     *
     * @param buffer Output buffer (must have enough space)
     * @param startOffset Starting position in buffer
     * @return Number of bytes written
     */
    public int encode(byte[] buffer, int startOffset) {
        int offset = startOffset;

        // Encode MESSAGE_NAME prefix
        buffer[offset++] = (byte) MESSAGE_NAME.length();
        for (int i = 0; i < MESSAGE_NAME.length(); i++) {
            buffer[offset++] = (byte) MESSAGE_NAME.charAt(i);
        }

        offset += Encoder.encodeUint8(buffer, offset, viewType.getValue());
        offset += Encoder.encodeBool(buffer, offset, selectorActive);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 12;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded ViewStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static ViewStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for ViewStateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        ViewType viewType = ViewType.fromValue(Decoder.decodeUint8(data, offset));
        offset += 1;

        boolean selectorActive = Decoder.decodeBool(data, offset);
        offset += 1;

        return new ViewStateMessage(viewType, selectorActive);
    }

}  // class Message
