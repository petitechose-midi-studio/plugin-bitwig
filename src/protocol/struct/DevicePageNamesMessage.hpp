/**
 * DevicePageNamesMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_NAMES message
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

namespace Protocol {



struct DevicePageNamesMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_PAGE_NAMES;

    uint8_t devicePageCount;
    uint8_t devicePageIndex;
    std::array<std::string, 32> pageNames;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 1058;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 34;

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

        encodeUint8(ptr, devicePageCount);
        encodeUint8(ptr, devicePageIndex);
        encodeUint8(ptr, pageNames.size());
        for (const auto& item : pageNames) {
            encodeString(ptr, item);
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
    static std::optional<DevicePageNamesMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t devicePageCount;
        if (!decodeUint8(ptr, remaining, devicePageCount)) return std::nullopt;
        uint8_t devicePageIndex;
        if (!decodeUint8(ptr, remaining, devicePageIndex)) return std::nullopt;
        uint8_t count_pageNames;
        if (!decodeUint8(ptr, remaining, count_pageNames)) return std::nullopt;
        std::array<std::string, 32> pageNames_data;
        for (uint8_t i = 0; i < count_pageNames && i < 32; ++i) {
            if (!decodeString(ptr, remaining, pageNames_data[i])) return std::nullopt;
        }

        return DevicePageNamesMessage{devicePageCount, devicePageIndex, pageNames_data};
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

        ptr += snprintf(ptr, end - ptr, "# DevicePageNames\ndevicePageNames:\n");

        ptr += snprintf(ptr, end - ptr, "  devicePageCount: %lu\n", (unsigned long)devicePageCount);
        ptr += snprintf(ptr, end - ptr, "  devicePageIndex: %lu\n", (unsigned long)devicePageIndex);
        ptr += snprintf(ptr, end - ptr, "  pageNames:");
        if (pageNames.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            for (size_t i = 0; i < pageNames.size(); ++i) {
                ptr += snprintf(ptr, end - ptr, "    - \"%s\"\n", pageNames[i].c_str());
            }
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
