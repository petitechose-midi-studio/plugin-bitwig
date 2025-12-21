/**
 * DeviceRemoteControlUpdateMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: DEVICE_REMOTE_CONTROL_UPDATE message
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



struct DeviceRemoteControlUpdateMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::DEVICE_REMOTE_CONTROL_UPDATE;

    // Message name for logging (encoded in payload)
    static constexpr const char* MESSAGE_NAME = "DeviceRemoteControlUpdate";

    uint8_t remoteControlIndex;
    std::string parameterName;
    float parameterValue;
    std::string displayValue;
    float parameterOrigin;
    bool parameterExists;
    uint8_t parameterType;
    int16_t discreteValueCount;
    uint8_t currentValueIndex;
    bool hasAutomation;
    float modulatedValue;

    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 111;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 47;

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
        encodeString(ptr, parameterName);
        encodeFloat32(ptr, parameterValue);
        encodeString(ptr, displayValue);
        encodeFloat32(ptr, parameterOrigin);
        encodeBool(ptr, parameterExists);
        encodeUint8(ptr, parameterType);
        encodeInt16(ptr, discreteValueCount);
        encodeUint8(ptr, currentValueIndex);
        encodeBool(ptr, hasAutomation);
        encodeFloat32(ptr, modulatedValue);

        return ptr - buffer;
    }

    /**
     * Decode struct from MIDI-safe bytes
     *
     * @param data Input buffer with encoded data
     * @param len Length of input buffer
     * @return Decoded struct, or std::nullopt if invalid/insufficient data
     */
    static std::optional<DeviceRemoteControlUpdateMessage> decode(
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
        std::string parameterName;
        if (!decodeString(ptr, remaining, parameterName)) return std::nullopt;
        float parameterValue;
        if (!decodeFloat32(ptr, remaining, parameterValue)) return std::nullopt;
        std::string displayValue;
        if (!decodeString(ptr, remaining, displayValue)) return std::nullopt;
        float parameterOrigin;
        if (!decodeFloat32(ptr, remaining, parameterOrigin)) return std::nullopt;
        bool parameterExists;
        if (!decodeBool(ptr, remaining, parameterExists)) return std::nullopt;
        uint8_t parameterType;
        if (!decodeUint8(ptr, remaining, parameterType)) return std::nullopt;
        int16_t discreteValueCount;
        if (!decodeInt16(ptr, remaining, discreteValueCount)) return std::nullopt;
        uint8_t currentValueIndex;
        if (!decodeUint8(ptr, remaining, currentValueIndex)) return std::nullopt;
        bool hasAutomation;
        if (!decodeBool(ptr, remaining, hasAutomation)) return std::nullopt;
        float modulatedValue;
        if (!decodeFloat32(ptr, remaining, modulatedValue)) return std::nullopt;

        return DeviceRemoteControlUpdateMessage{remoteControlIndex, parameterName, parameterValue, displayValue, parameterOrigin, parameterExists, parameterType, discreteValueCount, currentValueIndex, hasAutomation, modulatedValue};
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

        ptr += snprintf(ptr, end - ptr, "# DeviceRemoteControlUpdate\ndeviceRemoteControlUpdate:\n");

        ptr += snprintf(ptr, end - ptr, "  remoteControlIndex: %lu\n", (unsigned long)remoteControlIndex);
        ptr += snprintf(ptr, end - ptr, "  parameterName: \"%s\"\n", parameterName.c_str());
        {
            char floatBuf_parameterValue[16];
            floatToString(floatBuf_parameterValue, sizeof(floatBuf_parameterValue), parameterValue);
            ptr += snprintf(ptr, end - ptr, "  parameterValue: %s\n", floatBuf_parameterValue);
        }
        ptr += snprintf(ptr, end - ptr, "  displayValue: \"%s\"\n", displayValue.c_str());
        {
            char floatBuf_parameterOrigin[16];
            floatToString(floatBuf_parameterOrigin, sizeof(floatBuf_parameterOrigin), parameterOrigin);
            ptr += snprintf(ptr, end - ptr, "  parameterOrigin: %s\n", floatBuf_parameterOrigin);
        }
        ptr += snprintf(ptr, end - ptr, "  parameterExists: %s\n", parameterExists ? "true" : "false");
        ptr += snprintf(ptr, end - ptr, "  parameterType: %lu\n", (unsigned long)parameterType);
        ptr += snprintf(ptr, end - ptr, "  discreteValueCount: %ld\n", (long)discreteValueCount);
        ptr += snprintf(ptr, end - ptr, "  currentValueIndex: %lu\n", (unsigned long)currentValueIndex);
        ptr += snprintf(ptr, end - ptr, "  hasAutomation: %s\n", hasAutomation ? "true" : "false");
        {
            char floatBuf_modulatedValue[16];
            floatToString(floatBuf_modulatedValue, sizeof(floatBuf_modulatedValue), modulatedValue);
            ptr += snprintf(ptr, end - ptr, "  modulatedValue: %s\n", floatBuf_modulatedValue);
        }

        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
