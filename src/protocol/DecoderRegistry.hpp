/**
 * DecoderRegistry.hpp - MessageID to Decoder mapping
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Dispatches incoming messages to typed callbacks.
 * Called by Protocol.dispatch().
 *
 * Supports optimistic update reconciliation for messages with optimistic=True.
 */

#pragma once

#include "MessageID.hpp"
#include "MessageStructure.hpp"
#include "ProtocolCallbacks.hpp"

#include <cstdint>

namespace Protocol {

class DecoderRegistry {
public:
    /**
     * Decode message and invoke appropriate callback
     *
     * @param callbacks Object with typed callbacks (ProtocolCallbacks)
     * @param messageId MessageID to decode
     * @param payload Raw payload bytes
     * @param payloadLen Payload length
     * @param fromHost Origin flag (true if message from host, false if from controller)
     */
    static void dispatch(
        ProtocolCallbacks& callbacks,
        MessageID messageId,
        const uint8_t* payload,
        uint16_t payloadLen,
        bool fromHost
    ) {
        switch (messageId) {
        case MessageID::DEVICE_CHANGE:
            if (callbacks.onDeviceChange) {
                auto decoded = DeviceChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_CHANGE_HEADER:
            if (callbacks.onDeviceChangeHeader) {
                auto decoded = DeviceChangeHeaderMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChangeHeader(msg);
                }
            }
            break;
        case MessageID::DEVICE_CHILDREN:
            if (callbacks.onDeviceChildren) {
                auto decoded = DeviceChildrenMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChildren(msg);
                }
            }
            break;
        case MessageID::DEVICE_LIST:
            if (callbacks.onDeviceList) {
                auto decoded = DeviceListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceList(msg);
                }
            }
            break;
        case MessageID::DEVICE_MACRO_DISCRETE_VALUES:
            if (callbacks.onDeviceMacroDiscreteValues) {
                auto decoded = DeviceMacroDiscreteValuesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroDiscreteValues(msg);
                }
            }
            break;
        case MessageID::DEVICE_MACRO_NAME_CHANGE:
            if (callbacks.onDeviceMacroNameChange) {
                auto decoded = DeviceMacroNameChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroNameChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_MACRO_TOUCH:
            if (callbacks.onDeviceMacroTouch) {
                auto decoded = DeviceMacroTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroTouch(msg);
                }
            }
            break;
        case MessageID::DEVICE_MACRO_UPDATE:
            if (callbacks.onDeviceMacroUpdate) {
                auto decoded = DeviceMacroUpdateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroUpdate(msg);
                }
            }
            break;
        case MessageID::DEVICE_MACRO_VALUE_CHANGE:
            if (callbacks.onDeviceMacroValueChange) {
                auto decoded = DeviceMacroValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroValueChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_PAGE_CHANGE:
            if (callbacks.onDevicePageChange) {
                auto decoded = DevicePageChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_PAGE_NAMES:
            if (callbacks.onDevicePageNames) {
                auto decoded = DevicePageNamesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageNames(msg);
                }
            }
            break;
        case MessageID::DEVICE_PAGE_SELECT_BY_INDEX:
            if (callbacks.onDevicePageSelectByIndex) {
                auto decoded = DevicePageSelectByIndexMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageSelectByIndex(msg);
                }
            }
            break;
        case MessageID::DEVICE_SELECT_BY_INDEX:
            if (callbacks.onDeviceSelectByIndex) {
                auto decoded = DeviceSelectByIndexMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceSelectByIndex(msg);
                }
            }
            break;
        case MessageID::DEVICE_STATE_CHANGE:
            if (callbacks.onDeviceStateChange) {
                auto decoded = DeviceStateChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceStateChange(msg);
                }
            }
            break;
        case MessageID::ENTER_DEVICE_CHILD:
            if (callbacks.onEnterDeviceChild) {
                auto decoded = EnterDeviceChildMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onEnterDeviceChild(msg);
                }
            }
            break;
        case MessageID::EXIT_TO_PARENT:
            if (callbacks.onExitToParent) {
                auto decoded = ExitToParentMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onExitToParent(msg);
                }
            }
            break;
        case MessageID::REQUEST_DEVICE_CHILDREN:
            if (callbacks.onRequestDeviceChildren) {
                auto decoded = RequestDeviceChildrenMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceChildren(msg);
                }
            }
            break;
        case MessageID::REQUEST_DEVICE_LIST:
            if (callbacks.onRequestDeviceList) {
                auto decoded = RequestDeviceListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceList(msg);
                }
            }
            break;
        case MessageID::REQUEST_DEVICE_PAGE_NAMES:
            if (callbacks.onRequestDevicePageNames) {
                auto decoded = RequestDevicePageNamesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDevicePageNames(msg);
                }
            }
            break;
        case MessageID::LAST_CLICKED_TOUCH:
            if (callbacks.onLastClickedTouch) {
                auto decoded = LastClickedTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedTouch(msg);
                }
            }
            break;
        case MessageID::LAST_CLICKED_UPDATE:
            if (callbacks.onLastClickedUpdate) {
                auto decoded = LastClickedUpdateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedUpdate(msg);
                }
            }
            break;
        case MessageID::LAST_CLICKED_VALUE_CHANGE:
            if (callbacks.onLastClickedValueChange) {
                auto decoded = LastClickedValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedValueChange(msg);
                }
            }
            break;
        case MessageID::HOST_DEACTIVATED:
            if (callbacks.onHostDeactivated) {
                auto decoded = HostDeactivatedMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onHostDeactivated(msg);
                }
            }
            break;
        case MessageID::HOST_INITIALIZED:
            if (callbacks.onHostInitialized) {
                auto decoded = HostInitializedMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onHostInitialized(msg);
                }
            }
            break;
        case MessageID::REQUEST_HOST_STATUS:
            if (callbacks.onRequestHostStatus) {
                auto decoded = RequestHostStatusMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestHostStatus(msg);
                }
            }
            break;
        case MessageID::ENTER_TRACK_GROUP:
            if (callbacks.onEnterTrackGroup) {
                auto decoded = EnterTrackGroupMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onEnterTrackGroup(msg);
                }
            }
            break;
        case MessageID::EXIT_TRACK_GROUP:
            if (callbacks.onExitTrackGroup) {
                auto decoded = ExitTrackGroupMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onExitTrackGroup(msg);
                }
            }
            break;
        case MessageID::REQUEST_TRACK_LIST:
            if (callbacks.onRequestTrackList) {
                auto decoded = RequestTrackListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackList(msg);
                }
            }
            break;
        case MessageID::TRACK_ACTIVATE:
            if (callbacks.onTrackActivate) {
                auto decoded = TrackActivateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackActivate(msg);
                }
            }
            break;
        case MessageID::TRACK_CHANGE:
            if (callbacks.onTrackChange) {
                auto decoded = TrackChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackChange(msg);
                }
            }
            break;
        case MessageID::TRACK_LIST:
            if (callbacks.onTrackList) {
                auto decoded = TrackListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackList(msg);
                }
            }
            break;
        case MessageID::TRACK_MUTE:
            if (callbacks.onTrackMute) {
                auto decoded = TrackMuteMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMute(msg);
                }
            }
            break;
        case MessageID::TRACK_SELECT_BY_INDEX:
            if (callbacks.onTrackSelectByIndex) {
                auto decoded = TrackSelectByIndexMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSelectByIndex(msg);
                }
            }
            break;
        case MessageID::TRACK_SOLO:
            if (callbacks.onTrackSolo) {
                auto decoded = TrackSoloMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSolo(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_PLAY:
            if (callbacks.onTransportPlay) {
                auto decoded = TransportPlayMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportPlay(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_RECORD:
            if (callbacks.onTransportRecord) {
                auto decoded = TransportRecordMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportRecord(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_STOP:
            if (callbacks.onTransportStop) {
                auto decoded = TransportStopMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportStop(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_TEMPO:
            if (callbacks.onTransportTempo) {
                auto decoded = TransportTempoMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportTempo(msg);
                }
            }
            break;
            default:
                // Unknown message type - silently ignore
                break;
        }
    }
};

}  // namespace Protocol
