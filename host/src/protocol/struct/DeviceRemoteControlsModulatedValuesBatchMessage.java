package protocol.struct;

import protocol.MessageID;
import protocol.Encoder;
import protocol.Decoder;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceRemoteControlsModulatedValuesBatchMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROLS_MODULATED_VALUES_BATCH message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class DeviceRemoteControlsModulatedValuesBatchMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.DEVICE_REMOTE_CONTROLS_MODULATED_VALUES_BATCH;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int sequenceNumber;
    private final List<Float> modulatedValues;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new DeviceRemoteControlsModulatedValuesBatchMessage
     *
     * @param sequenceNumber The sequenceNumber value
     * @param modulatedValues The modulatedValues value
     */
    public DeviceRemoteControlsModulatedValuesBatchMessage(int sequenceNumber, List<Float> modulatedValues) {
        this.sequenceNumber = sequenceNumber;
        this.modulatedValues = modulatedValues;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the sequenceNumber value
     *
     * @return sequenceNumber
     */
    public int getSequenceNumber() {
        return sequenceNumber;
    }

    /**
     * Get the modulatedValues value
     *
     * @return modulatedValues
     */
    public List<Float> getModulatedValues() {
        return modulatedValues;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    public static final int MAX_PAYLOAD_SIZE = 9;

    /**
     * Encode message to MIDI-safe bytes
     *
     * @return Encoded byte array
     */
    public byte[] encode() {
        byte[] buffer = new byte[MAX_PAYLOAD_SIZE];
        int offset = 0;

        byte[] sequenceNumber_encoded = Encoder.encodeUint8(sequenceNumber);
        System.arraycopy(sequenceNumber_encoded, 0, buffer, offset, sequenceNumber_encoded.length);
        offset += sequenceNumber_encoded.length;
        for (float item : modulatedValues) {
    byte[] item_encoded = Encoder.encodeNorm8(item);
            System.arraycopy(item_encoded, 0, buffer, offset, item_encoded.length);
            offset += item_encoded.length;
        }


        return java.util.Arrays.copyOf(buffer, offset);
    }

    // ============================================================================
    // Decoding
    // ============================================================================

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    private static final int MIN_PAYLOAD_SIZE = 9;

    /**
     * Decode message from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @return Decoded DeviceRemoteControlsModulatedValuesBatchMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static DeviceRemoteControlsModulatedValuesBatchMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for DeviceRemoteControlsModulatedValuesBatchMessage decode");
        }

        int offset = 0;

        int sequenceNumber = Decoder.decodeUint8(data, offset);
        offset += 1;
        List<Float> modulatedValues_list = new ArrayList<>();
        for (int i = 0; i < 8; i++) {
    float item_modulatedValues = Decoder.decodeNorm8(data, offset);
            offset += 1;
            modulatedValues_list.add(item_modulatedValues);
        }


        return new DeviceRemoteControlsModulatedValuesBatchMessage(sequenceNumber, modulatedValues_list);
    }

    // ============================================================================
    // Logging
    // ============================================================================
    
    /**
     * Format float with 4 decimal places, handling edge cases.
     * 
     * @param value Float value to format
     * @return Formatted string (e.g., "3.1416", "NaN", "Inf")
     */
    private static String formatFloat(float value) {
        if (Float.isNaN(value)) return "NaN";
        if (Float.isInfinite(value)) return value > 0 ? "Inf" : "-Inf";
        return String.format("%.4f", value);
    }
    
    /**
     * Convert message to YAML format for logging.
     * 
     * @return YAML string representation
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder(256);
        sb.append("# DeviceRemoteControlsModulatedValuesBatch\n");
        sb.append("deviceRemoteControlsModulatedValuesBatch:\n");
        sb.append("  sequenceNumber: ").append(getSequenceNumber()).append("\n");
        sb.append("  modulatedValues:");
        if (getModulatedValues().isEmpty()) {
            sb.append(" []\n");
        } else {
            sb.append("\n");
            for (float item : getModulatedValues()) {
                sb.append("    - ").append(formatFloat(item)).append("\n");
            }
        }
        return sb.toString();
    }
}  // class Message
