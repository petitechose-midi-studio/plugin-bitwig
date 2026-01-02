package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import protocol.ProtocolConstants;
import protocol.ParameterType;

/**
 * LastClickedUpdateMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: LAST_CLICKED_UPDATE message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 8-bit binary (Serial8).
 */
public final class LastClickedUpdateMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.LAST_CLICKED_UPDATE;

    // Message name for logging (encoded in payload)
    public static final String MESSAGE_NAME = "LastClickedUpdate";


    // ============================================================================
    // Fields
    // ============================================================================

    private final String parameterName;
    private final float parameterValue;
    private final String displayValue;
    private final float parameterOrigin;
    private final boolean parameterExists;
    private final ParameterType parameterType;
    private final short discreteValueCount;
    private final int currentValueIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new LastClickedUpdateMessage
     *
     * @param parameterName The parameterName value
     * @param parameterValue The parameterValue value
     * @param displayValue The displayValue value
     * @param parameterOrigin The parameterOrigin value
     * @param parameterExists The parameterExists value
     * @param parameterType The parameterType value
     * @param discreteValueCount The discreteValueCount value
     * @param currentValueIndex The currentValueIndex value
     */
    public LastClickedUpdateMessage(String parameterName, float parameterValue, String displayValue, float parameterOrigin, boolean parameterExists, ParameterType parameterType, short discreteValueCount, int currentValueIndex) {
        this.parameterName = parameterName;
        this.parameterValue = parameterValue;
        this.displayValue = displayValue;
        this.parameterOrigin = parameterOrigin;
        this.parameterExists = parameterExists;
        this.parameterType = parameterType;
        this.discreteValueCount = discreteValueCount;
        this.currentValueIndex = currentValueIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the parameterName value
     *
     * @return parameterName
     */
    public String getParameterName() {
        return parameterName;
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

    /**
     * Get the parameterOrigin value
     *
     * @return parameterOrigin
     */
    public float getParameterOrigin() {
        return parameterOrigin;
    }

    /**
     * Get the parameterExists value
     *
     * @return parameterExists
     */
    public boolean getParameterExists() {
        return parameterExists;
    }

    /**
     * Get the parameterType value
     *
     * @return parameterType
     */
    public ParameterType getParameterType() {
        return parameterType;
    }

    /**
     * Get the discreteValueCount value
     *
     * @return discreteValueCount
     */
    public short getDiscreteValueCount() {
        return discreteValueCount;
    }

    /**
     * Get the currentValueIndex value
     *
     * @return currentValueIndex
     */
    public int getCurrentValueIndex() {
        return currentValueIndex;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (8-bit binary)
     */
    public static final int MAX_PAYLOAD_SIZE = 97;

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

        offset += Encoder.encodeString(buffer, offset, parameterName);
        offset += Encoder.encodeFloat32(buffer, offset, parameterValue);
        offset += Encoder.encodeString(buffer, offset, displayValue);
        offset += Encoder.encodeFloat32(buffer, offset, parameterOrigin);
        offset += Encoder.encodeBool(buffer, offset, parameterExists);
        offset += Encoder.encodeUint8(buffer, offset, parameterType.getValue());
        offset += Encoder.encodeInt16(buffer, offset, discreteValueCount);
        offset += Encoder.encodeUint8(buffer, offset, currentValueIndex);

        return offset - startOffset;
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 33;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded LastClickedUpdateMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static LastClickedUpdateMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for LastClickedUpdateMessage decode");
        }

        int offset = 0;

        // Skip MESSAGE_NAME prefix
        int nameLen = Decoder.decodeUint8(data, offset);
        offset += 1 + nameLen;

        String parameterName = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + parameterName.length();
        float parameterValue = Decoder.decodeFloat32(data, offset);
        offset += 4;
        String displayValue = Decoder.decodeString(data, offset, ProtocolConstants.STRING_MAX_LENGTH);
        offset += 1 + displayValue.length();
        float parameterOrigin = Decoder.decodeFloat32(data, offset);
        offset += 4;
        boolean parameterExists = Decoder.decodeBool(data, offset);
        offset += 1;
        ParameterType parameterType = ParameterType.fromValue(Decoder.decodeUint8(data, offset));
        offset += 1;

        short discreteValueCount = Decoder.decodeInt16(data, offset);
        offset += 2;
        int currentValueIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new LastClickedUpdateMessage(parameterName, parameterValue, displayValue, parameterOrigin, parameterExists, parameterType, discreteValueCount, currentValueIndex);
    }

}  // class Message
