package com.midi_studio.protocol.struct;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.Encoder;
import com.midi_studio.protocol.Decoder;

/**
 * TrackSoloMessage - Auto-generated Protocol Message
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SOLO message
 *
 * This class is immutable and uses Encoder for encode/decode operations.
 * All encoding is 7-bit MIDI-safe.
 */
public final class TrackSoloMessage {


    // ============================================================================
    // Auto-detected MessageID for protocol.send()
    // ============================================================================

    public static final MessageID MESSAGE_ID = MessageID.TRACK_SOLO;


    // ============================================================================
    // Fields
    // ============================================================================

    // Origin tracking (set by DecoderRegistry during decode)
    public boolean fromHost = false;

    private final int trackIndex;
    private final boolean isSolo;

    // ============================================================================
    // Constructor
    // ============================================================================

    /**
     * Construct a new TrackSoloMessage
     *
     * @param trackIndex The trackIndex value
     * @param isSolo The isSolo value
     */
    public TrackSoloMessage(int trackIndex, boolean isSolo) {
        this.trackIndex = trackIndex;
        this.isSolo = isSolo;
    }

    // ============================================================================
    // Getters
    // ============================================================================

    /**
     * Get the trackIndex value
     *
     * @return trackIndex
     */
    public int getTrackIndex() {
        return trackIndex;
    }

    /**
     * Get the isSolo value
     *
     * @return isSolo
     */
    public boolean isSolo() {
        return isSolo;
    }

    // ============================================================================
    // Encoding
    // ============================================================================

    /**
     * Maximum payload size in bytes (7-bit encoded)
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

        byte[] trackIndex_encoded = Encoder.encodeUint8(trackIndex);
        System.arraycopy(trackIndex_encoded, 0, buffer, offset, trackIndex_encoded.length);
        offset += trackIndex_encoded.length;
        byte[] isSolo_encoded = Encoder.encodeBool(isSolo);
        System.arraycopy(isSolo_encoded, 0, buffer, offset, isSolo_encoded.length);
        offset += isSolo_encoded.length;

        return buffer;
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
     * @return Decoded TrackSoloMessage instance
     * @throws IllegalArgumentException if data is invalid or insufficient
     */
    public static TrackSoloMessage decode(byte[] data) {
        if (data.length < MIN_PAYLOAD_SIZE) {
            throw new IllegalArgumentException("Insufficient data for TrackSoloMessage decode");
        }

        int offset = 0;

        int trackIndex = Decoder.decodeUint8(data, offset);
        offset += 1;
        boolean isSolo = Decoder.decodeBool(data, offset);
        offset += 1;

        return new TrackSoloMessage(trackIndex, isSolo);
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
        sb.append("# TrackSolo\n");
        sb.append("trackSolo:\n");
        sb.append("  trackIndex: ").append(getTrackIndex()).append("\n");
        sb.append("  isSolo: ").append(isSolo() ? "true" : "false").append("\n");
        return sb.toString();
    }
}  // class Message
