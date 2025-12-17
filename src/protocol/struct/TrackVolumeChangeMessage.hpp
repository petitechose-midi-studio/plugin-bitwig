/**
 * TrackVolumeChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_VOLUME_CHANGE message
 *
 * This struct uses encode/decode functions from Protocol namespace.
 * All encoding is 7-bit MIDI-safe. Performance is identical to inline
 * code due to static inline + compiler optimization.
 */

#pragma once

#include "../Encoder.hpp"
#include "../Decoder.hpp"
#include "../MessageID.hpp"
#include "../ProtocolConstants.hpp"
#include "../Logger.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace Protocol {



struct TrackVolumeChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_VOLUME_CHANGE;

    uint8_t trackIndex;
    float volume;
    std::string volumeDisplay;
    bool isEcho;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 40;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 8;

    /**
     * Encode struct to MIDI-safe bytes
     *
     * @param buffer Output buffer (must have >= MAX_PAYLOAD_SIZE bytes)
     * @param bufferSize Size of output buffer
     * @return Number of bytes written, or 0 if buffer too small
     */
    uint16_t encode(uint8_t* buffer, uint16_t bufferSize) const {
        if (bufferSize < MAX_PAYLOAD_SIZE) return 0;

        uint8_t* ptr = buffer;

        encodeUint8(ptr, trackIndex);
        encodeFloat32(ptr, volume);
        encodeString(ptr, volumeDisplay);
        encodeBool(ptr, isEcho);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<TrackVolumeChangeMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t trackIndex;
        if (!decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        float volume;
        if (!decodeFloat32(ptr, remaining, volume)) return std::nullopt;
        std::string volumeDisplay;
        if (!decodeString(ptr, remaining, volumeDisplay)) return std::nullopt;
        bool isEcho;
        if (!decodeBool(ptr, remaining, isEcho)) return std::nullopt;

        return TrackVolumeChangeMessage{trackIndex, volume, volumeDisplay, isEcho};
    }


    /**
     * Convert message to YAML format for logging
     *
     * WARNING: Uses shared g_logBuffer - log immediately!
     * Multiple calls will overwrite previous results.
     *
     * @return YAML string representation
     */
    const char* toString() const {
        char* ptr = g_logBuffer;
        const char* end = g_logBuffer + LOG_BUFFER_SIZE - 1;

        ptr += snprintf(ptr, end - ptr, "# TrackVolumeChange\ntrackVolumeChange:\n");

        ptr += snprintf(ptr, end - ptr, "  trackIndex: %lu\n", (unsigned long)trackIndex);
        {
            char floatBuf_volume[16];
            floatToString(floatBuf_volume, sizeof(floatBuf_volume), volume);
            ptr += snprintf(ptr, end - ptr, "  volume: %s\n", floatBuf_volume);
        }
        ptr += snprintf(ptr, end - ptr, "  volumeDisplay: \"%s\"\n", volumeDisplay.c_str());
        ptr += snprintf(ptr, end - ptr, "  isEcho: %s\n", isEcho ? "true" : "false");

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
