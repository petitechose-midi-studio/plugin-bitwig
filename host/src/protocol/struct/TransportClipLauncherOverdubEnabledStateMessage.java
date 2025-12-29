package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportClipLauncherOverdubEnabledStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportClipLauncherOverdubEnabledStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportClipLauncherOverdubEnabledState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final boolean isClipLauncherOverdubEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportClipLauncherOverdubEnabledStateMessage
     *
     * @param isClipLauncherOverdubEnabled The isClipLauncherOverdubEnabled value
     */
    public TransportClipLauncherOverdubEnabledStateMessage(boolean isClipLauncherOverdubEnabled) {
        this.isClipLauncherOverdubEnabled = isClipLauncherOverdubEnabled;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isClipLauncherOverdubEnabled value
     *
     * @return isClipLauncherOverdubEnabled
     */
    public boolean isClipLauncherOverdubEnabled() {
        return isClipLauncherOverdubEnabled;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 42;

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

        offset += Encoder.writeBool(buffer, offset, isClipLauncherOverdubEnabled);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 42;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportClipLauncherOverdubEnabledStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportClipLauncherOverdubEnabledStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportClipLauncherOverdubEnabledStateMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        boolean isClipLauncherOverdubEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportClipLauncherOverdubEnabledStateMessage(isClipLauncherOverdubEnabled);
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
        sb.append("# TransportClipLauncherOverdubEnabledState\n");
        sb.append("transportClipLauncherOverdubEnabledState:\n");
        sb.append("  isClipLauncherOverdubEnabled: ").append(isClipLauncherOverdubEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
