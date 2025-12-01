/**
 * TrackListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_LIST message
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
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

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
    bool isGroup;
    uint8_t trackType;
};

#endif // PROTOCOL_TRACKS_STRUCT

struct TrackListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_LIST;

    uint8_t trackCount;
    uint8_t trackIndex;
    bool isNested;
    std::string parentGroupName;
    std::array<Tracks, 32> tracks;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 1445;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 389;

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

        encodeUint8(ptr, trackCount);
        encodeUint8(ptr, trackIndex);
        encodeBool(ptr, isNested);
        encodeString(ptr, parentGroupName);
        encodeUint8(ptr, tracks.size());
        for (const auto& item : tracks) {
            encodeUint8(ptr, item.trackIndex);
            encodeString(ptr, item.trackName);
            encodeUint32(ptr, item.color);
            encodeBool(ptr, item.isActivated);
            encodeBool(ptr, item.isMute);
            encodeBool(ptr, item.isSolo);
            encodeBool(ptr, item.isGroup);
            encodeUint8(ptr, item.trackType);
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
    static std::optional<TrackListMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t trackCount;
        if (!decodeUint8(ptr, remaining, trackCount)) return std::nullopt;
        uint8_t trackIndex;
        if (!decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        bool isNested;
        if (!decodeBool(ptr, remaining, isNested)) return std::nullopt;
        std::string parentGroupName;
        if (!decodeString(ptr, remaining, parentGroupName)) return std::nullopt;
        uint8_t count_tracks;
        if (!decodeUint8(ptr, remaining, count_tracks)) return std::nullopt;
        std::array<Tracks, 32> tracks_data;
        for (uint8_t i = 0; i < count_tracks && i < 32; ++i) {
            Tracks item;
            if (!decodeUint8(ptr, remaining, item.trackIndex)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.trackName)) return std::nullopt;
            if (!decodeUint32(ptr, remaining, item.color)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.isActivated)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.isMute)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.isSolo)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.isGroup)) return std::nullopt;
            if (!decodeUint8(ptr, remaining, item.trackType)) return std::nullopt;
            tracks_data[i] = item;
        }

        return TrackListMessage{trackCount, trackIndex, isNested, parentGroupName, tracks_data};
    }


    /**
     * Convert message to YAML format for logging
     * 
     * WARNING: Uses static 32KB buffer - log immediately!
     * Multiple calls will overwrite previous results.
     * 
     * @return YAML string representation
     */
    const char* toString() const {
        #ifdef EXTMEM
        static EXTMEM char buffer[32768];  // Use external memory on Teensy
        #else
        static char buffer[32768];  // Standard static buffer
        #endif
        char* ptr = buffer;
        const char* end = buffer + sizeof(buffer) - 1;
        
        ptr += snprintf(ptr, end - ptr, "# TrackList\ntrackList:\n");
        
        ptr += snprintf(ptr, end - ptr, "  trackCount: %lu\n", (unsigned long)trackCount);
        ptr += snprintf(ptr, end - ptr, "  trackIndex: %lu\n", (unsigned long)trackIndex);
        ptr += snprintf(ptr, end - ptr, "  isNested: %s\n", isNested ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  parentGroupName: \"%s\"\n", parentGroupName.c_str());
        ptr += snprintf(ptr, end - ptr, "  tracks:\n");
        for (size_t i = 0; i < tracks.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - trackIndex: %lu\n", (unsigned long)tracks[i].trackIndex);
        ptr += snprintf(ptr, end - ptr, "      trackName: \"%s\"\n", tracks[i].trackName.c_str());
        ptr += snprintf(ptr, end - ptr, "      color: %lu\n", (unsigned long)tracks[i].color);
        ptr += snprintf(ptr, end - ptr, "      isActivated: %s\n", tracks[i].isActivated ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      isMute: %s\n", tracks[i].isMute ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      isSolo: %s\n", tracks[i].isSolo ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      isGroup: %s\n", tracks[i].isGroup ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      trackType: %lu\n", (unsigned long)tracks[i].trackType);
        }
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
