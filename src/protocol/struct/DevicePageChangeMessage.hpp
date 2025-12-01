/**
 * DevicePageChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_PAGE_CHANGE message
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


#ifndef PROTOCOL_PAGEINFO_STRUCT
#define PROTOCOL_PAGEINFO_STRUCT

struct PageInfo {
    uint8_t devicePageIndex;
    uint8_t devicePageCount;
    std::string devicePageName;
};

#endif // PROTOCOL_PAGEINFO_STRUCT

#ifndef PROTOCOL_MACROS_STRUCT
#define PROTOCOL_MACROS_STRUCT

struct Macros {
    uint8_t parameterIndex;
    float parameterValue;
    std::string parameterName;
    float parameterOrigin;
    bool parameterExists;
    int16_t discreteValueCount;
    std::string displayValue;
    uint8_t parameterType;
    std::vector<std::string> discreteValueNames;
    uint8_t currentValueIndex;
};

#endif // PROTOCOL_MACROS_STRUCT

struct DevicePageChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_PAGE_CHANGE;

    PageInfo pageInfo;
    std::array<Macros, 8> macros;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 9148;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 412;

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

        encodeUint8(ptr, pageInfo.devicePageIndex);
        encodeUint8(ptr, pageInfo.devicePageCount);
        encodeString(ptr, pageInfo.devicePageName);
        encodeUint8(ptr, macros.size());
        for (const auto& item : macros) {
            encodeUint8(ptr, item.parameterIndex);
            encodeFloat32(ptr, item.parameterValue);
            encodeString(ptr, item.parameterName);
            encodeFloat32(ptr, item.parameterOrigin);
            encodeBool(ptr, item.parameterExists);
            encodeInt16(ptr, item.discreteValueCount);
            encodeString(ptr, item.displayValue);
            encodeUint8(ptr, item.parameterType);
            encodeUint8(ptr, item.discreteValueNames.size());
            for (const auto& type : item.discreteValueNames) {
                encodeString(ptr, type);
            }
            encodeUint8(ptr, item.currentValueIndex);
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
    static std::optional<DevicePageChangeMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        PageInfo pageInfo_data;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageIndex)) return std::nullopt;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageCount)) return std::nullopt;
        if (!decodeString(ptr, remaining, pageInfo_data.devicePageName)) return std::nullopt;
        uint8_t count_macros;
        if (!decodeUint8(ptr, remaining, count_macros)) return std::nullopt;
        std::array<Macros, 8> macros_data;
        for (uint8_t i = 0; i < count_macros && i < 8; ++i) {
            Macros item;
            if (!decodeUint8(ptr, remaining, item.parameterIndex)) return std::nullopt;
            if (!decodeFloat32(ptr, remaining, item.parameterValue)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.parameterName)) return std::nullopt;
            if (!decodeFloat32(ptr, remaining, item.parameterOrigin)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.parameterExists)) return std::nullopt;
            if (!decodeInt16(ptr, remaining, item.discreteValueCount)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.displayValue)) return std::nullopt;
            if (!decodeUint8(ptr, remaining, item.parameterType)) return std::nullopt;
            uint8_t count_discreteValueNames;
            if (!decodeUint8(ptr, remaining, count_discreteValueNames)) return std::nullopt;
            for (uint8_t j = 0; j < count_discreteValueNames && j < 32; ++j) {
                std::string temp_discreteValueNames;
                if (!decodeString(ptr, remaining, temp_discreteValueNames)) return std::nullopt;
                item.discreteValueNames.push_back(temp_discreteValueNames);
            }
            if (!decodeUint8(ptr, remaining, item.currentValueIndex)) return std::nullopt;
            macros_data[i] = item;
        }

        return DevicePageChangeMessage{pageInfo_data, macros_data};
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
        
        ptr += snprintf(ptr, end - ptr, "# DevicePageChange\ndevicePageChange:\n");
        
        ptr += snprintf(ptr, end - ptr, "  pageInfo:\n");
        ptr += snprintf(ptr, end - ptr, "    devicePageIndex: %lu\n", (unsigned long)pageInfo.devicePageIndex);
        ptr += snprintf(ptr, end - ptr, "    devicePageCount: %lu\n", (unsigned long)pageInfo.devicePageCount);
        ptr += snprintf(ptr, end - ptr, "    devicePageName: \"%s\"\n", pageInfo.devicePageName.c_str());
        ptr += snprintf(ptr, end - ptr, "  macros:\n");
        for (size_t i = 0; i < macros.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - parameterIndex: %lu\n", (unsigned long)macros[i].parameterIndex);
        {
            char floatBuf_macros_i_parameterValue[16];
            floatToString(floatBuf_macros_i_parameterValue, sizeof(floatBuf_macros_i_parameterValue), macros[i].parameterValue);
            ptr += snprintf(ptr, end - ptr, "      parameterValue: %s\n", floatBuf_macros_i_parameterValue);
        }
        ptr += snprintf(ptr, end - ptr, "      parameterName: \"%s\"\n", macros[i].parameterName.c_str());
        {
            char floatBuf_macros_i_parameterOrigin[16];
            floatToString(floatBuf_macros_i_parameterOrigin, sizeof(floatBuf_macros_i_parameterOrigin), macros[i].parameterOrigin);
            ptr += snprintf(ptr, end - ptr, "      parameterOrigin: %s\n", floatBuf_macros_i_parameterOrigin);
        }
        ptr += snprintf(ptr, end - ptr, "      parameterExists: %s\n", macros[i].parameterExists ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      discreteValueCount: %ld\n", (long)macros[i].discreteValueCount);
        ptr += snprintf(ptr, end - ptr, "      displayValue: \"%s\"\n", macros[i].displayValue.c_str());
        ptr += snprintf(ptr, end - ptr, "      parameterType: %lu\n", (unsigned long)macros[i].parameterType);
        ptr += snprintf(ptr, end - ptr, "      discreteValueNames:");
        if (macros[i].discreteValueNames.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            for (size_t j = 0; j < macros[i].discreteValueNames.size(); ++j) {
                ptr += snprintf(ptr, end - ptr, "        - \"%s\"\n", macros[i].discreteValueNames[j].c_str());
            }
        }
        ptr += snprintf(ptr, end - ptr, "      currentValueIndex: %lu\n", (unsigned long)macros[i].currentValueIndex);
        }
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
