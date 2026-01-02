/**
 * DeviceChangeHeaderMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE_HEADER message
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
#include "../DeviceType.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {


#ifndef PROTOCOL_PAGEINFO_STRUCT
#define PROTOCOL_PAGEINFO_STRUCT

struct PageInfo {
    uint8_t devicePageIndex;
    uint8_t devicePageCount;
    std::string devicePageName;
};

#endif // PROTOCOL_PAGEINFO_STRUCT

struct DeviceChangeHeaderMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_CHANGE_HEADER;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceChangeHeader";

    std::string deviceName;
    bool isEnabled;
    DeviceType deviceType;
    PageInfo pageInfo;
    std::array<uint8_t, 4> childrenTypes;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 94;

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

        Encoder::encodeString(ptr, deviceName);
        Encoder::encodeBool(ptr, isEnabled);
        Encoder::encodeUint8(ptr, static_cast<uint8_t>(deviceType));
        Encoder::encodeUint8(ptr, pageInfo.devicePageIndex);
        Encoder::encodeUint8(ptr, pageInfo.devicePageCount);
        Encoder::encodeString(ptr, pageInfo.devicePageName);
        Encoder::encodeUint8(ptr, childrenTypes.size());
        for (const auto& item : childrenTypes) {
            Encoder::encodeUint8(ptr, item);
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
    static std::optional<DeviceChangeHeaderMessage> decode(
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
        std::string deviceName;
        if (!Decoder::decodeString(ptr, remaining, deviceName)) return std::nullopt;
        bool isEnabled;
        if (!Decoder::decodeBool(ptr, remaining, isEnabled)) return std::nullopt;
        uint8_t deviceType_raw;
        if (!Decoder::decodeUint8(ptr, remaining, deviceType_raw)) return std::nullopt;
        DeviceType deviceType = static_cast<DeviceType>(deviceType_raw);
        PageInfo pageInfo_data;
        if (!Decoder::decodeUint8(ptr, remaining, pageInfo_data.devicePageIndex)) return std::nullopt;
        if (!Decoder::decodeUint8(ptr, remaining, pageInfo_data.devicePageCount)) return std::nullopt;
        if (!Decoder::decodeString(ptr, remaining, pageInfo_data.devicePageName)) return std::nullopt;
        std::array<uint8_t, 4> childrenTypes_data;
        uint8_t count_childrenTypes;
        if (!Decoder::decodeUint8(ptr, remaining, count_childrenTypes)) return std::nullopt;
        for (uint8_t i = 0; i < count_childrenTypes && i < 4; ++i) {
            if (!Decoder::decodeUint8(ptr, remaining, childrenTypes_data[i])) return std::nullopt;
        }

        return DeviceChangeHeaderMessage{deviceName, isEnabled, deviceType, pageInfo_data, childrenTypes_data};
    }

};

}  // namespace Protocol
