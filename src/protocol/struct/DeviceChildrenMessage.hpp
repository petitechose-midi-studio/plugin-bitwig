/**
 * DeviceChildrenMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHILDREN message
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
#include "../ChildType.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {


#ifndef PROTOCOL_CHILDREN_STRUCT
#define PROTOCOL_CHILDREN_STRUCT

struct Children {
    uint8_t childIndex;
    std::string childName;
    uint8_t itemType;
};

#endif // PROTOCOL_CHILDREN_STRUCT

struct DeviceChildrenMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_CHILDREN;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceChildren";

    uint8_t deviceIndex;
    uint8_t childType;
    uint8_t childrenCount;
    std::array<Children, 16> children;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 579;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 19;

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

        Encoder::encodeUint8(ptr, deviceIndex);
        Encoder::encodeUint8(ptr, static_cast<uint8_t>(childType));
        Encoder::encodeUint8(ptr, childrenCount);
        Encoder::encodeUint8(ptr, children.size());
        for (const auto& item : children) {
            Encoder::encodeUint8(ptr, item.childIndex);
            Encoder::encodeString(ptr, item.childName);
            Encoder::encodeUint8(ptr, item.itemType);
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
    static std::optional<DeviceChildrenMessage> decode(
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
        uint8_t deviceIndex;
        if (!Decoder::decodeUint8(ptr, remaining, deviceIndex)) return std::nullopt;
        uint8_t childType;
        if (!Decoder::decodeUint8(ptr, remaining, childType)) return std::nullopt;
        uint8_t childrenCount;
        if (!Decoder::decodeUint8(ptr, remaining, childrenCount)) return std::nullopt;
        uint8_t count_children;
        if (!Decoder::decodeUint8(ptr, remaining, count_children)) return std::nullopt;
        std::array<Children, 16> children_data;
        for (uint8_t i = 0; i < count_children && i < 16; ++i) {
            Children item;
            if (!Decoder::decodeUint8(ptr, remaining, item.childIndex)) return std::nullopt;
            if (!Decoder::decodeString(ptr, remaining, item.childName)) return std::nullopt;
            if (!Decoder::decodeUint8(ptr, remaining, item.itemType)) return std::nullopt;
            children_data[i] = item;
        }

        return DeviceChildrenMessage{deviceIndex, childType, childrenCount, children_data};
    }

};

}  // namespace Protocol
