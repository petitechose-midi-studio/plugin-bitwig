/**
 * SendDestinationsListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: SEND_DESTINATIONS_LIST message
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


#ifndef PROTOCOL_SENDDESTINATIONS_STRUCT
#define PROTOCOL_SENDDESTINATIONS_STRUCT

struct SendDestinations {
    uint8_t sendIndex;
    std::string sendDestinationName;
};

#endif // PROTOCOL_SENDDESTINATIONS_STRUCT

struct SendDestinationsListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::SEND_DESTINATIONS_LIST;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "SendDestinationsList";

    uint8_t sendCount;
    std::array<SendDestinations, 8> sendDestinations;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 295;

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
        encodeUint8(ptr, static_cast<uint8_t>(strlen(MESSAGE_NAME)));
        for (size_t i = 0; i < strlen(MESSAGE_NAME); ++i) {
            *ptr++ = static_cast<uint8_t>(MESSAGE_NAME[i]);
        }

        encodeUint8(ptr, sendCount);
        encodeUint8(ptr, sendDestinations.size());
        for (const auto& item : sendDestinations) {
            encodeUint8(ptr, item.sendIndex);
            encodeString(ptr, item.sendDestinationName);
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
    static std::optional<SendDestinationsListMessage> decode(
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
        uint8_t sendCount;
        if (!decodeUint8(ptr, remaining, sendCount)) return std::nullopt;
        uint8_t count_sendDestinations;
        if (!decodeUint8(ptr, remaining, count_sendDestinations)) return std::nullopt;
        std::array<SendDestinations, 8> sendDestinations_data;
        for (uint8_t i = 0; i < count_sendDestinations && i < 8; ++i) {
            SendDestinations item;
            if (!decodeUint8(ptr, remaining, item.sendIndex)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.sendDestinationName)) return std::nullopt;
            sendDestinations_data[i] = item;
        }

        return SendDestinationsListMessage{sendCount, sendDestinations_data};
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

        ptr += snprintf(ptr, end - ptr, "# SendDestinationsList\nsendDestinationsList:\n");

        ptr += snprintf(ptr, end - ptr, "  sendCount: %lu\n", (unsigned long)sendCount);
        ptr += snprintf(ptr, end - ptr, "  sendDestinations:\n");
        for (size_t i = 0; i < sendDestinations.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - sendIndex: %lu\n", (unsigned long)sendDestinations[i].sendIndex);
        ptr += snprintf(ptr, end - ptr, "      sendDestinationName: \"%s\"\n", sendDestinations[i].sendDestinationName.c_str());
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
