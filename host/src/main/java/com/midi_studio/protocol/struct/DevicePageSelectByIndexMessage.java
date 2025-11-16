package com.midi_studio.protocol.struct;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.Encoder;
import com.midi_studio.protocol.Decoder;

/**
 * DevicePageSelectByIndexMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_SELECT_BY_INDEX message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DevicePageSelectByIndexMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_PAGE_SELECT_BY_INDEX;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int devicePageIndex;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DevicePageSelectByIndexMessage
     *
     * @param devicePageIndex The devicePageIndex value
     */
    public DevicePageSelectByIndexMessage(int devicePageIndex) {
        this.devicePageIndex = devicePageIndex;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the devicePageIndex value
     *
     * @return devicePageIndex
     */
    public int getDevicePageIndex() {
        return devicePageIndex;
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

        byte[] devicePageIndex_encoded = Encoder.encodeUint8(devicePageIndex);
        System.arraycopy(devicePageIndex_encoded, 0, buffer, offset, devicePageIndex_encoded.length);
        offset += devicePageIndex_encoded.length;

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
     * @return Decoded DevicePageSelectByIndexMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DevicePageSelectByIndexMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DevicePageSelectByIndexMessage decode");
        }

        int offset = 0;

        int devicePageIndex = Decoder.decodeUint8(data, offset);
        offset += 1;

        return new DevicePageSelectByIndexMessage(devicePageIndex);
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
        sb.append("# DevicePageSelectByIndex\n");
        sb.append("devicePageSelectByIndex:\n");
        sb.append("  devicePageIndex: ").append(getDevicePageIndex()).append("\n");
        return sb.toString();
    }
}  // class Message
