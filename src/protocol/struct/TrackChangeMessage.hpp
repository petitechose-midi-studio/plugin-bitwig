/**
 * TrackChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_CHANGE message
 *
 * This struct uses encode/decode functions from Protocol namespace.
 * All encoding is 8-bit binary (Serial8). Performance is identical to inline
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



struct TrackChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_CHANGE;

    std::string trackName;
    uint32_t color;
    uint8_t trackIndex;
    uint8_t trackType;
    bool isActivated;
    bool isMute;
    bool isSolo;
    bool isMutedBySolo;
    bool isArm;
    float volume;
    std::string volumeDisplay;
    float pan;
    std::string panDisplay;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 118;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 22;

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

        encodeString(ptr, trackName);
        encodeUint32(ptr, color);
        encodeUint8(ptr, trackIndex);
        encodeUint8(ptr, trackType);
        encodeBool(ptr, isActivated);
        encodeBool(ptr, isMute);
        encodeBool(ptr, isSolo);
        encodeBool(ptr, isMutedBySolo);
        encodeBool(ptr, isArm);
        encodeFloat32(ptr, volume);
        encodeString(ptr, volumeDisplay);
        encodeFloat32(ptr, pan);
        encodeString(ptr, panDisplay);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<TrackChangeMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        std::string trackName;
        if (!decodeString(ptr, remaining, trackName)) return std::nullopt;
        uint32_t color;
        if (!decodeUint32(ptr, remaining, color)) return std::nullopt;
        uint8_t trackIndex;
        if (!decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        uint8_t trackType;
        if (!decodeUint8(ptr, remaining, trackType)) return std::nullopt;
        bool isActivated;
        if (!decodeBool(ptr, remaining, isActivated)) return std::nullopt;
        bool isMute;
        if (!decodeBool(ptr, remaining, isMute)) return std::nullopt;
        bool isSolo;
        if (!decodeBool(ptr, remaining, isSolo)) return std::nullopt;
        bool isMutedBySolo;
        if (!decodeBool(ptr, remaining, isMutedBySolo)) return std::nullopt;
        bool isArm;
        if (!decodeBool(ptr, remaining, isArm)) return std::nullopt;
        float volume;
        if (!decodeFloat32(ptr, remaining, volume)) return std::nullopt;
        std::string volumeDisplay;
        if (!decodeString(ptr, remaining, volumeDisplay)) return std::nullopt;
        float pan;
        if (!decodeFloat32(ptr, remaining, pan)) return std::nullopt;
        std::string panDisplay;
        if (!decodeString(ptr, remaining, panDisplay)) return std::nullopt;

        return TrackChangeMessage{trackName, color, trackIndex, trackType, isActivated, isMute, isSolo, isMutedBySolo, isArm, volume, volumeDisplay, pan, panDisplay};
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

        ptr += snprintf(ptr, end - ptr, "# TrackChange\ntrackChange:\n");

        ptr += snprintf(ptr, end - ptr, "  trackName: \"%s\"\n", trackName.c_str());
        ptr += snprintf(ptr, end - ptr, "  color: %lu\n", (unsigned long)color);
        ptr += snprintf(ptr, end - ptr, "  trackIndex: %lu\n", (unsigned long)trackIndex);
        ptr += snprintf(ptr, end - ptr, "  trackType: %lu\n", (unsigned long)trackType);
        ptr += snprintf(ptr, end - ptr, "  isActivated: %s\n", isActivated ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  isMute: %s\n", isMute ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  isSolo: %s\n", isSolo ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  isMutedBySolo: %s\n", isMutedBySolo ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  isArm: %s\n", isArm ? "true" : "false");
        {
            char floatBuf_volume[16];
            floatToString(floatBuf_volume, sizeof(floatBuf_volume), volume);
            ptr += snprintf(ptr, end - ptr, "  volume: %s\n", floatBuf_volume);
        }
        ptr += snprintf(ptr, end - ptr, "  volumeDisplay: \"%s\"\n", volumeDisplay.c_str());
        {
            char floatBuf_pan[16];
            floatToString(floatBuf_pan, sizeof(floatBuf_pan), pan);
            ptr += snprintf(ptr, end - ptr, "  pan: %s\n", floatBuf_pan);
        }
        ptr += snprintf(ptr, end - ptr, "  panDisplay: \"%s\"\n", panDisplay.c_str());

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
