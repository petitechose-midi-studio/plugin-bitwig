/**
 * Protocol.hpp - Bitwig Protocol Implementation
 *
 * MANUAL - DO NOT REGENERATE
 *
 * Handles transport layer (ControllerAPI) only.
 * Message callbacks are defined in ProtocolCallbacks (generated).
 */

#pragma once

#include "DecoderRegistry.hpp"
#include "MessageID.hpp"
#include "ProtocolCallbacks.hpp"
#include "ProtocolConstants.hpp"

#include <cstdint>

#include "api/ControllerAPI.hpp"

namespace Protocol {

class Protocol : public ProtocolCallbacks {
public:
    /**
     * Construct Protocol with ControllerAPI
     * Automatically registers SysEx receive callback
     */
    explicit Protocol(ControllerAPI& api) : api_(api) {
        api_.onSysEx([this](const uint8_t* data, uint16_t length) { dispatch(data, length); });
    }

    /**
     * Send a protocol message (auto-detects MessageID from message type)
     *
     * Example:
     *   protocol.send(TransportPlayMessage{true});
     */
    template <typename T>
    void send(const T& message) {
        constexpr MessageID messageId = T::MESSAGE_ID;

        uint8_t payload[T::MAX_PAYLOAD_SIZE];
        uint16_t payloadLen = message.encode(payload, sizeof(payload));

        uint8_t sysex[MAX_MESSAGE_SIZE];

        uint16_t offset = 0;
        sysex[offset++] = SYSEX_START;
        sysex[offset++] = MANUFACTURER_ID;
        sysex[offset++] = DEVICE_ID;
        sysex[offset++] = static_cast<uint8_t>(messageId);
        sysex[offset++] = 0;  // fromHost flag (C++ = controller = 0)

        for (uint16_t i = 0; i < payloadLen; ++i) { sysex[offset++] = payload[i]; }

        sysex[offset++] = SYSEX_END;

        api_.sendSysEx(sysex, offset);
    }

    /**
     * Dispatch incoming SysEx to callbacks
     * Called automatically by SysEx receive callback
     */
    void dispatch(const uint8_t* sysex, uint16_t length) {
        if (sysex == nullptr || length < MIN_MESSAGE_LENGTH) { return; }

        if (sysex[0] != SYSEX_START || sysex[length - 1] != SYSEX_END) { return; }

        if (sysex[1] != MANUFACTURER_ID || sysex[2] != DEVICE_ID) { return; }

        MessageID messageId = static_cast<MessageID>(sysex[MESSAGE_TYPE_OFFSET]);
        bool fromHost = (sysex[FROM_HOST_OFFSET] != 0);

        uint16_t payloadLen = length - MIN_MESSAGE_LENGTH;
        const uint8_t* payload = sysex + PAYLOAD_OFFSET;

        DecoderRegistry::dispatch(*this, messageId, payload, payloadLen, fromHost);
    }
private:
    ControllerAPI& api_;
};

}  // namespace Protocol
