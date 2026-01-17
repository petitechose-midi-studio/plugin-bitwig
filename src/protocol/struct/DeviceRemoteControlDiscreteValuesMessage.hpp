/**
 * DeviceRemoteControlDiscreteValuesMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_DISCRETE_VALUES message
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
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <vector>

namespace Protocol {



struct DeviceRemoteControlDiscreteValuesMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_REMOTE_CONTROL_DISCRETE_VALUES;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceRemoteControlDiscreteValues";

    uint8_t remoteControlIndex;
    std::vector<std::string> discreteValueNames;
    uint8_t currentValueIndex;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 1093;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 37;

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

        Encoder::encodeUint8(ptr, remoteControlIndex);
        Encoder::encodeUint8(ptr, discreteValueNames.size());
        for (const auto& item : discreteValueNames) {
            Encoder::encodeString(ptr, item);
        }
        Encoder::encodeUint8(ptr, currentValueIndex);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<DeviceRemoteControlDiscreteValuesMessage> decode(
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
        uint8_t remoteControlIndex;
        if (!Decoder::decodeUint8(ptr, remaining, remoteControlIndex)) return std::nullopt;
        std::vector<std::string> discreteValueNames_data;
        uint8_t count_discreteValueNames;
        if (!Decoder::decodeUint8(ptr, remaining, count_discreteValueNames)) return std::nullopt;
        for (uint8_t i = 0; i < count_discreteValueNames && i < 32; ++i) {
            std::string temp_item;
            if (!Decoder::decodeString(ptr, remaining, temp_item)) return std::nullopt;
            discreteValueNames_data.push_back(temp_item);
        }
        uint8_t currentValueIndex;
        if (!Decoder::decodeUint8(ptr, remaining, currentValueIndex)) return std::nullopt;

        return DeviceRemoteControlDiscreteValuesMessage{remoteControlIndex, discreteValueNames_data, currentValueIndex};
    }

};

}  // namespace Protocol
