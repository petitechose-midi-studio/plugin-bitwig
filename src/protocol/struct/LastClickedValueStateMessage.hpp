/**
 * LastClickedValueStateMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: LAST_CLICKED_VALUE_STATE message
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
#include <string>

namespace Protocol {



struct LastClickedValueStateMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::LAST_CLICKED_VALUE_STATE;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "LastClickedValueState";

    float parameterValue;
    std::string displayValue;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 59;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 27;

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

        Encoder::encodeFloat32(ptr, parameterValue);
        Encoder::encodeString(ptr, displayValue);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<LastClickedValueStateMessage> decode(
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
        float parameterValue;
        if (!Decoder::decodeFloat32(ptr, remaining, parameterValue)) return std::nullopt;
        std::string displayValue;
        if (!Decoder::decodeString(ptr, remaining, displayValue)) return std::nullopt;

        return LastClickedValueStateMessage{parameterValue, displayValue};
    }

};

}  // namespace Protocol
