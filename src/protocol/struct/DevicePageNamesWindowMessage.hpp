/**
 * DevicePageNamesWindowMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_NAMES_WINDOW message
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
#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {



struct DevicePageNamesWindowMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_PAGE_NAMES_WINDOW;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DevicePageNamesWindow";

    uint8_t devicePageCount;
    uint8_t pageStartIndex;
    uint8_t devicePageIndex;
    std::array<std::string, 16> pageNames;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 554;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 26;

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

        Encoder::encodeUint8(ptr, devicePageCount);
        Encoder::encodeUint8(ptr, pageStartIndex);
        Encoder::encodeUint8(ptr, devicePageIndex);
        Encoder::encodeUint8(ptr, pageNames.size());
        for (const auto& item : pageNames) {
            Encoder::encodeString(ptr, item);
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
    static std::optional<DevicePageNamesWindowMessage> decode(
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
        uint8_t devicePageCount;
        if (!Decoder::decodeUint8(ptr, remaining, devicePageCount)) return std::nullopt;
        uint8_t pageStartIndex;
        if (!Decoder::decodeUint8(ptr, remaining, pageStartIndex)) return std::nullopt;
        uint8_t devicePageIndex;
        if (!Decoder::decodeUint8(ptr, remaining, devicePageIndex)) return std::nullopt;
        std::array<std::string, 16> pageNames_data;
        uint8_t count_pageNames;
        if (!Decoder::decodeUint8(ptr, remaining, count_pageNames)) return std::nullopt;
        for (uint8_t i = 0; i < count_pageNames && i < 16; ++i) {
            if (!Decoder::decodeString(ptr, remaining, pageNames_data[i])) return std::nullopt;
        }

        return DevicePageNamesWindowMessage{devicePageCount, pageStartIndex, devicePageIndex, pageNames_data};
    }

};

}  // namespace Protocol
