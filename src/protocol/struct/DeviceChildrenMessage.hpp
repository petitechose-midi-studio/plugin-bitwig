/**
 * DeviceChildrenMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHILDREN message
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
#include <array>
#include <cstdint>
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

    uint8_t deviceIndex;
    uint8_t childType;
    uint8_t childrenCount;
    std::array<Children, 16> children;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 564;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 4;

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

        encodeUint8(ptr, deviceIndex);
        encodeUint8(ptr, childType);
        encodeUint8(ptr, childrenCount);
        encodeUint8(ptr, children.size());
        for (const auto& item : children) {
            encodeUint8(ptr, item.childIndex);
            encodeString(ptr, item.childName);
            encodeUint8(ptr, item.itemType);
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

        // Decode fields
        uint8_t deviceIndex;
        if (!decodeUint8(ptr, remaining, deviceIndex)) return std::nullopt;
        uint8_t childType;
        if (!decodeUint8(ptr, remaining, childType)) return std::nullopt;
        uint8_t childrenCount;
        if (!decodeUint8(ptr, remaining, childrenCount)) return std::nullopt;
        uint8_t count_children;
        if (!decodeUint8(ptr, remaining, count_children)) return std::nullopt;
        std::array<Children, 16> children_data;
        for (uint8_t i = 0; i < count_children && i < 16; ++i) {
            Children item;
            if (!decodeUint8(ptr, remaining, item.childIndex)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.childName)) return std::nullopt;
            if (!decodeUint8(ptr, remaining, item.itemType)) return std::nullopt;
            children_data[i] = item;
        }

        return DeviceChildrenMessage{deviceIndex, childType, childrenCount, children_data};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceChildren\ndeviceChildren:\n");

        ptr += snprintf(ptr, end - ptr, "  deviceIndex: %lu\n", (unsigned long)deviceIndex);
        ptr += snprintf(ptr, end - ptr, "  childType: %lu\n", (unsigned long)childType);
        ptr += snprintf(ptr, end - ptr, "  childrenCount: %lu\n", (unsigned long)childrenCount);
        ptr += snprintf(ptr, end - ptr, "  children:\n");
        for (size_t i = 0; i < children.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - childIndex: %lu\n", (unsigned long)children[i].childIndex);
        ptr += snprintf(ptr, end - ptr, "      childName: \"%s\"\n", children[i].childName.c_str());
        ptr += snprintf(ptr, end - ptr, "      itemType: %lu\n", (unsigned long)children[i].itemType);
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
