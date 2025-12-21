package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportClipLauncherOverdubEnabledChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class TransportClipLauncherOverdubEnabledChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "TransportClipLauncherOverdubEnabledChange";


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isClipLauncherOverdubEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportClipLauncherOverdubEnabledChangeMessage
     *
     * @param isClipLauncherOverdubEnabled The isClipLauncherOverdubEnabled value
     */
    public TransportClipLauncherOverdubEnabledChangeMessage(boolean isClipLauncherOverdubEnabled) {
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
    public static final int MAX_PAYLOAD_SIZE = 43;

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

        byte[] isClipLauncherOverdubEnabled_encoded = Encoder.encodeBool(isClipLauncherOverdubEnabled);
        System.arraycopy(isClipLauncherOverdubEnabled_encoded, 0, buffer, offset, isClipLauncherOverdubEnabled_encoded.length);
        offset += isClipLauncherOverdubEnabled_encoded.length;

        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 43;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded TransportClipLauncherOverdubEnabledChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportClipLauncherOverdubEnabledChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportClipLauncherOverdubEnabledChangeMessage decode");
        }

        int offset = 0;

        // Skip message name prefix (length + name bytes)
        int nameLen = data[offset++] & 0xFF;
        offset += nameLen;

        boolean isClipLauncherOverdubEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportClipLauncherOverdubEnabledChangeMessage(isClipLauncherOverdubEnabled);
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
        sb.append("# TransportClipLauncherOverdubEnabledChange\n");
        sb.append("transportClipLauncherOverdubEnabledChange:\n");
        sb.append("  isClipLauncherOverdubEnabled: ").append(isClipLauncherOverdubEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
