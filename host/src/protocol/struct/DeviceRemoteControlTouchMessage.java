package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;

/**
 * DeviceRemoteControlTouchMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_TOUCH message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceRemoteControlTouchMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROL_TOUCH;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int remoteControlIndex;
    private final boolean isTouched;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlTouchMessage
     *
     * @param remoteControlIndex The remoteControlIndex value
     * @param isTouched The isTouched value
     */
    public DeviceRemoteControlTouchMessage(int remoteControlIndex, boolean isTouched) {
        this.remoteControlIndex = remoteControlIndex;
        this.isTouched = isTouched;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the remoteControlIndex value
     *
     * @return remoteControlIndex
     */
    public int getRemoteControlIndex() {
        return remoteControlIndex;
    }

    /**
     * Get the isTouched value
     *
     * @return isTouched
     */
    public boolean isTouched() {
        return isTouched;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
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

        byte[] remoteControlIndex_encoded = Encoder.encodeUint8(remoteControlIndex);
        System.arraycopy(remoteControlIndex_encoded, 0, buffer, offset, remoteControlIndex_encoded.length);
        offset += remoteControlIndex_encoded.length;
        byte[] isTouched_encoded = Encoder.encodeBool(isTouched);
        System.arraycopy(isTouched_encoded, 0, buffer, offset, isTouched_encoded.length);
        offset += isTouched_encoded.length;

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
     * @return Decoded DeviceRemoteControlTouchMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlTouchMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlTouchMessage decode");
        }

        int offset = 0;

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isTouched = Decoder.decodeBool(data, offset);
        offset += 1;

        return new DeviceRemoteControlTouchMessage(remoteControlIndex, isTouched);
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
        sb.append("# DeviceRemoteControlTouch\n");
        sb.append("deviceRemoteControlTouch:\n");
        sb.append("  remoteControlIndex: ").append(getRemoteControlIndex()).append("\n");
        sb.append("  isTouched: ").append(isTouched() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
