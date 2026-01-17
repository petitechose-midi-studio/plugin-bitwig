/**
 * DeviceRemoteControlsBatchMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROLS_BATCH message
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



struct DeviceRemoteControlsBatchMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_REMOTE_CONTROLS_BATCH;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceRemoteControlsBatch";

    uint8_t sequenceNumber;
    uint8_t dirtyMask;
    uint8_t echoMask;
    uint8_t hasAutomationMask;
    std::array<float, 8> values;
    std::array<float, 8> modulatedValues;
    std::array<std::string, 8> displayValues;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 313;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 33;

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

        Encoder::encodeUint8(ptr, sequenceNumber);
        Encoder::encodeUint8(ptr, dirtyMask);
        Encoder::encodeUint8(ptr, echoMask);
        Encoder::encodeUint8(ptr, hasAutomationMask);
        Encoder::encodeUint8(ptr, values.size());
        for (const auto& item : values) {
            Encoder::encodeNorm8(ptr, item);
        }
        Encoder::encodeUint8(ptr, modulatedValues.size());
        for (const auto& item : modulatedValues) {
            Encoder::encodeNorm8(ptr, item);
        }
        Encoder::encodeUint8(ptr, displayValues.size());
        for (const auto& item : displayValues) {
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
    static std::optional<DeviceRemoteControlsBatchMessage> decode(
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
        uint8_t sequenceNumber;
        if (!Decoder::decodeUint8(ptr, remaining, sequenceNumber)) return std::nullopt;
        uint8_t dirtyMask;
        if (!Decoder::decodeUint8(ptr, remaining, dirtyMask)) return std::nullopt;
        uint8_t echoMask;
        if (!Decoder::decodeUint8(ptr, remaining, echoMask)) return std::nullopt;
        uint8_t hasAutomationMask;
        if (!Decoder::decodeUint8(ptr, remaining, hasAutomationMask)) return std::nullopt;
        std::array<float, 8> values_data;
        uint8_t count_values;
        if (!Decoder::decodeUint8(ptr, remaining, count_values)) return std::nullopt;
        for (uint8_t i = 0; i < count_values && i < 8; ++i) {
            if (!Decoder::decodeNorm8(ptr, remaining, values_data[i])) return std::nullopt;
        }
        std::array<float, 8> modulatedValues_data;
        uint8_t count_modulatedValues;
        if (!Decoder::decodeUint8(ptr, remaining, count_modulatedValues)) return std::nullopt;
        for (uint8_t i = 0; i < count_modulatedValues && i < 8; ++i) {
            if (!Decoder::decodeNorm8(ptr, remaining, modulatedValues_data[i])) return std::nullopt;
        }
        std::array<std::string, 8> displayValues_data;
        uint8_t count_displayValues;
        if (!Decoder::decodeUint8(ptr, remaining, count_displayValues)) return std::nullopt;
        for (uint8_t i = 0; i < count_displayValues && i < 8; ++i) {
            if (!Decoder::decodeString(ptr, remaining, displayValues_data[i])) return std::nullopt;
        }

        return DeviceRemoteControlsBatchMessage{sequenceNumber, dirtyMask, echoMask, hasAutomationMask, values_data, modulatedValues_data, displayValues_data};
    }

};

}  // namespace Protocol
