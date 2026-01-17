package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;

/**
 * RemoteControlValueStateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REMOTE_CONTROL_VALUE_STATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Binary).
 */
public final class RemoteControlValueStateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.REMOTE_CONTROL_VALUE_STATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "RemoteControlValueState";


    // ============================================================================
    // Fields
    // ============================================================================

    private final int remoteControlIndex;
    private final float parameterValue;
    private final String displayValue;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new RemoteControlValueStateMessage
     *
     * @param remoteControlIndex The remoteControlIndex value
     * @param parameterValue The parameterValue value
     * @param displayValue The displayValue value
     */
    public RemoteControlValueStateMessage(int remoteControlIndex, float parameterValue, String displayValue) {
        this.remoteControlIndex = remoteControlIndex;
        this.parameterValue = parameterValue;
        this.displayValue = displayValue;
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
     * Get the parameterValue value
     *
     * @return parameterValue
     */
    public float getParameterValue() {
        return parameterValue;
    }

    /**
     * Get the displayValue value
     *
     * @return displayValue
     */
    public String getDisplayValue() {
        return displayValue;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 62;

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

        offset += Encoder.encodeUint8(buffer, offset, remoteControlIndex);
        offset += Encoder.encodeFloat32(buffer, offset, parameterValue);
        offset += Encoder.encodeString(buffer, offset, displayValue);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 30;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded RemoteControlValueStateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static RemoteControlValueStateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for RemoteControlValueStateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        int remoteControlIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        float parameterValue = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String displayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + displayValue.length();

        return new RemoteControlValueStateMessage(remoteControlIndex, parameterValue, displayValue);
    }

}  // class Message
