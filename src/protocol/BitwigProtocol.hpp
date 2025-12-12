#pragma once

/**
 * @file BitwigProtocol.hpp
 * @brief Bitwig protocol wrapper for open-control framework
 *
 * Adapts the Protocol class to use the framework's MidiAPI and EventBus.
 * Subscribes to SysExEvent from the EventBus (set up by OpenControlApp)
 * to avoid callback conflicts.
 *
 * ## Usage
 *
 * ```cpp
 * // In BitwigContext
 * BitwigProtocol protocol(midi(), events());
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

#include <oc/api/MidiAPI.hpp>
#include <oc/core/event/Events.hpp>
#include <oc/core/event/IEventBus.hpp>

#include "DecoderRegistry.hpp"
#include "MessageID.hpp"
#include "ProtocolCallbacks.hpp"
#include "ProtocolConstants.hpp"

namespace bitwig {

/**
 * @brief Bitwig SysEx protocol handler using open-control framework
 *
 * Inherits from ProtocolCallbacks for message callbacks.
 * Subscribes to EventBus for incoming SysEx (avoids callback conflicts).
 * Uses MidiAPI for outgoing SysEx.
 */
class BitwigProtocol : public Protocol::ProtocolCallbacks {
public:
    /**
     * @brief Construct protocol with MidiAPI and EventBus
     *
     * Subscribes to SysExEvent from EventBus for incoming messages.
     * Uses MidiAPI only for sending outgoing messages.
     *
     * @param midi Reference to MidiAPI (for sending)
     * @param events Reference to EventBus (for receiving SysEx events)
     */
    BitwigProtocol(oc::api::MidiAPI& midi, oc::core::event::IEventBus& events)
        : midi_(midi), events_(events) {
        // Subscribe to SysEx events from EventBus
        using namespace oc::core::event;
        subscriptionId_ = events_.on(
            EventCategory::MIDI,
            MidiEvent::SYSEX,
            [this](const Event& evt) {
                const auto& sysex = static_cast<const SysExEvent&>(evt);
                dispatch(sysex.data, sysex.length);
            });
    }

    ~BitwigProtocol() {
        // Unsubscribe from EventBus
        events_.off(subscriptionId_);
    }

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
        using Protocol::MANUFACTURER_ID;
        using Protocol::DEVICE_ID;
        using Protocol::SYSEX_START;
        using Protocol::SYSEX_END;

        constexpr Protocol::MessageID messageId = T::MESSAGE_ID;

        uint8_t payload[T::MAX_PAYLOAD_SIZE];
        uint16_t payloadLen = message.encode(payload, sizeof(payload));

        uint8_t sysex[MAX_MESSAGE_SIZE];

        uint16_t offset = 0;
        sysex[offset++] = SYSEX_START;
        sysex[offset++] = MANUFACTURER_ID;
        sysex[offset++] = DEVICE_ID;
        sysex[offset++] = static_cast<uint8_t>(messageId);
        sysex[offset++] = 0;  // fromHost flag (C++ = controller = 0)

        for (uint16_t i = 0; i < payloadLen; ++i) {
            sysex[offset++] = payload[i];
        }

        sysex[offset++] = SYSEX_END;

        midi_.sendSysEx(sysex, offset);
    }

    /**
     * @brief Request host connection status
     *
     * Sends RequestHostStatusMessage. If Bitwig is running,
     * it will respond with HostInitializedMessage.
     */
    void requestHostStatus();

    /**
     * @brief Request device list for current track
     */
    void requestDeviceList();

    /**
     * @brief Request page names for current device
     */
    void requestPageNames();

    /**
     * @brief Request track list
     */
    void requestTrackList();

    /**
     * @brief Select device by index
     * @param index Device index in chain (0-based)
     */
    void selectDevice(uint8_t index);

    /**
     * @brief Select page by index
     * @param index Page index (0-based)
     */
    void selectPage(uint8_t index);

    /**
     * @brief Select track by index
     * @param index Track index (0-based)
     */
    void selectTrack(uint8_t index);

    /**
     * @brief Send parameter value change
     * @param paramIndex Parameter index (0-7)
     * @param value Normalized value [0.0, 1.0]
     */
    void sendParameterValue(uint8_t paramIndex, float value);

    /**
     * @brief Send parameter touch state
     * @param paramIndex Parameter index (0-7)
     * @param touched true when encoder touched, false when released
     */
    void sendParameterTouch(uint8_t paramIndex, bool touched);

    /**
     * @brief Toggle transport play/pause
     */
    void togglePlay();

    /**
     * @brief Toggle transport record
     */
    void toggleRecord();

    /**
     * @brief Stop transport
     */
    void stop();

private:
    oc::api::MidiAPI& midi_;
    oc::core::event::IEventBus& events_;
    oc::core::event::SubscriptionID subscriptionId_{0};

    /**
     * @brief Dispatch incoming SysEx to callbacks
     *
     * Called automatically by SysEx receive callback.
     */
    void dispatch(const uint8_t* sysex, uint16_t length) {
        using Protocol::MIN_MESSAGE_LENGTH;
        using Protocol::MANUFACTURER_ID;
        using Protocol::DEVICE_ID;
        using Protocol::SYSEX_START;
        using Protocol::SYSEX_END;
        using Protocol::MESSAGE_TYPE_OFFSET;
        using Protocol::FROM_HOST_OFFSET;
        using Protocol::PAYLOAD_OFFSET;
        using Protocol::MessageID;
        using Protocol::DecoderRegistry;

        if (sysex == nullptr || length < MIN_MESSAGE_LENGTH) {
            return;
        }

        if (sysex[0] != SYSEX_START || sysex[length - 1] != SYSEX_END) {
            return;
        }

        if (sysex[1] != MANUFACTURER_ID || sysex[2] != DEVICE_ID) {
            return;
        }

        MessageID messageId = static_cast<MessageID>(sysex[MESSAGE_TYPE_OFFSET]);
        bool fromHost = (sysex[FROM_HOST_OFFSET] != 0);

        uint16_t payloadLen = length - MIN_MESSAGE_LENGTH;
        const uint8_t* payload = sysex + PAYLOAD_OFFSET;

        DecoderRegistry::dispatch(*this, messageId, payload, payloadLen, fromHost);
    }
};

}  // namespace bitwig
