/**
 * DeviceListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_LIST message
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
#include <etl/optional.h>
#include <etl/vector.h>

namespace Protocol {


#ifndef PROTOCOL_DEVICES_STRUCT
#define PROTOCOL_DEVICES_STRUCT

struct Devices {
    uint8_t deviceIndex;
    etl::string<STRING_MAX_LENGTH> deviceName;
    bool isEnabled;
    etl::array<uint8_t, 4> childrenTypes;
};

#endif // PROTOCOL_DEVICES_STRUCT

struct DeviceListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_LIST;

    uint8_t deviceCount;
    uint8_t deviceIndex;
    bool isNested;
    etl::string<STRING_MAX_LENGTH> parentName;
    etl::array<Devices, 32> devices;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 757;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 229;

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

        encodeUint8(ptr, deviceCount);
        encodeUint8(ptr, deviceIndex);
        encodeBool(ptr, isNested);
        encodeString(ptr, parentName);
        encodeUint8(ptr, devices.size());
        for (const auto& item : devices) {
            encodeUint8(ptr, item.deviceIndex);
            encodeString(ptr, item.deviceName);
            encodeBool(ptr, item.isEnabled);
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
     * @return Decoded struct, or etl::nullopt if invalid/insufficient data
     */
    static etl::optional<DeviceListMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return etl::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t deviceCount;
        if (!decodeUint8(ptr, remaining, deviceCount)) return etl::nullopt;
        uint8_t deviceIndex;
        if (!decodeUint8(ptr, remaining, deviceIndex)) return etl::nullopt;
        bool isNested;
        if (!decodeBool(ptr, remaining, isNested)) return etl::nullopt;
        etl::string<STRING_MAX_LENGTH> parentName;
        if (!decodeString<STRING_MAX_LENGTH>(ptr, remaining, parentName)) return etl::nullopt;
        uint8_t count_devices;
        if (!decodeUint8(ptr, remaining, count_devices)) return etl::nullopt;
        etl::array<Devices, 32> devices_data;
        for (uint8_t i = 0; i < count_devices && i < 32; ++i) {
            Devices item;
            if (!decodeUint8(ptr, remaining, item.deviceIndex)) return etl::nullopt;
            if (!decodeString<STRING_MAX_LENGTH>(ptr, remaining, item.deviceName)) return etl::nullopt;
            if (!decodeBool(ptr, remaining, item.isEnabled)) return etl::nullopt;
            uint8_t count_childrenTypes;
            if (!decodeUint8(ptr, remaining, count_childrenTypes)) return etl::nullopt;
            for (uint8_t j = 0; j < count_childrenTypes && j < 4; ++j) {
                if (!decodeUint8(ptr, remaining, item.childrenTypes[j])) return etl::nullopt;
            }
            devices_data[i] = item;
        }

        return DeviceListMessage{deviceCount, deviceIndex, isNested, parentName, devices_data};
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
        return buffer;
    }

};

}  // namespace Protocol
