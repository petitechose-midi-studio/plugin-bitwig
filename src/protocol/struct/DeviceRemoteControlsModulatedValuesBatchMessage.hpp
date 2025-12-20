/**
 * DeviceRemoteControlsModulatedValuesBatchMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROLS_MODULATED_VALUES_BATCH message
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

namespace Protocol {



struct DeviceRemoteControlsModulatedValuesBatchMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_REMOTE_CONTROLS_MODULATED_VALUES_BATCH;

    uint8_t sequenceNumber;
    std::array<float, 8> modulatedValues;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 9;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 9;

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

        encodeUint8(ptr, sequenceNumber);
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
    static std::optional<DeviceRemoteControlsModulatedValuesBatchMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t sequenceNumber;
        if (!decodeUint8(ptr, remaining, sequenceNumber)) return std::nullopt;
        std::array<float, 8> modulatedValues_data;
        for (uint8_t i = 0; i < 8; ++i) {
            if (!decodeNorm8(ptr, remaining, modulatedValues_data[i])) return std::nullopt;
        }

        return DeviceRemoteControlsModulatedValuesBatchMessage{sequenceNumber, modulatedValues_data};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceRemoteControlsModulatedValuesBatch\ndeviceRemoteControlsModulatedValuesBatch:\n");

        ptr += snprintf(ptr, end - ptr, "  sequenceNumber: %lu\n", (unsigned long)sequenceNumber);
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
