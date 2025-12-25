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
        case MessageID::DEVICE_LIST_WINDOW:
            if (callbacks.onDeviceListWindow) {
                auto decoded = DeviceListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceListWindow(msg);
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
        case MessageID::DEVICE_PAGE_NAMES_WINDOW:
            if (callbacks.onDevicePageNamesWindow) {
                auto decoded = DevicePageNamesWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageNamesWindow(msg);
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
        case MessageID::DEVICE_REMOTE_CONTROLS_BATCH:
            if (callbacks.onDeviceRemoteControlsBatch) {
                auto decoded = DeviceRemoteControlsBatchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlsBatch(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_DISCRETE_VALUES:
            if (callbacks.onDeviceRemoteControlDiscreteValues) {
                auto decoded = DeviceRemoteControlDiscreteValuesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlDiscreteValues(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE:
            if (callbacks.onDeviceRemoteControlHasAutomationChange) {
                auto decoded = DeviceRemoteControlHasAutomationChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlHasAutomationChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE:
            if (callbacks.onDeviceRemoteControlIsModulatedChange) {
                auto decoded = DeviceRemoteControlIsModulatedChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlIsModulatedChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_NAME_CHANGE:
            if (callbacks.onDeviceRemoteControlNameChange) {
                auto decoded = DeviceRemoteControlNameChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlNameChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE:
            if (callbacks.onDeviceRemoteControlOriginChange) {
                auto decoded = DeviceRemoteControlOriginChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlOriginChange(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION:
            if (callbacks.onDeviceRemoteControlRestoreAutomation) {
                auto decoded = DeviceRemoteControlRestoreAutomationMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlRestoreAutomation(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_TOUCH:
            if (callbacks.onDeviceRemoteControlTouch) {
                auto decoded = DeviceRemoteControlTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlTouch(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_UPDATE:
            if (callbacks.onDeviceRemoteControlUpdate) {
                auto decoded = DeviceRemoteControlUpdateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlUpdate(msg);
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_VALUE_CHANGE:
            if (callbacks.onDeviceRemoteControlValueChange) {
                auto decoded = DeviceRemoteControlValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlValueChange(msg);
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
        case MessageID::REQUEST_DEVICE_LIST_WINDOW:
            if (callbacks.onRequestDeviceListWindow) {
                auto decoded = RequestDeviceListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceListWindow(msg);
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
        case MessageID::REQUEST_DEVICE_PAGE_NAMES_WINDOW:
            if (callbacks.onRequestDevicePageNamesWindow) {
                auto decoded = RequestDevicePageNamesWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDevicePageNamesWindow(msg);
                }
            }
            break;
        case MessageID::VIEW_STATE_CHANGE:
            if (callbacks.onViewStateChange) {
                auto decoded = ViewStateChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onViewStateChange(msg);
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
        case MessageID::REQUEST_SEND_DESTINATIONS:
            if (callbacks.onRequestSendDestinations) {
                auto decoded = RequestSendDestinationsMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestSendDestinations(msg);
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
        case MessageID::REQUEST_TRACK_LIST_WINDOW:
            if (callbacks.onRequestTrackListWindow) {
                auto decoded = RequestTrackListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackListWindow(msg);
                }
            }
            break;
        case MessageID::REQUEST_TRACK_SEND_LIST:
            if (callbacks.onRequestTrackSendList) {
                auto decoded = RequestTrackSendListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackSendList(msg);
                }
            }
            break;
        case MessageID::SELECT_MIX_SEND:
            if (callbacks.onSelectMixSend) {
                auto decoded = SelectMixSendMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onSelectMixSend(msg);
                }
            }
            break;
        case MessageID::SEND_DESTINATIONS_LIST:
            if (callbacks.onSendDestinationsList) {
                auto decoded = SendDestinationsListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onSendDestinationsList(msg);
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
        case MessageID::TRACK_ARM_CHANGE:
            if (callbacks.onTrackArmChange) {
                auto decoded = TrackArmChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackArmChange(msg);
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
        case MessageID::TRACK_LIST_WINDOW:
            if (callbacks.onTrackListWindow) {
                auto decoded = TrackListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackListWindow(msg);
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
        case MessageID::TRACK_MUTED_BY_SOLO_CHANGE:
            if (callbacks.onTrackMutedBySoloChange) {
                auto decoded = TrackMutedBySoloChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMutedBySoloChange(msg);
                }
            }
            break;
        case MessageID::TRACK_PAN_CHANGE:
            if (callbacks.onTrackPanChange) {
                auto decoded = TrackPanChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanChange(msg);
                }
            }
            break;
        case MessageID::TRACK_PAN_HAS_AUTOMATION_CHANGE:
            if (callbacks.onTrackPanHasAutomationChange) {
                auto decoded = TrackPanHasAutomationChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanHasAutomationChange(msg);
                }
            }
            break;
        case MessageID::TRACK_PAN_MODULATED_VALUE_CHANGE:
            if (callbacks.onTrackPanModulatedValueChange) {
                auto decoded = TrackPanModulatedValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanModulatedValueChange(msg);
                }
            }
            break;
        case MessageID::TRACK_PAN_TOUCH:
            if (callbacks.onTrackPanTouch) {
                auto decoded = TrackPanTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanTouch(msg);
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
        case MessageID::TRACK_SEND_ENABLED_CHANGE:
            if (callbacks.onTrackSendEnabledChange) {
                auto decoded = TrackSendEnabledChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendEnabledChange(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_HAS_AUTOMATION_CHANGE:
            if (callbacks.onTrackSendHasAutomationChange) {
                auto decoded = TrackSendHasAutomationChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendHasAutomationChange(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_LIST:
            if (callbacks.onTrackSendList) {
                auto decoded = TrackSendListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendList(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_MODE_CHANGE:
            if (callbacks.onTrackSendModeChange) {
                auto decoded = TrackSendModeChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendModeChange(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_MODULATED_VALUE_CHANGE:
            if (callbacks.onTrackSendModulatedValueChange) {
                auto decoded = TrackSendModulatedValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendModulatedValueChange(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_PRE_FADER_CHANGE:
            if (callbacks.onTrackSendPreFaderChange) {
                auto decoded = TrackSendPreFaderChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendPreFaderChange(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_TOUCH:
            if (callbacks.onTrackSendTouch) {
                auto decoded = TrackSendTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendTouch(msg);
                }
            }
            break;
        case MessageID::TRACK_SEND_VALUE_CHANGE:
            if (callbacks.onTrackSendValueChange) {
                auto decoded = TrackSendValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendValueChange(msg);
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
        case MessageID::TRACK_VOLUME_CHANGE:
            if (callbacks.onTrackVolumeChange) {
                auto decoded = TrackVolumeChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeChange(msg);
                }
            }
            break;
        case MessageID::TRACK_VOLUME_HAS_AUTOMATION_CHANGE:
            if (callbacks.onTrackVolumeHasAutomationChange) {
                auto decoded = TrackVolumeHasAutomationChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeHasAutomationChange(msg);
                }
            }
            break;
        case MessageID::TRACK_VOLUME_MODULATED_VALUE_CHANGE:
            if (callbacks.onTrackVolumeModulatedValueChange) {
                auto decoded = TrackVolumeModulatedValueChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeModulatedValueChange(msg);
                }
            }
            break;
        case MessageID::TRACK_VOLUME_TOUCH:
            if (callbacks.onTrackVolumeTouch) {
                auto decoded = TrackVolumeTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeTouch(msg);
                }
            }
            break;
        case MessageID::RESET_AUTOMATION_OVERRIDES:
            if (callbacks.onResetAutomationOverrides) {
                auto decoded = ResetAutomationOverridesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onResetAutomationOverrides(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE:
            if (callbacks.onTransportArrangerAutomationWriteEnabledChange) {
                auto decoded = TransportArrangerAutomationWriteEnabledChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerAutomationWriteEnabledChange(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE:
            if (callbacks.onTransportArrangerOverdubEnabledChange) {
                auto decoded = TransportArrangerOverdubEnabledChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerOverdubEnabledChange(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE:
            if (callbacks.onTransportAutomationOverrideActiveChange) {
                auto decoded = TransportAutomationOverrideActiveChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationOverrideActiveChange(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE:
            if (callbacks.onTransportAutomationWriteModeChange) {
                auto decoded = TransportAutomationWriteModeChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationWriteModeChange(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE:
            if (callbacks.onTransportClipLauncherAutomationWriteEnabledChange) {
                auto decoded = TransportClipLauncherAutomationWriteEnabledChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherAutomationWriteEnabledChange(msg);
                }
            }
            break;
        case MessageID::TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE:
            if (callbacks.onTransportClipLauncherOverdubEnabledChange) {
                auto decoded = TransportClipLauncherOverdubEnabledChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    auto& msg = decoded.value();  // Reference to avoid copy
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherOverdubEnabledChange(msg);
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
