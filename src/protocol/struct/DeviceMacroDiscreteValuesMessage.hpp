/**
 * DeviceMacroDiscreteValuesMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_MACRO_DISCRETE_VALUES message
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
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Protocol {



struct DeviceMacroDiscreteValuesMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_MACRO_DISCRETE_VALUES;

    uint8_t parameterIndex;
    std::vector<std::string> discreteValueNames;
    uint8_t currentValueIndex;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 1058;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 3;

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

        encodeUint8(ptr, parameterIndex);
        encodeUint8(ptr, discreteValueNames.size());
        for (const auto& item : discreteValueNames) {
            encodeString(ptr, item);
        }
        encodeUint8(ptr, currentValueIndex);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<DeviceMacroDiscreteValuesMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t parameterIndex;
        if (!decodeUint8(ptr, remaining, parameterIndex)) return std::nullopt;
        uint8_t count_discreteValueNames;
        if (!decodeUint8(ptr, remaining, count_discreteValueNames)) return std::nullopt;
        std::vector<std::string> discreteValueNames_data;
        for (uint8_t i = 0; i < count_discreteValueNames && i < 32; ++i) {
            std::string temp_item;
            if (!decodeString(ptr, remaining, temp_item)) return std::nullopt;
            discreteValueNames_data.push_back(temp_item);
        }
        uint8_t currentValueIndex;
        if (!decodeUint8(ptr, remaining, currentValueIndex)) return std::nullopt;

        return DeviceMacroDiscreteValuesMessage{parameterIndex, discreteValueNames_data, currentValueIndex};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceMacroDiscreteValues\ndeviceMacroDiscreteValues:\n");

        ptr += snprintf(ptr, end - ptr, "  parameterIndex: %lu\n", (unsigned long)parameterIndex);
        ptr += snprintf(ptr, end - ptr, "  discreteValueNames:");
        if (discreteValueNames.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            for (size_t i = 0; i < discreteValueNames.size(); ++i) {
                ptr += snprintf(ptr, end - ptr, "    - \"%s\"\n", discreteValueNames[i].c_str());
            }
        }
        ptr += snprintf(ptr, end - ptr, "  currentValueIndex: %lu\n", (unsigned long)currentValueIndex);

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
