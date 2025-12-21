/**
 * DeviceRemoteControlHasAutomationChangeMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE message
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

namespace Protocol {



struct DeviceRemoteControlHasAutomationChangeMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceRemoteControlHasAutomationChange";

    uint8_t remoteControlIndex;
    bool hasAutomation;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 41;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 41;

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

        encodeUint8(ptr, remoteControlIndex);
        encodeBool(ptr, hasAutomation);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<DeviceRemoteControlHasAutomationChangeMessage> decode(
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
        uint8_t remoteControlIndex;
        if (!decodeUint8(ptr, remaining, remoteControlIndex)) return std::nullopt;
        bool hasAutomation;
        if (!decodeBool(ptr, remaining, hasAutomation)) return std::nullopt;

        return DeviceRemoteControlHasAutomationChangeMessage{remoteControlIndex, hasAutomation};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceRemoteControlHasAutomationChange\ndeviceRemoteControlHasAutomationChange:\n");

        ptr += snprintf(ptr, end - ptr, "  remoteControlIndex: %lu\n", (unsigned long)remoteControlIndex);
        ptr += snprintf(ptr, end - ptr, "  hasAutomation: %s\n", hasAutomation ? "true" : "false");

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
