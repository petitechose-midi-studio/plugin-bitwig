package com.midi_studio.protocol.struct;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.Encoder;
import com.midi_studio.protocol.Decoder;

/**
 * DeviceSelectByIndexMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_SELECT_BY_INDEX message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceSelectByIndexMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_SELECT_BY_INDEX;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int deviceIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceSelectByIndexMessage
     *
     * @param deviceIndex The deviceIndex value
     */
    public DeviceSelectByIndexMessage(int deviceIndex) {
        this.deviceIndex = deviceIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the deviceIndex value
     *
     * @return deviceIndex
     */
    public int getDeviceIndex() {
        return deviceIndex;
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

        byte[] deviceIndex_encoded = Encoder.encodeUint8(deviceIndex);
        System.arraycopy(deviceIndex_encoded, 0, buffer, offset, deviceIndex_encoded.length);
        offset += deviceIndex_encoded.length;

        return buffer;
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
     * @return Decoded DeviceSelectByIndexMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceSelectByIndexMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceSelectByIndexMessage decode");
        }

        int offset = 0;

        int deviceIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new DeviceSelectByIndexMessage(deviceIndex);
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
        sb.append("# DeviceSelectByIndex\n");
        sb.append("deviceSelectByIndex:\n");
        sb.append("  deviceIndex: ").append(getDeviceIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
