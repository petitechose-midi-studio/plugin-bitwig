package protocol.struct;

import protocol.MessageID;

/**
 * RequestDeviceListMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_DEVICE_LIST message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class RequestDeviceListMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REQUEST_DEVICE_LIST;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;


    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RequestDeviceListMessage
     *
     */
    public RequestDeviceListMessage() {
    }

    // ============================================================================
    // Getters
    // ============================================================================

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 0;

    /**
     * Encode message to MIDI-safe bytes (empty message)
     * @return Empty byte array
     */
    public byte[] encode() { return new byte[0]; }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Decode message from MIDI-safe bytes (empty message)
     * @param data Input buffer (unused)
     * @return New RequestDeviceListMessage instance
     */
    public static RequestDeviceListMessage decode(byte[] data) { return new RequestDeviceListMessage(); }

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
        sb.append("# RequestDeviceList\n");
        sb.append("requestDeviceList:\n");
        return sb.toString();
    }
}  // class Message
