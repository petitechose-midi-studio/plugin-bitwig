#pragma once

/**
 * @file BitwigProtocol.hpp
 * @brief Bitwig protocol wrapper for open-control framework (Serial8)
 *
 * Uses ISerialTransport for USB Serial communication with COBS framing.
 * The transport layer handles COBS encoding/decoding internally.
 *
 * ## Usage
 *
 * ```cpp
 * // In BitwigContext
 * BitwigProtocol protocol(serial());
 *
 * // Send messages
 * protocol.send(TransportPlayMessage{true});
 * protocol.send(DeviceSelectByIndexMessage{index});
 *
 * // Register callbacks (inherited from ProtocolCallbacks)
 * protocol.onDeviceChange = [this](const DeviceChangeMessage& msg) {
 *     state_.device.name.set(msg.deviceName);
 * };
 * ```
 */

#include <cstdint>
#include <cstring>

#include <oc/hal/ISerialTransport.hpp>
#include <oc/log/Log.hpp>

#include "DecoderRegistry.hpp"
#include "MessageID.hpp"
#include "ProtocolCallbacks.hpp"
#include "ProtocolConstants.hpp"

namespace bitwig {

/**
 * @brief Bitwig Serial8 protocol handler using open-control framework
 *
 * Inherits from ProtocolCallbacks for message callbacks.
 * Uses ISerialTransport for COBS-framed serial communication.
 */
class BitwigProtocol : public Protocol::ProtocolCallbacks {
public:
    /**
     * @brief Construct protocol with ISerialTransport
     *
     * Registers receive callback with transport for automatic dispatch.
     *
     * @param transport Reference to ISerialTransport (must outlive Protocol)
     */
    explicit BitwigProtocol(oc::hal::ISerialTransport& transport)
        : transport_(transport) {
        transport_.setOnReceive([this](const uint8_t* data, size_t len) {
            dispatch(data, len);
        });
    }

    ~BitwigProtocol() = default;

    // Non-copyable, non-movable
    BitwigProtocol(const BitwigProtocol&) = delete;
    BitwigProtocol& operator=(const BitwigProtocol&) = delete;
    BitwigProtocol(BitwigProtocol&&) = delete;
    BitwigProtocol& operator=(BitwigProtocol&&) = delete;

    /**
     * @brief Send a protocol message
     *
     * Auto-detects MessageID from message type's MESSAGE_ID constant.
     *
     * @tparam T Message type (must have MESSAGE_ID and encode())
     * @param message Message to send
     *
     * @code
     * protocol.send(TransportPlayMessage{true});
     * protocol.send(DeviceMacroValueChangeMessage{index, value});
     * @endcode
     */
    template <typename T>
    void send(const T& message) {
        using Protocol::MAX_MESSAGE_SIZE;

        constexpr Protocol::MessageID messageId = T::MESSAGE_ID;

        // Encode payload
        uint8_t payload[T::MAX_PAYLOAD_SIZE];
        uint16_t payloadLen = message.encode(payload, sizeof(payload));

        // Build frame: [MessageID][fromHost][payload...]
        uint8_t frame[MAX_MESSAGE_SIZE];
        uint16_t offset = 0;

        frame[offset++] = static_cast<uint8_t>(messageId);
        frame[offset++] = 0;  // fromHost = false (we are the controller)

        std::memcpy(frame + offset, payload, payloadLen);
        offset += payloadLen;

        // Send via transport (COBS framing handled by transport)
        transport_.send(frame, offset);
    }

private:
    oc::hal::ISerialTransport& transport_;

    /**
     * @brief Dispatch incoming frame to callbacks
     *
     * Called automatically by transport when a complete frame arrives.
     */
    void dispatch(const uint8_t* data, size_t len) {
        using Protocol::MIN_MESSAGE_LENGTH;
        using Protocol::MESSAGE_TYPE_OFFSET;
        using Protocol::FROM_HOST_OFFSET;
        using Protocol::PAYLOAD_OFFSET;
        using Protocol::MessageID;
        using Protocol::DecoderRegistry;

        if (data == nullptr || len < MIN_MESSAGE_LENGTH) {
            OC_LOG_WARN("[Protocol] dispatch: invalid frame (null or too short: {})", len);
            return;
        }

        MessageID messageId = static_cast<MessageID>(data[MESSAGE_TYPE_OFFSET]);
        bool fromHost = (data[FROM_HOST_OFFSET] != 0);

        uint16_t payloadLen = len - PAYLOAD_OFFSET;
        const uint8_t* payload = data + PAYLOAD_OFFSET;

        DecoderRegistry::dispatch(*this, messageId, payload, payloadLen, fromHost);
    }
};

}  // namespace bitwig
