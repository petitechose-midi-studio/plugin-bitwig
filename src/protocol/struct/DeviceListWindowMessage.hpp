/**
 * DeviceListWindowMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_LIST_WINDOW message
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
#include "../DeviceType.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {


#ifndef PROTOCOL_DEVICES_STRUCT
#define PROTOCOL_DEVICES_STRUCT

struct Devices {
    uint8_t deviceIndex;
    std::string deviceName;
    bool isEnabled;
    DeviceType deviceType;
    std::array<uint8_t, 4> childrenTypes;
};

#endif // PROTOCOL_DEVICES_STRUCT

struct DeviceListWindowMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_LIST_WINDOW;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceListWindow";

    uint8_t deviceCount;
    uint8_t deviceStartIndex;
    uint8_t deviceIndex;
    bool isNested;
    std::string parentName;
    std::array<Devices, 16> devices;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 711;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 23;

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

        Encoder::encodeUint8(ptr, deviceCount);
        Encoder::encodeUint8(ptr, deviceStartIndex);
        Encoder::encodeUint8(ptr, deviceIndex);
        Encoder::encodeBool(ptr, isNested);
        Encoder::encodeString(ptr, parentName);
        Encoder::encodeUint8(ptr, devices.size());
        for (const auto& item : devices) {
            Encoder::encodeUint8(ptr, item.deviceIndex);
            Encoder::encodeString(ptr, item.deviceName);
            Encoder::encodeBool(ptr, item.isEnabled);
            Encoder::encodeUint8(ptr, static_cast<uint8_t>(item.deviceType));
            Encoder::encodeUint8(ptr, item.childrenTypes.size());
            for (const auto& type : item.childrenTypes) {
                Encoder::encodeUint8(ptr, type);
            }
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
    static std::optional<DeviceListWindowMessage> decode(
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
        uint8_t deviceCount;
        if (!Decoder::decodeUint8(ptr, remaining, deviceCount)) return std::nullopt;
        uint8_t deviceStartIndex;
        if (!Decoder::decodeUint8(ptr, remaining, deviceStartIndex)) return std::nullopt;
        uint8_t deviceIndex;
        if (!Decoder::decodeUint8(ptr, remaining, deviceIndex)) return std::nullopt;
        bool isNested;
        if (!Decoder::decodeBool(ptr, remaining, isNested)) return std::nullopt;
        std::string parentName;
        if (!Decoder::decodeString(ptr, remaining, parentName)) return std::nullopt;
        uint8_t count_devices;
        if (!Decoder::decodeUint8(ptr, remaining, count_devices)) return std::nullopt;
        std::array<Devices, 16> devices_data;
        for (uint8_t i = 0; i < count_devices && i < 16; ++i) {
            Devices item;
            if (!Decoder::decodeUint8(ptr, remaining, item.deviceIndex)) return std::nullopt;
            if (!Decoder::decodeString(ptr, remaining, item.deviceName)) return std::nullopt;
            if (!Decoder::decodeBool(ptr, remaining, item.isEnabled)) return std::nullopt;
            uint8_t deviceType_raw;
            if (!Decoder::decodeUint8(ptr, remaining, deviceType_raw)) return std::nullopt;
            item.deviceType = static_cast<DeviceType>(deviceType_raw);
            uint8_t count_childrenTypes;
            if (!Decoder::decodeUint8(ptr, remaining, count_childrenTypes)) return std::nullopt;
            for (uint8_t j = 0; j < count_childrenTypes && j < 4; ++j) {
                if (!Decoder::decodeUint8(ptr, remaining, item.childrenTypes[j])) return std::nullopt;
            }
            devices_data[i] = item;
        }

        return DeviceListWindowMessage{deviceCount, deviceStartIndex, deviceIndex, isNested, parentName, devices_data};
    }

};

}  // namespace Protocol
