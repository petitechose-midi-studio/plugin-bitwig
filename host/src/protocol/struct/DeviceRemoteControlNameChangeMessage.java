package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * DeviceRemoteControlNameChangeMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_NAME_CHANGE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class DeviceRemoteControlNameChangeMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROL_NAME_CHANGE;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int remoteControlIndex;
    private final String parameterName;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlNameChangeMessage
     *
     * @param remoteControlIndex The remoteControlIndex value
     * @param parameterName The parameterName value
     */
    public DeviceRemoteControlNameChangeMessage(int remoteControlIndex, String parameterName) {
        this.remoteControlIndex = remoteControlIndex;
        this.parameterName = parameterName;
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
     * Get the parameterName value
     *
     * @return parameterName
     */
    public String getParameterName() {
        return parameterName;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 34;

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
        byte[] parameterName_encoded = Encoder.encodeString(parameterName, ProtocolConstants.STRING_MAX_LENGTH);
        System.arraycopy(parameterName_encoded, 0, buffer, offset, parameterName_encoded.length);
        offset += parameterName_encoded.length;

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
     * @return Decoded DeviceRemoteControlNameChangeMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlNameChangeMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlNameChangeMessage decode");
        }

        int offset = 0;

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        String parameterName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parameterName.length();

        return new DeviceRemoteControlNameChangeMessage(remoteControlIndex, parameterName);
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
        sb.append("# DeviceRemoteControlNameChange\n");
        sb.append("deviceRemoteControlNameChange:\n");
        sb.append("  remoteControlIndex: ").append(getRemoteControlIndex()).append("\n");
        sb.append("  parameterName: \"").append(getParameterName()).append("\"\n");
        return sb.toString();
    }
}  // class Message
