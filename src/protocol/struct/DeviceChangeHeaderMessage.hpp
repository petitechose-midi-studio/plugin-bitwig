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
#include "../Logger.hpp"
#include <array>
#include <cstdint>
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

    std::string deviceName;
    bool isEnabled;
    uint8_t deviceType;
    PageInfo pageInfo;
    std::array<uint8_t, 4> childrenTypes;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 74;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 10;

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

        encodeString(ptr, deviceName);
        encodeBool(ptr, isEnabled);
        encodeUint8(ptr, deviceType);
        encodeUint8(ptr, pageInfo.devicePageIndex);
        encodeUint8(ptr, pageInfo.devicePageCount);
        encodeString(ptr, pageInfo.devicePageName);
        for (const auto& item : childrenTypes) {
            encodeUint8(ptr, item);
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

        // Decode fields
        std::string deviceName;
        if (!decodeString(ptr, remaining, deviceName)) return std::nullopt;
        bool isEnabled;
        if (!decodeBool(ptr, remaining, isEnabled)) return std::nullopt;
        uint8_t deviceType;
        if (!decodeUint8(ptr, remaining, deviceType)) return std::nullopt;
        PageInfo pageInfo_data;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageIndex)) return std::nullopt;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageCount)) return std::nullopt;
        if (!decodeString(ptr, remaining, pageInfo_data.devicePageName)) return std::nullopt;
        std::array<uint8_t, 4> childrenTypes_data;
        for (uint8_t i = 0; i < 4; ++i) {
            if (!decodeUint8(ptr, remaining, childrenTypes_data[i])) return std::nullopt;
        }

        return DeviceChangeHeaderMessage{deviceName, isEnabled, deviceType, pageInfo_data, childrenTypes_data};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceChangeHeader\ndeviceChangeHeader:\n");

        ptr += snprintf(ptr, end - ptr, "  deviceName: \"%s\"\n", deviceName.c_str());
        ptr += snprintf(ptr, end - ptr, "  isEnabled: %s\n", isEnabled ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  deviceType: %lu\n", (unsigned long)deviceType);
        ptr += snprintf(ptr, end - ptr, "  pageInfo:\n");
        ptr += snprintf(ptr, end - ptr, "    devicePageIndex: %lu\n", (unsigned long)pageInfo.devicePageIndex);
        ptr += snprintf(ptr, end - ptr, "    devicePageCount: %lu\n", (unsigned long)pageInfo.devicePageCount);
        ptr += snprintf(ptr, end - ptr, "    devicePageName: \"%s\"\n", pageInfo.devicePageName.c_str());
        ptr += snprintf(ptr, end - ptr, "  childrenTypes:");
        if (childrenTypes.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            for (size_t i = 0; i < childrenTypes.size(); ++i) {
                ptr += snprintf(ptr, end - ptr, "    - %lu\n", (unsigned long)childrenTypes[i]);
            }
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
