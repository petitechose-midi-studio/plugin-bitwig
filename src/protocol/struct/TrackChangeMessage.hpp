/**
 * TrackChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_CHANGE message
 *
 * This struct uses encode/decode functions from Protocol namespace.
 * All encoding is 8-bit binary (Binary). Performance is identical to inline
 * code due to static inline + compiler optimization.
 */

#pragma once

#include "../Encoder.hpp"
#include "../Decoder.hpp"
#include "../MessageID.hpp"
#include "../ProtocolConstants.hpp"
#include "../TrackType.hpp"
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {



struct TrackChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_CHANGE;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "TrackChange";

    std::string trackName;
    uint32_t color;
    uint8_t trackIndex;
    TrackType trackType;
    bool isActivated;
    bool isMute;
    bool isSolo;
    bool isMutedBySolo;
    bool isArm;
    float volume;
    std::string volumeDisplay;
    float pan;
    std::string panDisplay;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 130;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 34;

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

        // Encode message name (length-prefixed string for bridge logging)
        Encoder::encodeUint8(ptr, static_cast<uint8_t>(strlen(MESSAGE_NAME)));
        for (size_t i = 0; i < strlen(MESSAGE_NAME); ++i) {
            *ptr++ = static_cast<uint8_t>(MESSAGE_NAME[i]);
        }

        Encoder::encodeString(ptr, trackName);
        Encoder::encodeUint32(ptr, color);
        Encoder::encodeUint8(ptr, trackIndex);
        Encoder::encodeUint8(ptr, static_cast<uint8_t>(trackType));
        Encoder::encodeBool(ptr, isActivated);
        Encoder::encodeBool(ptr, isMute);
        Encoder::encodeBool(ptr, isSolo);
        Encoder::encodeBool(ptr, isMutedBySolo);
        Encoder::encodeBool(ptr, isArm);
        Encoder::encodeFloat32(ptr, volume);
        Encoder::encodeString(ptr, volumeDisplay);
        Encoder::encodeFloat32(ptr, pan);
        Encoder::encodeString(ptr, panDisplay);

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

        // Skip MESSAGE_NAME prefix
        uint8_t nameLen;
        if (!Decoder::decodeUint8(ptr, remaining, nameLen)) return std::nullopt;
        ptr += nameLen;
        remaining -= nameLen;

        // Decode fields
        std::string trackName;
        if (!Decoder::decodeString(ptr, remaining, trackName)) return std::nullopt;
        uint32_t color;
        if (!Decoder::decodeUint32(ptr, remaining, color)) return std::nullopt;
        uint8_t trackIndex;
        if (!Decoder::decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        uint8_t trackType_raw;
        if (!Decoder::decodeUint8(ptr, remaining, trackType_raw)) return std::nullopt;
        TrackType trackType = static_cast<TrackType>(trackType_raw);
        bool isActivated;
        if (!Decoder::decodeBool(ptr, remaining, isActivated)) return std::nullopt;
        bool isMute;
        if (!Decoder::decodeBool(ptr, remaining, isMute)) return std::nullopt;
        bool isSolo;
        if (!Decoder::decodeBool(ptr, remaining, isSolo)) return std::nullopt;
        bool isMutedBySolo;
        if (!Decoder::decodeBool(ptr, remaining, isMutedBySolo)) return std::nullopt;
        bool isArm;
        if (!Decoder::decodeBool(ptr, remaining, isArm)) return std::nullopt;
        float volume;
        if (!Decoder::decodeFloat32(ptr, remaining, volume)) return std::nullopt;
        std::string volumeDisplay;
        if (!Decoder::decodeString(ptr, remaining, volumeDisplay)) return std::nullopt;
        float pan;
        if (!Decoder::decodeFloat32(ptr, remaining, pan)) return std::nullopt;
        std::string panDisplay;
        if (!Decoder::decodeString(ptr, remaining, panDisplay)) return std::nullopt;

        return TrackChangeMessage{trackName, color, trackIndex, trackType, isActivated, isMute, isSolo, isMutedBySolo, isArm, volume, volumeDisplay, pan, panDisplay};
    }

};

}  // namespace Protocol
