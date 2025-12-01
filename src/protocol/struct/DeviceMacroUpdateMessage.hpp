/**
 * DeviceMacroUpdateMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_MACRO_UPDATE message
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
#include <vector>

namespace Protocol {



struct DeviceMacroUpdateMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_MACRO_UPDATE;

    uint8_t parameterIndex;
    std::string parameterName;
    float parameterValue;
    std::string displayValue;
    float parameterOrigin;
    bool parameterExists;
    uint8_t parameterType;
    int16_t discreteValueCount;
    uint8_t currentValueIndex;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 83;

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

        encodeUint8(ptr, parameterIndex);
        encodeString(ptr, parameterName);
        encodeFloat32(ptr, parameterValue);
        encodeString(ptr, displayValue);
        encodeFloat32(ptr, parameterOrigin);
        encodeBool(ptr, parameterExists);
        encodeUint8(ptr, parameterType);
        encodeInt16(ptr, discreteValueCount);
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
    static std::optional<DeviceMacroUpdateMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t parameterIndex;
        if (!decodeUint8(ptr, remaining, parameterIndex)) return std::nullopt;
        std::string parameterName;
        if (!decodeString(ptr, remaining, parameterName)) return std::nullopt;
        float parameterValue;
        if (!decodeFloat32(ptr, remaining, parameterValue)) return std::nullopt;
        std::string displayValue;
        if (!decodeString(ptr, remaining, displayValue)) return std::nullopt;
        float parameterOrigin;
        if (!decodeFloat32(ptr, remaining, parameterOrigin)) return std::nullopt;
        bool parameterExists;
        if (!decodeBool(ptr, remaining, parameterExists)) return std::nullopt;
        uint8_t parameterType;
        if (!decodeUint8(ptr, remaining, parameterType)) return std::nullopt;
        int16_t discreteValueCount;
        if (!decodeInt16(ptr, remaining, discreteValueCount)) return std::nullopt;
        uint8_t currentValueIndex;
        if (!decodeUint8(ptr, remaining, currentValueIndex)) return std::nullopt;

        return DeviceMacroUpdateMessage{parameterIndex, parameterName, parameterValue, displayValue, parameterOrigin, parameterExists, parameterType, discreteValueCount, currentValueIndex};
    }


    /**
     * Convert message to YAML format for logging
     * 
     * WARNING: Uses static 32KB buffer - log immediately!
     * Multiple calls will overwrite previous results.
     * 
     * @return YAML string representation
     */
    const char* toString() const {
        #ifdef EXTMEM
        static EXTMEM char buffer[32768];  // Use external memory on Teensy
        #else
        static char buffer[32768];  // Standard static buffer
        #endif
        char* ptr = buffer;
        const char* end = buffer + sizeof(buffer) - 1;
        
        ptr += snprintf(ptr, end - ptr, "# DeviceMacroUpdate\ndeviceMacroUpdate:\n");
        
        ptr += snprintf(ptr, end - ptr, "  parameterIndex: %lu\n", (unsigned long)parameterIndex);
        ptr += snprintf(ptr, end - ptr, "  parameterName: \"%s\"\n", parameterName.c_str());
        {
            char floatBuf_parameterValue[16];
            floatToString(floatBuf_parameterValue, sizeof(floatBuf_parameterValue), parameterValue);
            ptr += snprintf(ptr, end - ptr, "  parameterValue: %s\n", floatBuf_parameterValue);
        }
        ptr += snprintf(ptr, end - ptr, "  displayValue: \"%s\"\n", displayValue.c_str());
        {
            char floatBuf_parameterOrigin[16];
            floatToString(floatBuf_parameterOrigin, sizeof(floatBuf_parameterOrigin), parameterOrigin);
            ptr += snprintf(ptr, end - ptr, "  parameterOrigin: %s\n", floatBuf_parameterOrigin);
        }
        ptr += snprintf(ptr, end - ptr, "  parameterExists: %s\n", parameterExists ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  parameterType: %lu\n", (unsigned long)parameterType);
        ptr += snprintf(ptr, end - ptr, "  discreteValueCount: %ld\n", (long)discreteValueCount);
        ptr += snprintf(ptr, end - ptr, "  currentValueIndex: %lu\n", (unsigned long)currentValueIndex);
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
