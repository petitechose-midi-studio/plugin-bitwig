/**
 * EnterDeviceChildMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: ENTER_DEVICE_CHILD message
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



struct EnterDeviceChildMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::ENTER_DEVICE_CHILD;

    uint8_t deviceIndex;
    uint8_t childType;
    uint8_t childIndex;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (7-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 3;

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

        encodeUint8(ptr, deviceIndex);
        encodeUint8(ptr, childType);
        encodeUint8(ptr, childIndex);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or etl::nullopt if invalid/insufficient data
     */
    static etl::optional<EnterDeviceChildMessage> decode(
        const uint8_t* data, uint16_t len) {

        if (len < MIN_PAYLOAD_SIZE) return etl::nullopt;

        const uint8_t* ptr = data;
        size_t remaining = len;

        // Decode fields
        uint8_t deviceIndex;
        if (!decodeUint8(ptr, remaining, deviceIndex)) return etl::nullopt;
        uint8_t childType;
        if (!decodeUint8(ptr, remaining, childType)) return etl::nullopt;
        uint8_t childIndex;
        if (!decodeUint8(ptr, remaining, childIndex)) return etl::nullopt;

        return EnterDeviceChildMessage{deviceIndex, childType, childIndex};
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
        
        ptr += snprintf(ptr, end - ptr, "# EnterDeviceChild\nenterDeviceChild:\n");
        
        ptr += snprintf(ptr, end - ptr, "  deviceIndex: %lu\n", (unsigned long)deviceIndex);
        ptr += snprintf(ptr, end - ptr, "  childType: %lu\n", (unsigned long)childType);
        ptr += snprintf(ptr, end - ptr, "  childIndex: %lu\n", (unsigned long)childIndex);
        
        *ptr = '\0';
        return buffer;
    }

};

}  // namespace Protocol
