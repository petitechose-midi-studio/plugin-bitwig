/**
 * TrackSendListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: TRACK_SEND_LIST message
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


#ifndef PROTOCOL_SENDS_STRUCT
#define PROTOCOL_SENDS_STRUCT

struct Sends {
    uint8_t sendIndex;
    std::string sendName;
    uint32_t color;
    float sendValue;
    std::string sendDisplayValue;
    bool sendIsEnabled;
    std::string sendMode;
    bool sendIsPreFader;
};

#endif // PROTOCOL_SENDS_STRUCT

struct TrackSendListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::TRACK_SEND_LIST;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "TrackSendList";

    uint8_t trackIndex;
    uint8_t sendCount;
    std::array<Sends, 8> sends;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 897;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 17;

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

        encodeUint8(ptr, trackIndex);
        encodeUint8(ptr, sendCount);
        encodeUint8(ptr, sends.size());
        for (const auto& item : sends) {
            encodeUint8(ptr, item.sendIndex);
            encodeString(ptr, item.sendName);
            encodeUint32(ptr, item.color);
            encodeFloat32(ptr, item.sendValue);
            encodeString(ptr, item.sendDisplayValue);
            encodeBool(ptr, item.sendIsEnabled);
            encodeString(ptr, item.sendMode);
            encodeBool(ptr, item.sendIsPreFader);
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
    static std::optional<TrackSendListMessage> decode(
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
        uint8_t trackIndex;
        if (!decodeUint8(ptr, remaining, trackIndex)) return std::nullopt;
        uint8_t sendCount;
        if (!decodeUint8(ptr, remaining, sendCount)) return std::nullopt;
        uint8_t count_sends;
        if (!decodeUint8(ptr, remaining, count_sends)) return std::nullopt;
        std::array<Sends, 8> sends_data;
        for (uint8_t i = 0; i < count_sends && i < 8; ++i) {
            Sends item;
            if (!decodeUint8(ptr, remaining, item.sendIndex)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.sendName)) return std::nullopt;
            if (!decodeUint32(ptr, remaining, item.color)) return std::nullopt;
            if (!decodeFloat32(ptr, remaining, item.sendValue)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.sendDisplayValue)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.sendIsEnabled)) return std::nullopt;
            if (!decodeString(ptr, remaining, item.sendMode)) return std::nullopt;
            if (!decodeBool(ptr, remaining, item.sendIsPreFader)) return std::nullopt;
            sends_data[i] = item;
        }

        return TrackSendListMessage{trackIndex, sendCount, sends_data};
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

        ptr += snprintf(ptr, end - ptr, "# TrackSendList\ntrackSendList:\n");

        ptr += snprintf(ptr, end - ptr, "  trackIndex: %lu\n", (unsigned long)trackIndex);
        ptr += snprintf(ptr, end - ptr, "  sendCount: %lu\n", (unsigned long)sendCount);
        ptr += snprintf(ptr, end - ptr, "  sends:\n");
        for (size_t i = 0; i < sends.size(); ++i) {
            ptr += snprintf(ptr, end - ptr, "    - sendIndex: %lu\n", (unsigned long)sends[i].sendIndex);
        ptr += snprintf(ptr, end - ptr, "      sendName: \"%s\"\n", sends[i].sendName.c_str());
        ptr += snprintf(ptr, end - ptr, "      color: %lu\n", (unsigned long)sends[i].color);
        {
            char floatBuf_sends_i_sendValue[16];
            floatToString(floatBuf_sends_i_sendValue, sizeof(floatBuf_sends_i_sendValue), sends[i].sendValue);
            ptr += snprintf(ptr, end - ptr, "      sendValue: %s\n", floatBuf_sends_i_sendValue);
        }
        ptr += snprintf(ptr, end - ptr, "      sendDisplayValue: \"%s\"\n", sends[i].sendDisplayValue.c_str());
        ptr += snprintf(ptr, end - ptr, "      sendIsEnabled: %s\n", sends[i].sendIsEnabled ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "      sendMode: \"%s\"\n", sends[i].sendMode.c_str());
        ptr += snprintf(ptr, end - ptr, "      sendIsPreFader: %s\n", sends[i].sendIsPreFader ? "true" : "false");
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
