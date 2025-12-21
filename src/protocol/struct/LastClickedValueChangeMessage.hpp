/**
 * LastClickedValueChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: LAST_CLICKED_VALUE_CHANGE message
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
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace Protocol {



struct LastClickedValueChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::LAST_CLICKED_VALUE_CHANGE;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "LastClickedValueChange";

    float parameterValue;
    std::string displayValue;
    bool isEcho;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 61;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 29;

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

        encodeFloat32(ptr, parameterValue);
        encodeString(ptr, displayValue);
        encodeBool(ptr, isEcho);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<LastClickedValueChangeMessage> decode(
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
        float parameterValue;
        if (!decodeFloat32(ptr, remaining, parameterValue)) return std::nullopt;
        std::string displayValue;
        if (!decodeString(ptr, remaining, displayValue)) return std::nullopt;
        bool isEcho;
        if (!decodeBool(ptr, remaining, isEcho)) return std::nullopt;

        return LastClickedValueChangeMessage{parameterValue, displayValue, isEcho};
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

        ptr += snprintf(ptr, end - ptr, "# LastClickedValueChange\nlastClickedValueChange:\n");

        {
            char floatBuf_parameterValue[16];
            floatToString(floatBuf_parameterValue, sizeof(floatBuf_parameterValue), parameterValue);
            ptr += snprintf(ptr, end - ptr, "  parameterValue: %s\n", floatBuf_parameterValue);
        }
        ptr += snprintf(ptr, end - ptr, "  displayValue: \"%s\"\n", displayValue.c_str());
        ptr += snprintf(ptr, end - ptr, "  isEcho: %s\n", isEcho ? "true" : "false");

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
