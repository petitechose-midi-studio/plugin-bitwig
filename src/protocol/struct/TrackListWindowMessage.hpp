/**
 * TrackListWindowMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_LIST_WINDOW message
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
#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {


#ifndef PROTOCOL_TRACKS_STRUCT
#define PROTOCOL_TRACKS_STRUCT

struct Tracks {
    uint8_t trackIndex;
    std::string trackName;
    uint32_t color;
    bool isActivated;
    bool isMute;
    bool isSolo;
    bool isMutedBySolo;
    bool isArm;
    bool isGroup;
    TrackType trackType;
    float volume;
    float pan;
};

#endif // PROTOCOL_TRACKS_STRUCT

struct TrackListWindowMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_LIST_WINDOW;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "TrackListWindow";

    uint8_t trackCount;
    uint8_t trackStartIndex;
    uint8_t trackIndex;
    bool isNested;
    std::string parentGroupName;
    std::array<Tracks, 16> tracks;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 902;

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

        // Encode message name (length-prefixed string for bridge logging)
        Encoder::encodeUint8(ptr, static_cast<uint8_t>(strlen(MESSAGE_NAME)));
        for (size_t i = 0; i < strlen(MESSAGE_NAME); ++i) {
            *ptr++ = static_cast<uint8_t>(MESSAGE_NAME[i]);
        }

        Encoder::encodeUint8(ptr, trackCount);
        Encoder::encodeUint8(ptr, trackStartIndex);
        Encoder::encodeUint8(ptr, trackIndex);
        Encoder::encodeBool(ptr, isNested);
        Encoder::encodeString(ptr, parentGroupName);
        Encoder::encodeUint8(ptr, tracks.size());
        for (const auto& item : tracks) {
            Encoder::encodeUint8(ptr, item.trackIndex);
            Encoder::encodeString(ptr, item.trackName);
            Encoder::encodeUint32(ptr, item.color);
            Encoder::encodeBool(ptr, item.isActivated);
            Encoder::encodeBool(ptr, item.isMute);
            Encoder::encodeBool(ptr, item.isSolo);
            Encoder::encodeBool(ptr, item.isMutedBySolo);
            Encoder::encodeBool(ptr, item.isArm);
            Encoder::encodeBool(ptr, item.isGroup);
            Encoder::encodeUint8(ptr, static_cast<uint8_t>(item.trackType));
            Encoder::encodeFloat32(ptr, item.volume);
            Encoder::encodeFloat32(ptr, item.pan);
        }

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<TrackListWindowMessage> decode(
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
        uint8_t trackCount;
        if (!Decoder::decodeUint8(ptr, remaining, trackCount)) return std::nullopt;
        uint8_t trackStartIndex;
        if (!Decoder::decodeUint8(ptr, remaining, trackStartIndex)) return std::nullopt;
        uint8_t trackIndex;
        if (!Decoder::decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        bool isNested;
        if (!Decoder::decodeBool(ptr, remaining, isNested)) return std::nullopt;
        std::string parentGroupName;
        if (!Decoder::decodeString(ptr, remaining, parentGroupName)) return std::nullopt;
        uint8_t count_tracks;
        if (!Decoder::decodeUint8(ptr, remaining, count_tracks)) return std::nullopt;
        std::array<Tracks, 16> tracks_data;
        for (uint8_t i = 0; i < count_tracks && i < 16; ++i) {
            Tracks item;
            if (!Decoder::decodeUint8(ptr, remaining, item.trackIndex)) return std::nullopt;
            if (!Decoder::decodeString(ptr, remaining, item.trackName)) return std::nullopt;
            if (!Decoder::decodeUint32(ptr, remaining, item.color)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isActivated)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isMute)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isSolo)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isMutedBySolo)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isArm)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isGroup)) return std::nullopt;
            uint8_t trackType_raw;
            if (!Decoder::decodeUint8(ptr, remaining, trackType_raw)) return std::nullopt;
            item.trackType = static_cast<TrackType>(trackType_raw);
            if (!Decoder::decodeFloat32(ptr, remaining, item.volume)) return std::nullopt;
            if (!Decoder::decodeFloat32(ptr, remaining, item.pan)) return std::nullopt;
            tracks_data[i] = item;
        }

        return TrackListWindowMessage{trackCount, trackStartIndex, trackIndex, isNested, parentGroupName, tracks_data};
    }

};

}  // namespace Protocol
