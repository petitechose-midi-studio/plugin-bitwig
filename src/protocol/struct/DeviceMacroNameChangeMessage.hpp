/**
 * DeviceMacroNameChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_MACRO_NAME_CHANGE message
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



struct DeviceMacroNameChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_MACRO_NAME_CHANGE;

    uint8_t parameterIndex;
    std::string parameterName;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 129;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 2;

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

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<DeviceMacroNameChangeMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return std::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t parameterIndex;
        if (!decodeUint8(ptr, remaining, parameterIndex)) return std::nullopt;
        std::string parameterName;
        if (!decodeString(ptr, remaining, parameterName)) return std::nullopt;

        return DeviceMacroNameChangeMessage{parameterIndex, parameterName};
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
        
        ptr += snprintf(ptr, end - ptr, "# DeviceMacroNameChange\ndeviceMacroNameChange:\n");
        
        ptr += snprintf(ptr, end - ptr, "  parameterIndex: %lu\n", (unsigned long)parameterIndex);
        ptr += snprintf(ptr, end - ptr, "  parameterName: \"%s\"\n", parameterName.c_str());
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
