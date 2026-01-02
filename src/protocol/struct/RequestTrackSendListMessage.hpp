/**
 * RequestTrackSendListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_TRACK_SEND_LIST message
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
#include <cstdint>
#include <cstring>
#include <optional>

namespace Protocol {



struct RequestTrackSendListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::REQUEST_TRACK_SEND_LIST;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "RequestTrackSendList";


    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 21;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 21;

    /**
     * Encode struct to MIDI-safe bytes (message name only, no fields)
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

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes (empty message with name prefix)
     * @return Always returns empty struct after skipping name prefix
     */
    static std::optional<RequestTrackSendListMessage> decode(const uint8_t* data, uint16_t len) {
        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;
        const uint8_t* ptr = data;
        size_t remaining = len;
        // Skip MESSAGE_NAME prefix
        uint8_t nameLen;
        if (!Decoder::decodeUint8(ptr, remaining, nameLen)) return std::nullopt;
        ptr += nameLen;
        remaining -= nameLen;
        return RequestTrackSendListMessage{};
    }

};

}  // namespace Protocol
