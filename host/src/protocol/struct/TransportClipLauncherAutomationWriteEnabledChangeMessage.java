package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * TransportClipLauncherAutomationWriteEnabledChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TransportClipLauncherAutomationWriteEnabledChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final boolean isClipLauncherAutomationWriteEnabled;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TransportClipLauncherAutomationWriteEnabledChangeMessage
     *
     * @param isClipLauncherAutomationWriteEnabled The isClipLauncherAutomationWriteEnabled value
     */
    public TransportClipLauncherAutomationWriteEnabledChangeMessage(boolean isClipLauncherAutomationWriteEnabled) {
        this.isClipLauncherAutomationWriteEnabled = isClipLauncherAutomationWriteEnabled;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the isClipLauncherAutomationWriteEnabled value
     *
     * @return isClipLauncherAutomationWriteEnabled
     */
    public boolean isClipLauncherAutomationWriteEnabled() {
        return isClipLauncherAutomationWriteEnabled;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
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

        byte[] isClipLauncherAutomationWriteEnabled_encoded = Encoder.encodeBool(isClipLauncherAutomationWriteEnabled);
        System.arraycopy(isClipLauncherAutomationWriteEnabled_encoded, 0, buffer, offset, isClipLauncherAutomationWriteEnabled_encoded.length);
        offset += isClipLauncherAutomationWriteEnabled_encoded.length;

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
     * @return Decoded TransportClipLauncherAutomationWriteEnabledChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TransportClipLauncherAutomationWriteEnabledChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TransportClipLauncherAutomationWriteEnabledChangeMessage decode");
        }

        int offset = 0;

        boolean isClipLauncherAutomationWriteEnabled = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TransportClipLauncherAutomationWriteEnabledChangeMessage(isClipLauncherAutomationWriteEnabled);
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
        sb.append("# TransportClipLauncherAutomationWriteEnabledChange\n");
        sb.append("transportClipLauncherAutomationWriteEnabledChange:\n");
        sb.append("  isClipLauncherAutomationWriteEnabled: ").append(isClipLauncherAutomationWriteEnabled() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
