/**
 * TrackMuteMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_MUTE message
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



struct TrackMuteMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_MUTE;

    uint8_t trackIndex;
    bool isMute;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 2;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 2;

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
        encodeBool(ptr, isMute);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<TrackMuteMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t trackIndex;
        if (!decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        bool isMute;
        if (!decodeBool(ptr, remaining, isMute)) return std::nullopt;

        return TrackMuteMessage{trackIndex, isMute};
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
        
        ptr += snprintf(ptr, end - ptr, "# TrackMute\ntrackMute:\n");
        
        ptr += snprintf(ptr, end - ptr, "  trackIndex: %lu\n", (unsigned long)trackIndex);
        ptr += snprintf(ptr, end - ptr, "  isMute: %s\n", isMute ? "true" : "false");
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
