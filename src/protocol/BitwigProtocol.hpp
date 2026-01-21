#pragma once

/**
 * @file BitwigProtocol.hpp
 * @brief Bitwig protocol wrapper for open-control framework (Binary)
 *
 * Uses IFrameTransport for frame-based communication.
 * The transport layer handles framing (e.g., COBS for serial) internally.
 *
 * ## Usage
 *
 * ```cpp
 * // In BitwigContext
 * BitwigProtocol protocol(serial());
 *
 * // Send messages using explicit API
 * protocol.transportPlay(true);
 * protocol.deviceSelect(index);
 *
 * // Register callbacks (inherited from ProtocolCallbacks)
 * protocol.onTransportPlayingState = [this](const TransportPlayingStateMessage& msg) {
 *     state_.transport.playing.set(msg.isPlaying);
 * };
 * ```
 */

#include <cstdint>
#include <cstring>

#include <oc/interface/ITransport.hpp>
#include <oc/log/Log.hpp>

#include "DecoderRegistry.hpp"
#include "MessageID.hpp"
#include "ProtocolCallbacks.hpp"
#include "ProtocolConstants.hpp"

namespace bitwig {

/**
 * @brief Bitwig Binary protocol handler using open-control framework
 *
 * Inherits from ProtocolCallbacks for message callbacks.
 * Uses IFrameTransport for frame-based communication.
 */
class BitwigProtocol : public Protocol::ProtocolCallbacks {
public:
    /**
     * @brief Construct protocol with IFrameTransport
     *
     * Registers receive callback with transport for automatic dispatch.
     *
     * @param transport Reference to IFrameTransport (must outlive Protocol)
     */
    explicit BitwigProtocol(oc::interface::ITransport& transport)
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

    // =========================================================================
    // Explicit API methods (generated) - e.g. transportPlay(bool), deviceSelect(uint8_t)
    // =========================================================================
#include "ProtocolMethods.ipp"

private:
    oc::interface::ITransport& transport_;

    /**
     * @brief Send a protocol message (internal use only)
     *
     * Use explicit API methods instead (e.g. transportPlay(), deviceSelect()).
     */
    template <typename T>
    void send(const T& message) {
        using Protocol::MAX_MESSAGE_SIZE;

        constexpr Protocol::MessageID messageId = T::MESSAGE_ID;

        // Encode payload
        uint8_t payload[T::MAX_PAYLOAD_SIZE];
        uint16_t payloadLen = message.encode(payload, sizeof(payload));

        // Build frame: [MessageID][payload...]
        uint8_t frame[MAX_MESSAGE_SIZE];
        uint16_t offset = 0;

        frame[offset++] = static_cast<uint8_t>(messageId);

        std::memcpy(frame + offset, payload, payloadLen);
        offset += payloadLen;

        // Send via transport (COBS framing handled by transport)
        transport_.send(frame, offset);
    }

    /**
     * @brief Dispatch incoming frame to callbacks
     *
     * Called automatically by transport when a complete frame arrives.
     */
    void dispatch(const uint8_t* data, size_t len) {
        using Protocol::MIN_MESSAGE_LENGTH;
        using Protocol::MESSAGE_TYPE_OFFSET;
        using Protocol::PAYLOAD_OFFSET;
        using Protocol::MessageID;
        using Protocol::DecoderRegistry;

        if (data == nullptr || len < MIN_MESSAGE_LENGTH) {
            OC_LOG_WARN("[Protocol] dispatch: invalid frame (null or too short: {})", len);
            return;
        }

        MessageID messageId = static_cast<MessageID>(data[MESSAGE_TYPE_OFFSET]);

        uint16_t payloadLen = len - PAYLOAD_OFFSET;
        const uint8_t* payload = data + PAYLOAD_OFFSET;

        DecoderRegistry::dispatch(*this, messageId, payload, payloadLen);
    }
};

}  // namespace bitwig
