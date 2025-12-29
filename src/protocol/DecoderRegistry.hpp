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
     */
    static void dispatch(
        ProtocolCallbacks& callbacks,
        MessageID messageId,
        const uint8_t* payload,
        uint16_t payloadLen
    ) {
        switch (messageId) {
        case MessageID::DEVICE_CHANGE:
            if (callbacks.onDeviceChange) {
                auto decoded = DeviceChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceChange(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_CHANGE_HEADER:
            if (callbacks.onDeviceChangeHeader) {
                auto decoded = DeviceChangeHeaderMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceChangeHeader(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_CHILDREN:
            if (callbacks.onDeviceChildren) {
                auto decoded = DeviceChildrenMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceChildren(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_ENABLED_STATE:
            if (callbacks.onDeviceEnabledState) {
                auto decoded = DeviceEnabledStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceEnabledState(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_LIST_WINDOW:
            if (callbacks.onDeviceListWindow) {
                auto decoded = DeviceListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceListWindow(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_PAGE_CHANGE:
            if (callbacks.onDevicePageChange) {
                auto decoded = DevicePageChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDevicePageChange(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_PAGE_NAMES_WINDOW:
            if (callbacks.onDevicePageNamesWindow) {
                auto decoded = DevicePageNamesWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDevicePageNamesWindow(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_PAGE_SELECT:
            if (callbacks.onDevicePageSelect) {
                auto decoded = DevicePageSelectMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDevicePageSelect(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROLS_BATCH:
            if (callbacks.onDeviceRemoteControlsBatch) {
                auto decoded = DeviceRemoteControlsBatchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlsBatch(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_DISCRETE_VALUES:
            if (callbacks.onDeviceRemoteControlDiscreteValues) {
                auto decoded = DeviceRemoteControlDiscreteValuesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlDiscreteValues(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE:
            if (callbacks.onDeviceRemoteControlHasAutomationChange) {
                auto decoded = DeviceRemoteControlHasAutomationChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlHasAutomationChange(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE:
            if (callbacks.onDeviceRemoteControlIsModulatedChange) {
                auto decoded = DeviceRemoteControlIsModulatedChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlIsModulatedChange(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_NAME_CHANGE:
            if (callbacks.onDeviceRemoteControlNameChange) {
                auto decoded = DeviceRemoteControlNameChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlNameChange(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE:
            if (callbacks.onDeviceRemoteControlOriginChange) {
                auto decoded = DeviceRemoteControlOriginChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlOriginChange(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION:
            if (callbacks.onDeviceRemoteControlRestoreAutomation) {
                auto decoded = DeviceRemoteControlRestoreAutomationMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlRestoreAutomation(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_TOUCH:
            if (callbacks.onDeviceRemoteControlTouch) {
                auto decoded = DeviceRemoteControlTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlTouch(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_REMOTE_CONTROL_UPDATE:
            if (callbacks.onDeviceRemoteControlUpdate) {
                auto decoded = DeviceRemoteControlUpdateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceRemoteControlUpdate(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_SELECT:
            if (callbacks.onDeviceSelect) {
                auto decoded = DeviceSelectMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceSelect(decoded.value());
                }
            }
            break;
        case MessageID::DEVICE_STATE:
            if (callbacks.onDeviceState) {
                auto decoded = DeviceStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onDeviceState(decoded.value());
                }
            }
            break;
        case MessageID::ENTER_DEVICE_CHILD:
            if (callbacks.onEnterDeviceChild) {
                auto decoded = EnterDeviceChildMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onEnterDeviceChild(decoded.value());
                }
            }
            break;
        case MessageID::EXIT_TO_PARENT:
            if (callbacks.onExitToParent) {
                auto decoded = ExitToParentMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onExitToParent(decoded.value());
                }
            }
            break;
        case MessageID::REMOTE_CONTROL_VALUE:
            if (callbacks.onRemoteControlValue) {
                auto decoded = RemoteControlValueMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRemoteControlValue(decoded.value());
                }
            }
            break;
        case MessageID::REMOTE_CONTROL_VALUE_STATE:
            if (callbacks.onRemoteControlValueState) {
                auto decoded = RemoteControlValueStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRemoteControlValueState(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_DEVICE_CHILDREN:
            if (callbacks.onRequestDeviceChildren) {
                auto decoded = RequestDeviceChildrenMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestDeviceChildren(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_DEVICE_LIST_WINDOW:
            if (callbacks.onRequestDeviceListWindow) {
                auto decoded = RequestDeviceListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestDeviceListWindow(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_DEVICE_PAGE_NAMES_WINDOW:
            if (callbacks.onRequestDevicePageNamesWindow) {
                auto decoded = RequestDevicePageNamesWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestDevicePageNamesWindow(decoded.value());
                }
            }
            break;
        case MessageID::VIEW_STATE:
            if (callbacks.onViewState) {
                auto decoded = ViewStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onViewState(decoded.value());
                }
            }
            break;
        case MessageID::LAST_CLICKED_TOUCH:
            if (callbacks.onLastClickedTouch) {
                auto decoded = LastClickedTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onLastClickedTouch(decoded.value());
                }
            }
            break;
        case MessageID::LAST_CLICKED_UPDATE:
            if (callbacks.onLastClickedUpdate) {
                auto decoded = LastClickedUpdateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onLastClickedUpdate(decoded.value());
                }
            }
            break;
        case MessageID::LAST_CLICKED_VALUE:
            if (callbacks.onLastClickedValue) {
                auto decoded = LastClickedValueMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onLastClickedValue(decoded.value());
                }
            }
            break;
        case MessageID::LAST_CLICKED_VALUE_STATE:
            if (callbacks.onLastClickedValueState) {
                auto decoded = LastClickedValueStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onLastClickedValueState(decoded.value());
                }
            }
            break;
        case MessageID::HOST_DEACTIVATED:
            if (callbacks.onHostDeactivated) {
                auto decoded = HostDeactivatedMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onHostDeactivated(decoded.value());
                }
            }
            break;
        case MessageID::HOST_INITIALIZED:
            if (callbacks.onHostInitialized) {
                auto decoded = HostInitializedMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onHostInitialized(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_HOST_STATUS:
            if (callbacks.onRequestHostStatus) {
                auto decoded = RequestHostStatusMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestHostStatus(decoded.value());
                }
            }
            break;
        case MessageID::ENTER_TRACK_GROUP:
            if (callbacks.onEnterTrackGroup) {
                auto decoded = EnterTrackGroupMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onEnterTrackGroup(decoded.value());
                }
            }
            break;
        case MessageID::EXIT_TRACK_GROUP:
            if (callbacks.onExitTrackGroup) {
                auto decoded = ExitTrackGroupMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onExitTrackGroup(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_SEND_DESTINATIONS:
            if (callbacks.onRequestSendDestinations) {
                auto decoded = RequestSendDestinationsMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestSendDestinations(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_TRACK_LIST_WINDOW:
            if (callbacks.onRequestTrackListWindow) {
                auto decoded = RequestTrackListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestTrackListWindow(decoded.value());
                }
            }
            break;
        case MessageID::REQUEST_TRACK_SEND_LIST:
            if (callbacks.onRequestTrackSendList) {
                auto decoded = RequestTrackSendListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onRequestTrackSendList(decoded.value());
                }
            }
            break;
        case MessageID::SELECT_MIX_SEND:
            if (callbacks.onSelectMixSend) {
                auto decoded = SelectMixSendMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onSelectMixSend(decoded.value());
                }
            }
            break;
        case MessageID::SEND_DESTINATIONS_LIST:
            if (callbacks.onSendDestinationsList) {
                auto decoded = SendDestinationsListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onSendDestinationsList(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_ACTIVATE:
            if (callbacks.onTrackActivate) {
                auto decoded = TrackActivateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackActivate(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_ARM:
            if (callbacks.onTrackArm) {
                auto decoded = TrackArmMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackArm(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_ARM_STATE:
            if (callbacks.onTrackArmState) {
                auto decoded = TrackArmStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackArmState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_CHANGE:
            if (callbacks.onTrackChange) {
                auto decoded = TrackChangeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackChange(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_LIST_WINDOW:
            if (callbacks.onTrackListWindow) {
                auto decoded = TrackListWindowMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackListWindow(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_MUTE:
            if (callbacks.onTrackMute) {
                auto decoded = TrackMuteMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackMute(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_MUTED_BY_SOLO_STATE:
            if (callbacks.onTrackMutedBySoloState) {
                auto decoded = TrackMutedBySoloStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackMutedBySoloState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_MUTE_STATE:
            if (callbacks.onTrackMuteState) {
                auto decoded = TrackMuteStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackMuteState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_PAN:
            if (callbacks.onTrackPan) {
                auto decoded = TrackPanMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackPan(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_PAN_HAS_AUTOMATION_STATE:
            if (callbacks.onTrackPanHasAutomationState) {
                auto decoded = TrackPanHasAutomationStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackPanHasAutomationState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_PAN_MODULATED_VALUE_STATE:
            if (callbacks.onTrackPanModulatedValueState) {
                auto decoded = TrackPanModulatedValueStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackPanModulatedValueState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_PAN_STATE:
            if (callbacks.onTrackPanState) {
                auto decoded = TrackPanStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackPanState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_PAN_TOUCH:
            if (callbacks.onTrackPanTouch) {
                auto decoded = TrackPanTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackPanTouch(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SELECT:
            if (callbacks.onTrackSelect) {
                auto decoded = TrackSelectMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSelect(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_ENABLED:
            if (callbacks.onTrackSendEnabled) {
                auto decoded = TrackSendEnabledMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendEnabled(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_ENABLED_STATE:
            if (callbacks.onTrackSendEnabledState) {
                auto decoded = TrackSendEnabledStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendEnabledState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_HAS_AUTOMATION_STATE:
            if (callbacks.onTrackSendHasAutomationState) {
                auto decoded = TrackSendHasAutomationStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendHasAutomationState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_LIST:
            if (callbacks.onTrackSendList) {
                auto decoded = TrackSendListMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendList(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_MODE:
            if (callbacks.onTrackSendMode) {
                auto decoded = TrackSendModeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendMode(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_MODE_STATE:
            if (callbacks.onTrackSendModeState) {
                auto decoded = TrackSendModeStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendModeState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_MODULATED_VALUE_STATE:
            if (callbacks.onTrackSendModulatedValueState) {
                auto decoded = TrackSendModulatedValueStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendModulatedValueState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_PRE_FADER_STATE:
            if (callbacks.onTrackSendPreFaderState) {
                auto decoded = TrackSendPreFaderStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendPreFaderState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_TOUCH:
            if (callbacks.onTrackSendTouch) {
                auto decoded = TrackSendTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendTouch(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_VALUE:
            if (callbacks.onTrackSendValue) {
                auto decoded = TrackSendValueMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendValue(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SEND_VALUE_STATE:
            if (callbacks.onTrackSendValueState) {
                auto decoded = TrackSendValueStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSendValueState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SOLO:
            if (callbacks.onTrackSolo) {
                auto decoded = TrackSoloMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSolo(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_SOLO_STATE:
            if (callbacks.onTrackSoloState) {
                auto decoded = TrackSoloStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackSoloState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_VOLUME:
            if (callbacks.onTrackVolume) {
                auto decoded = TrackVolumeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackVolume(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_VOLUME_HAS_AUTOMATION_STATE:
            if (callbacks.onTrackVolumeHasAutomationState) {
                auto decoded = TrackVolumeHasAutomationStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackVolumeHasAutomationState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_VOLUME_MODULATED_VALUE_STATE:
            if (callbacks.onTrackVolumeModulatedValueState) {
                auto decoded = TrackVolumeModulatedValueStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackVolumeModulatedValueState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_VOLUME_STATE:
            if (callbacks.onTrackVolumeState) {
                auto decoded = TrackVolumeStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackVolumeState(decoded.value());
                }
            }
            break;
        case MessageID::TRACK_VOLUME_TOUCH:
            if (callbacks.onTrackVolumeTouch) {
                auto decoded = TrackVolumeTouchMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTrackVolumeTouch(decoded.value());
                }
            }
            break;
        case MessageID::RESET_AUTOMATION_OVERRIDES:
            if (callbacks.onResetAutomationOverrides) {
                auto decoded = ResetAutomationOverridesMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onResetAutomationOverrides(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED:
            if (callbacks.onTransportArrangerAutomationWriteEnabled) {
                auto decoded = TransportArrangerAutomationWriteEnabledMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportArrangerAutomationWriteEnabled(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE:
            if (callbacks.onTransportArrangerAutomationWriteEnabledState) {
                auto decoded = TransportArrangerAutomationWriteEnabledStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportArrangerAutomationWriteEnabledState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_ARRANGER_OVERDUB_ENABLED:
            if (callbacks.onTransportArrangerOverdubEnabled) {
                auto decoded = TransportArrangerOverdubEnabledMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportArrangerOverdubEnabled(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE:
            if (callbacks.onTransportArrangerOverdubEnabledState) {
                auto decoded = TransportArrangerOverdubEnabledStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportArrangerOverdubEnabledState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE:
            if (callbacks.onTransportAutomationOverrideActiveState) {
                auto decoded = TransportAutomationOverrideActiveStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportAutomationOverrideActiveState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_AUTOMATION_WRITE_MODE:
            if (callbacks.onTransportAutomationWriteMode) {
                auto decoded = TransportAutomationWriteModeMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportAutomationWriteMode(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_AUTOMATION_WRITE_MODE_STATE:
            if (callbacks.onTransportAutomationWriteModeState) {
                auto decoded = TransportAutomationWriteModeStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportAutomationWriteModeState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED:
            if (callbacks.onTransportClipLauncherAutomationWriteEnabled) {
                auto decoded = TransportClipLauncherAutomationWriteEnabledMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportClipLauncherAutomationWriteEnabled(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_STATE:
            if (callbacks.onTransportClipLauncherAutomationWriteEnabledState) {
                auto decoded = TransportClipLauncherAutomationWriteEnabledStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportClipLauncherAutomationWriteEnabledState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED:
            if (callbacks.onTransportClipLauncherOverdubEnabled) {
                auto decoded = TransportClipLauncherOverdubEnabledMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportClipLauncherOverdubEnabled(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE:
            if (callbacks.onTransportClipLauncherOverdubEnabledState) {
                auto decoded = TransportClipLauncherOverdubEnabledStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportClipLauncherOverdubEnabledState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_PLAY:
            if (callbacks.onTransportPlay) {
                auto decoded = TransportPlayMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportPlay(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_PLAYING_STATE:
            if (callbacks.onTransportPlayingState) {
                auto decoded = TransportPlayingStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportPlayingState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_RECORD:
            if (callbacks.onTransportRecord) {
                auto decoded = TransportRecordMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportRecord(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_RECORDING_STATE:
            if (callbacks.onTransportRecordingState) {
                auto decoded = TransportRecordingStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportRecordingState(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_STOP:
            if (callbacks.onTransportStop) {
                auto decoded = TransportStopMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportStop(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_TEMPO:
            if (callbacks.onTransportTempo) {
                auto decoded = TransportTempoMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportTempo(decoded.value());
                }
            }
            break;
        case MessageID::TRANSPORT_TEMPO_STATE:
            if (callbacks.onTransportTempoState) {
                auto decoded = TransportTempoStateMessage::decode(payload, payloadLen);
                if (decoded.has_value()) {
                    callbacks.onTransportTempoState(decoded.value());
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
