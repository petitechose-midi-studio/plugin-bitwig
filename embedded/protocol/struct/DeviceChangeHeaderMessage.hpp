/**
 * DeviceChangeHeaderMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_CHANGE_HEADER message
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


#ifndef PROTOCOL_PAGEINFO_STRUCT
#define PROTOCOL_PAGEINFO_STRUCT

struct PageInfo {
    uint8_t devicePageIndex;
    uint8_t devicePageCount;
    etl::string<STRING_MAX_LENGTH> devicePageName;
};

#endif // PROTOCOL_PAGEINFO_STRUCT

struct DeviceChangeHeaderMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_CHANGE_HEADER;

    etl::string<STRING_MAX_LENGTH> deviceName;
    bool isEnabled;
    PageInfo pageInfo;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 37;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 5;

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

        encodeString(ptr, deviceName);
        encodeBool(ptr, isEnabled);
        encodeUint8(ptr, pageInfo.devicePageIndex);
        encodeUint8(ptr, pageInfo.devicePageCount);
        encodeString(ptr, pageInfo.devicePageName);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or etl::nullopt if invalid/insufficient data
     */
    static etl::optional<DeviceChangeHeaderMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return etl::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        etl::string<STRING_MAX_LENGTH> deviceName;
        if (!decodeString<STRING_MAX_LENGTH>(ptr, remaining, deviceName)) return etl::nullopt;
        bool isEnabled;
        if (!decodeBool(ptr, remaining, isEnabled)) return etl::nullopt;
        PageInfo pageInfo_data;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageIndex)) return etl::nullopt;
        if (!decodeUint8(ptr, remaining, pageInfo_data.devicePageCount)) return etl::nullopt;
        if (!decodeString<STRING_MAX_LENGTH>(ptr, remaining, pageInfo_data.devicePageName)) return etl::nullopt;

        return DeviceChangeHeaderMessage{deviceName, isEnabled, pageInfo_data};
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
        
        ptr += snprintf(ptr, end - ptr, "# DeviceChangeHeader\ndeviceChangeHeader:\n");
        
        ptr += snprintf(ptr, end - ptr, "  deviceName: \"%s\"\n", deviceName.c_str());
        ptr += snprintf(ptr, end - ptr, "  isEnabled: %s\n", isEnabled ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  pageInfo:\n");
        ptr += snprintf(ptr, end - ptr, "    devicePageIndex: %lu\n", (unsigned long)pageInfo.devicePageIndex);
        ptr += snprintf(ptr, end - ptr, "    devicePageCount: %lu\n", (unsigned long)pageInfo.devicePageCount);
        ptr += snprintf(ptr, end - ptr, "    devicePageName: \"%s\"\n", pageInfo.devicePageName.c_str());
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
