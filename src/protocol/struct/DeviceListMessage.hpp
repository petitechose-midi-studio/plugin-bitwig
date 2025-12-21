/**
 * DeviceListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_LIST message
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
#include <string>

namespace Protocol {


#ifndef PROTOCOL_DEVICES_STRUCT
#define PROTOCOL_DEVICES_STRUCT

struct Devices {
    uint8_t deviceIndex;
    std::string deviceName;
    bool isEnabled;
    uint8_t deviceType;
    std::array<uint8_t, 4> childrenTypes;
};

#endif // PROTOCOL_DEVICES_STRUCT

struct DeviceListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_LIST;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceList";

    uint8_t deviceCount;
    uint8_t deviceIndex;
    bool isNested;
    std::string parentName;
    std::array<Devices, 32> devices;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 1328;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 16;

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

        encodeUint8(ptr, deviceCount);
        encodeUint8(ptr, deviceIndex);
        encodeBool(ptr, isNested);
        encodeString(ptr, parentName);
        encodeUint8(ptr, devices.size());
        for (const auto& item : devices) {
            encodeUint8(ptr, item.deviceIndex);
            encodeString(ptr, item.deviceName);
            encodeBool(ptr, item.isEnabled);
            encodeUint8(ptr, item.deviceType);
            encodeUint8(ptr, item.childrenTypes.size());
            for (const auto& type : item.childrenTypes) {
                encodeUint8(ptr, type);
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
    static std::optional<DeviceListMessage> decode(
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
        uint8_t deviceCount;
        if (!decodeUint8(ptr, remaining, deviceCount)) return std::nullopt;
        uint8_t deviceIndex;
        if (!decodeUint8(ptr, remaining, deviceIndex)) return std::nullopt;
        bool isNested;
        if (!decodeBool(ptr, remaining, isNested)) return std::nullopt;
        std::string parentName;
        if (!decodeString(ptr, remaining, parentName)) return std::nullopt;
        uint8_t count_devices;
        if (!decodeUint8(ptr, remaining, count_devices)) return std::nullopt;
        std::array<Devices, 32> devices_data;
        for (uint8_t i = 0; i < count_devices && i < 32; ++i) {
            Devices item;
            if (!decodeUint8(ptr, remaining, item.deviceIndex)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.deviceName)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.isEnabled)) return std::nullopt;
            if (!decodeUint8(ptr, remaining, item.deviceType)) return std::nullopt;
            uint8_t count_childrenTypes;
            if (!decodeUint8(ptr, remaining, count_childrenTypes)) return std::nullopt;
            for (uint8_t j = 0; j < count_childrenTypes && j < 4; ++j) {
                if (!decodeUint8(ptr, remaining, item.childrenTypes[j])) return std::nullopt;
            }
            devices_data[i] = item;
        }

        return DeviceListMessage{deviceCount, deviceIndex, isNested, parentName, devices_data};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceList\ndeviceList:\n");

        ptr += snprintf(ptr, end - ptr, "  deviceCount: %lu\n", (unsigned long)deviceCount);
        ptr += snprintf(ptr, end - ptr, "  deviceIndex: %lu\n", (unsigned long)deviceIndex);
        ptr += snprintf(ptr, end - ptr, "  isNested: %s\n", isNested ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  parentName: \"%s\"\n", parentName.c_str());
        ptr += snprintf(ptr, end - ptr, "  devices:\n");
        for (size_t i = 0; i < devices.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - deviceIndex: %lu\n", (unsigned long)devices[i].deviceIndex);
        ptr += snprintf(ptr, end - ptr, "      deviceName: \"%s\"\n", devices[i].deviceName.c_str());
        ptr += snprintf(ptr, end - ptr, "      isEnabled: %s\n", devices[i].isEnabled ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      deviceType: %lu\n", (unsigned long)devices[i].deviceType);
        ptr += snprintf(ptr, end - ptr, "      childrenTypes:");
        if (devices[i].childrenTypes.size() == 0) {
            ptr += snprintf(ptr, end - ptr, " []\n");
        } else {
            ptr += snprintf(ptr, end - ptr, "\n");
            for (size_t j = 0; j < devices[i].childrenTypes.size(); ++j) {
                ptr += snprintf(ptr, end - ptr, "        - %lu\n", (unsigned long)devices[i].childrenTypes[j]);
            }
        }
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
