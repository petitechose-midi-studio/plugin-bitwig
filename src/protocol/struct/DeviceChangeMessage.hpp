/**
 * DeviceChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE message
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
#include "../ParameterType.hpp"
#include <array>
#include <cstdint>
#include <cstring>
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

#ifndef PROTOCOL_REMOTECONTROLS_STRUCT
#define PROTOCOL_REMOTECONTROLS_STRUCT

struct RemoteControls {
    uint8_t remoteControlIndex;
    float parameterValue;
    std::string parameterName;
    float parameterOrigin;
    bool parameterExists;
    int16_t discreteValueCount;
    std::string displayValue;
    ParameterType parameterType;
    std::vector<std::string> discreteValueNames;
    uint8_t currentValueIndex;
    bool hasAutomation;
    float modulatedValue;
    bool isModulated;
};

#endif // PROTOCOL_REMOTECONTROLS_STRUCT

struct DeviceChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_CHANGE;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceChange";

    std::string deviceTrackName;
    std::string deviceName;
    bool isEnabled;
    PageInfo pageInfo;
    std::array<RemoteControls, 8> remoteControls;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 9260;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 20;

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

        encodeString(ptr, deviceTrackName);
        encodeString(ptr, deviceName);
        encodeBool(ptr, isEnabled);
        encodeUint8(ptr, pageInfo.devicePageIndex);
        encodeUint8(ptr, pageInfo.devicePageCount);
        encodeString(ptr, pageInfo.devicePageName);
        encodeUint8(ptr, remoteControls.size());
        for (const auto& item : remoteControls) {
            encodeUint8(ptr, item.remoteControlIndex);
            encodeFloat32(ptr, item.parameterValue);
            encodeString(ptr, item.parameterName);
            encodeFloat32(ptr, item.parameterOrigin);
            encodeBool(ptr, item.parameterExists);
            encodeInt16(ptr, item.discreteValueCount);
            encodeString(ptr, item.displayValue);
            encodeUint8(ptr, static_cast<uint8_t>(item.parameterType));
            encodeUint8(ptr, item.discreteValueNames.size());
            for (const auto& type : item.discreteValueNames) {
                encodeString(ptr, type);
            }
            encodeUint8(ptr, item.currentValueIndex);
            encodeBool(ptr, item.hasAutomation);
            encodeFloat32(ptr, item.modulatedValue);
            encodeBool(ptr, item.isModulated);
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
    static std::optional<DeviceChangeMessage> decode(
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
        std::string deviceTrackName;
        if (!decodeString(ptr, remaining, deviceTrackName)) return std::nullopt;
        std::string deviceName;
        if (!decodeString(ptr, remaining, deviceName)) return std::nullopt;
        bool isEnabled;
        if (!decodeBool(ptr, remaining, isEnabled)) return std::nullopt;
        PageInfo pageInfo_data;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageIndex)) return std::nullopt;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageCount)) return std::nullopt;
        if (!decodeString(ptr, remaining, pageInfo_data.devicePageName)) return std::nullopt;
        uint8_t count_remoteControls;
        if (!decodeUint8(ptr, remaining, count_remoteControls)) return std::nullopt;
        std::array<RemoteControls, 8> remoteControls_data;
        for (uint8_t i = 0; i < count_remoteControls && i < 8; ++i) {
            RemoteControls item;
            if (!decodeUint8(ptr, remaining, item.remoteControlIndex)) return std::nullopt;
            if (!decodeFloat32(ptr, remaining, item.parameterValue)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.parameterName)) return std::nullopt;
            if (!decodeFloat32(ptr, remaining, item.parameterOrigin)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.parameterExists)) return std::nullopt;
            if (!decodeInt16(ptr, remaining, item.discreteValueCount)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.displayValue)) return std::nullopt;
            uint8_t parameterType_raw;
            if (!decodeUint8(ptr, remaining, parameterType_raw)) return std::nullopt;
            item.parameterType = static_cast<ParameterType>(parameterType_raw);
            uint8_t count_discreteValueNames;
            if (!decodeUint8(ptr, remaining, count_discreteValueNames)) return std::nullopt;
            for (uint8_t j = 0; j < count_discreteValueNames && j < 32; ++j) {
                std::string temp_discreteValueNames;
                if (!decodeString(ptr, remaining, temp_discreteValueNames)) return std::nullopt;
                item.discreteValueNames.push_back(temp_discreteValueNames);
            }
            if (!decodeUint8(ptr, remaining, item.currentValueIndex)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.hasAutomation)) return std::nullopt;
            if (!decodeFloat32(ptr, remaining, item.modulatedValue)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.isModulated)) return std::nullopt;
            remoteControls_data[i] = item;
        }

        return DeviceChangeMessage{deviceTrackName, deviceName, isEnabled, pageInfo_data, remoteControls_data};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceChange\ndeviceChange:\n");

        ptr += snprintf(ptr, end - ptr, "  deviceTrackName: \"%s\"\n", deviceTrackName.c_str());
        ptr += snprintf(ptr, end - ptr, "  deviceName: \"%s\"\n", deviceName.c_str());
        ptr += snprintf(ptr, end - ptr, "  isEnabled: %s\n", isEnabled ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  pageInfo:\n");
        ptr += snprintf(ptr, end - ptr, "    devicePageIndex: %lu\n", (unsigned long)pageInfo.devicePageIndex);
        ptr += snprintf(ptr, end - ptr, "    devicePageCount: %lu\n", (unsigned long)pageInfo.devicePageCount);
        ptr += snprintf(ptr, end - ptr, "    devicePageName: \"%s\"\n", pageInfo.devicePageName.c_str());
        ptr += snprintf(ptr, end - ptr, "  remoteControls:\n");
        for (size_t i = 0; i < remoteControls.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - remoteControlIndex: %lu\n", (unsigned long)remoteControls[i].remoteControlIndex);
        {
            char floatBuf_remoteControls_i_parameterValue[16];
            floatToString(floatBuf_remoteControls_i_parameterValue, sizeof(floatBuf_remoteControls_i_parameterValue), remoteControls[i].parameterValue);
            ptr += snprintf(ptr, end - ptr, "      parameterValue: %s\n", floatBuf_remoteControls_i_parameterValue);
        }
        ptr += snprintf(ptr, end - ptr, "      parameterName: \"%s\"\n", remoteControls[i].parameterName.c_str());
        {
            char floatBuf_remoteControls_i_parameterOrigin[16];
            floatToString(floatBuf_remoteControls_i_parameterOrigin, sizeof(floatBuf_remoteControls_i_parameterOrigin), remoteControls[i].parameterOrigin);
            ptr += snprintf(ptr, end - ptr, "      parameterOrigin: %s\n", floatBuf_remoteControls_i_parameterOrigin);
        }
        ptr += snprintf(ptr, end - ptr, "      parameterExists: %s\n", remoteControls[i].parameterExists ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      discreteValueCount: %ld\n", (long)remoteControls[i].discreteValueCount);
        ptr += snprintf(ptr, end - ptr, "      displayValue: \"%s\"\n", remoteControls[i].displayValue.c_str());
        ptr += snprintf(ptr, end - ptr, "      parameterType: %d\n", static_cast<int>(remoteControls[i].parameterType));
        ptr += snprintf(ptr, end - ptr, "      discreteValueNames:");
        if (remoteControls[i].discreteValueNames.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            for (size_t j = 0; j < remoteControls[i].discreteValueNames.size(); ++j) {
                ptr += snprintf(ptr, end - ptr, "        - \"%s\"\n", remoteControls[i].discreteValueNames[j].c_str());
            }
        }
        ptr += snprintf(ptr, end - ptr, "      currentValueIndex: %lu\n", (unsigned long)remoteControls[i].currentValueIndex);
        ptr += snprintf(ptr, end - ptr, "      hasAutomation: %s\n", remoteControls[i].hasAutomation ? "true" : "false");
        {
            char floatBuf_remoteControls_i_modulatedValue[16];
            floatToString(floatBuf_remoteControls_i_modulatedValue, sizeof(floatBuf_remoteControls_i_modulatedValue), remoteControls[i].modulatedValue);
            ptr += snprintf(ptr, end - ptr, "      modulatedValue: %s\n", floatBuf_remoteControls_i_modulatedValue);
        }
        ptr += snprintf(ptr, end - ptr, "      isModulated: %s\n", remoteControls[i].isModulated ? "true" : "false");
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
