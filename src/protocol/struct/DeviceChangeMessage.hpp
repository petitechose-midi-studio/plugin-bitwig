/**
 * DeviceChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE message
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
        Encoder::encodeUint8(ptr, static_cast<uint8_t>(strlen(MESSAGE_NAME)));
        for (size_t i = 0; i < strlen(MESSAGE_NAME); ++i) {
            *ptr++ = static_cast<uint8_t>(MESSAGE_NAME[i]);
        }

        Encoder::encodeString(ptr, deviceTrackName);
        Encoder::encodeString(ptr, deviceName);
        Encoder::encodeBool(ptr, isEnabled);
        Encoder::encodeUint8(ptr, pageInfo.devicePageIndex);
        Encoder::encodeUint8(ptr, pageInfo.devicePageCount);
        Encoder::encodeString(ptr, pageInfo.devicePageName);
        Encoder::encodeUint8(ptr, remoteControls.size());
        for (const auto& item : remoteControls) {
            Encoder::encodeUint8(ptr, item.remoteControlIndex);
            Encoder::encodeFloat32(ptr, item.parameterValue);
            Encoder::encodeString(ptr, item.parameterName);
            Encoder::encodeFloat32(ptr, item.parameterOrigin);
            Encoder::encodeBool(ptr, item.parameterExists);
            Encoder::encodeInt16(ptr, item.discreteValueCount);
            Encoder::encodeString(ptr, item.displayValue);
            Encoder::encodeUint8(ptr, static_cast<uint8_t>(item.parameterType));
            Encoder::encodeUint8(ptr, item.discreteValueNames.size());
            for (const auto& type : item.discreteValueNames) {
                Encoder::encodeString(ptr, type);
            }
            Encoder::encodeUint8(ptr, item.currentValueIndex);
            Encoder::encodeBool(ptr, item.hasAutomation);
            Encoder::encodeFloat32(ptr, item.modulatedValue);
            Encoder::encodeBool(ptr, item.isModulated);
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

        // Skip MESSAGE_NAME prefix
        uint8_t nameLen;
        if (!Decoder::decodeUint8(ptr, remaining, nameLen)) return std::nullopt;
        ptr += nameLen;
        remaining -= nameLen;

        // Decode fields
        std::string deviceTrackName;
        if (!Decoder::decodeString(ptr, remaining, deviceTrackName)) return std::nullopt;
        std::string deviceName;
        if (!Decoder::decodeString(ptr, remaining, deviceName)) return std::nullopt;
        bool isEnabled;
        if (!Decoder::decodeBool(ptr, remaining, isEnabled)) return std::nullopt;
        PageInfo pageInfo_data;
        if (!Decoder::decodeUint8(ptr, remaining, pageInfo_data.devicePageIndex)) return std::nullopt;
        if (!Decoder::decodeUint8(ptr, remaining, pageInfo_data.devicePageCount)) return std::nullopt;
        if (!Decoder::decodeString(ptr, remaining, pageInfo_data.devicePageName)) return std::nullopt;
        uint8_t count_remoteControls;
        if (!Decoder::decodeUint8(ptr, remaining, count_remoteControls)) return std::nullopt;
        std::array<RemoteControls, 8> remoteControls_data;
        for (uint8_t i = 0; i < count_remoteControls && i < 8; ++i) {
            RemoteControls item;
            if (!Decoder::decodeUint8(ptr, remaining, item.remoteControlIndex)) return std::nullopt;
            if (!Decoder::decodeFloat32(ptr, remaining, item.parameterValue)) return std::nullopt;
            if (!Decoder::decodeString(ptr, remaining, item.parameterName)) return std::nullopt;
            if (!Decoder::decodeFloat32(ptr, remaining, item.parameterOrigin)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.parameterExists)) return std::nullopt;
            if (!Decoder::decodeInt16(ptr, remaining, item.discreteValueCount)) return std::nullopt;
            if (!Decoder::decodeString(ptr, remaining, item.displayValue)) return std::nullopt;
            uint8_t parameterType_raw;
            if (!Decoder::decodeUint8(ptr, remaining, parameterType_raw)) return std::nullopt;
            item.parameterType = static_cast<ParameterType>(parameterType_raw);
            uint8_t count_discreteValueNames;
            if (!Decoder::decodeUint8(ptr, remaining, count_discreteValueNames)) return std::nullopt;
            for (uint8_t j = 0; j < count_discreteValueNames && j < 32; ++j) {
                std::string temp_discreteValueNames;
                if (!Decoder::decodeString(ptr, remaining, temp_discreteValueNames)) return std::nullopt;
                item.discreteValueNames.push_back(temp_discreteValueNames);
            }
            if (!Decoder::decodeUint8(ptr, remaining, item.currentValueIndex)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.hasAutomation)) return std::nullopt;
            if (!Decoder::decodeFloat32(ptr, remaining, item.modulatedValue)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isModulated)) return std::nullopt;
            remoteControls_data[i] = item;
        }

        return DeviceChangeMessage{deviceTrackName, deviceName, isEnabled, pageInfo_data, remoteControls_data};
    }

};

}  // namespace Protocol
