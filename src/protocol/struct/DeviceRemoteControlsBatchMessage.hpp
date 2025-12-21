/**
 * DeviceRemoteControlsBatchMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROLS_BATCH message
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
#include <cstring>
#include <optional>

namespace Protocol {



struct DeviceRemoteControlsBatchMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_REMOTE_CONTROLS_BATCH;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceRemoteControlsBatch";

    uint8_t sequenceNumber;
    uint8_t dirtyMask;
    uint8_t echoMask;
    std::array<float, 8> values;
    std::array<float, 8> modulatedValues;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 47;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 31;

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
        encodeUint8(ptr, static_cast<uint8_t>(strlen(MESSAGE_NAME)));
        for (size_t i = 0; i < strlen(MESSAGE_NAME); ++i) {
            *ptr++ = static_cast<uint8_t>(MESSAGE_NAME[i]);
        }

        encodeUint8(ptr, sequenceNumber);
        encodeUint8(ptr, dirtyMask);
        encodeUint8(ptr, echoMask);
        encodeUint8(ptr, values.size());
        for (const auto& item : values) {
            encodeNorm8(ptr, item);
        }
        encodeUint8(ptr, modulatedValues.size());
        for (const auto& item : modulatedValues) {
            encodeNorm8(ptr, item);
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
    static std::optional<DeviceRemoteControlsBatchMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Skip message name prefix (length + name bytes)
        uint8_t nameLen;
        if (!decodeUint8(ptr, remaining, nameLen)) return std::nullopt;
        if (remaining < nameLen) return std::nullopt;
        ptr += nameLen;
        remaining -= nameLen;

        // Decode fields
        uint8_t sequenceNumber;
        if (!decodeUint8(ptr, remaining, sequenceNumber)) return std::nullopt;
        uint8_t dirtyMask;
        if (!decodeUint8(ptr, remaining, dirtyMask)) return std::nullopt;
        uint8_t echoMask;
        if (!decodeUint8(ptr, remaining, echoMask)) return std::nullopt;
        std::array<float, 8> values_data;
        uint8_t count_values;
        if (!decodeUint8(ptr, remaining, count_values)) return std::nullopt;
        for (uint8_t i = 0; i < count_values && i < 8; ++i) {
            if (!decodeNorm8(ptr, remaining, values_data[i])) return std::nullopt;
        }
        std::array<float, 8> modulatedValues_data;
        uint8_t count_modulatedValues;
        if (!decodeUint8(ptr, remaining, count_modulatedValues)) return std::nullopt;
        for (uint8_t i = 0; i < count_modulatedValues && i < 8; ++i) {
            if (!decodeNorm8(ptr, remaining, modulatedValues_data[i])) return std::nullopt;
        }

        return DeviceRemoteControlsBatchMessage{sequenceNumber, dirtyMask, echoMask, values_data, modulatedValues_data};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceRemoteControlsBatch\ndeviceRemoteControlsBatch:\n");

        ptr += snprintf(ptr, end - ptr, "  sequenceNumber: %lu\n", (unsigned long)sequenceNumber);
        ptr += snprintf(ptr, end - ptr, "  dirtyMask: %lu\n", (unsigned long)dirtyMask);
        ptr += snprintf(ptr, end - ptr, "  echoMask: %lu\n", (unsigned long)echoMask);
        ptr += snprintf(ptr, end - ptr, "  values:");
        if (values.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            {
                char floatBuf_values[16];
                for (size_t i = 0; i < values.size(); ++i) {
                    floatToString(floatBuf_values, sizeof(floatBuf_values), values[i]);
                    ptr += snprintf(ptr, end - ptr, "    - %s\n", floatBuf_values);
                }
            }
        }
        ptr += snprintf(ptr, end - ptr, "  modulatedValues:");
        if (modulatedValues.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            {
                char floatBuf_modulatedValues[16];
                for (size_t i = 0; i < modulatedValues.size(); ++i) {
                    floatToString(floatBuf_modulatedValues, sizeof(floatBuf_modulatedValues), modulatedValues[i]);
                    ptr += snprintf(ptr, end - ptr, "    - %s\n", floatBuf_modulatedValues);
                }
            }
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
