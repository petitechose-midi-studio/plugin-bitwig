package protocol;

import protocol.MessageID;
import protocol.ProtocolCallbacks;
import protocol.struct.*;

/**
 * DecoderRegistry - MessageID to Decoder mapping
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Dispatches incoming messages to typed callbacks.
 * Called by Protocol.dispatch().
 */
public class DecoderRegistry {

    /**
     * Decode message and invoke appropriate callback
     *
     * @param callbacks Object with typed callbacks (ProtocolCallbacks)
     * @param messageId MessageID to decode
     * @param payload Raw payload bytes
     */
    public static void dispatch(
        ProtocolCallbacks callbacks,
        MessageID messageId,
        byte[] payload
    ) {
        switch (messageId) {
            case DEVICE_CHANGE:
                if (callbacks.onDeviceChange != null) {
                    callbacks.onDeviceChange.handle(DeviceChangeMessage.decode(payload));
                }
                break;
            case DEVICE_CHANGE_HEADER:
                if (callbacks.onDeviceChangeHeader != null) {
                    callbacks.onDeviceChangeHeader.handle(DeviceChangeHeaderMessage.decode(payload));
                }
                break;
            case DEVICE_CHILDREN:
                if (callbacks.onDeviceChildren != null) {
                    callbacks.onDeviceChildren.handle(DeviceChildrenMessage.decode(payload));
                }
                break;
            case DEVICE_ENABLED_STATE:
                if (callbacks.onDeviceEnabledState != null) {
                    callbacks.onDeviceEnabledState.handle(DeviceEnabledStateMessage.decode(payload));
                }
                break;
            case DEVICE_LIST_WINDOW:
                if (callbacks.onDeviceListWindow != null) {
                    callbacks.onDeviceListWindow.handle(DeviceListWindowMessage.decode(payload));
                }
                break;
            case DEVICE_PAGE_CHANGE:
                if (callbacks.onDevicePageChange != null) {
                    callbacks.onDevicePageChange.handle(DevicePageChangeMessage.decode(payload));
                }
                break;
            case DEVICE_PAGE_NAMES_WINDOW:
                if (callbacks.onDevicePageNamesWindow != null) {
                    callbacks.onDevicePageNamesWindow.handle(DevicePageNamesWindowMessage.decode(payload));
                }
                break;
            case DEVICE_PAGE_SELECT:
                if (callbacks.onDevicePageSelect != null) {
                    callbacks.onDevicePageSelect.handle(DevicePageSelectMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROLS_BATCH:
                if (callbacks.onDeviceRemoteControlsBatch != null) {
                    callbacks.onDeviceRemoteControlsBatch.handle(DeviceRemoteControlsBatchMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_DISCRETE_VALUES:
                if (callbacks.onDeviceRemoteControlDiscreteValues != null) {
                    callbacks.onDeviceRemoteControlDiscreteValues.handle(DeviceRemoteControlDiscreteValuesMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE:
                if (callbacks.onDeviceRemoteControlHasAutomationChange != null) {
                    callbacks.onDeviceRemoteControlHasAutomationChange.handle(DeviceRemoteControlHasAutomationChangeMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE:
                if (callbacks.onDeviceRemoteControlIsModulatedChange != null) {
                    callbacks.onDeviceRemoteControlIsModulatedChange.handle(DeviceRemoteControlIsModulatedChangeMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_NAME_CHANGE:
                if (callbacks.onDeviceRemoteControlNameChange != null) {
                    callbacks.onDeviceRemoteControlNameChange.handle(DeviceRemoteControlNameChangeMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE:
                if (callbacks.onDeviceRemoteControlOriginChange != null) {
                    callbacks.onDeviceRemoteControlOriginChange.handle(DeviceRemoteControlOriginChangeMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION:
                if (callbacks.onDeviceRemoteControlRestoreAutomation != null) {
                    callbacks.onDeviceRemoteControlRestoreAutomation.handle(DeviceRemoteControlRestoreAutomationMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_TOUCH:
                if (callbacks.onDeviceRemoteControlTouch != null) {
                    callbacks.onDeviceRemoteControlTouch.handle(DeviceRemoteControlTouchMessage.decode(payload));
                }
                break;
            case DEVICE_REMOTE_CONTROL_UPDATE:
                if (callbacks.onDeviceRemoteControlUpdate != null) {
                    callbacks.onDeviceRemoteControlUpdate.handle(DeviceRemoteControlUpdateMessage.decode(payload));
                }
                break;
            case DEVICE_SELECT:
                if (callbacks.onDeviceSelect != null) {
                    callbacks.onDeviceSelect.handle(DeviceSelectMessage.decode(payload));
                }
                break;
            case DEVICE_STATE:
                if (callbacks.onDeviceState != null) {
                    callbacks.onDeviceState.handle(DeviceStateMessage.decode(payload));
                }
                break;
            case ENTER_DEVICE_CHILD:
                if (callbacks.onEnterDeviceChild != null) {
                    callbacks.onEnterDeviceChild.handle(EnterDeviceChildMessage.decode(payload));
                }
                break;
            case EXIT_TO_PARENT:
                if (callbacks.onExitToParent != null) {
                    callbacks.onExitToParent.handle(ExitToParentMessage.decode(payload));
                }
                break;
            case REMOTE_CONTROL_VALUE:
                if (callbacks.onRemoteControlValue != null) {
                    callbacks.onRemoteControlValue.handle(RemoteControlValueMessage.decode(payload));
                }
                break;
            case REMOTE_CONTROL_VALUE_STATE:
                if (callbacks.onRemoteControlValueState != null) {
                    callbacks.onRemoteControlValueState.handle(RemoteControlValueStateMessage.decode(payload));
                }
                break;
            case REQUEST_DEVICE_CHILDREN:
                if (callbacks.onRequestDeviceChildren != null) {
                    callbacks.onRequestDeviceChildren.handle(RequestDeviceChildrenMessage.decode(payload));
                }
                break;
            case REQUEST_DEVICE_LIST_WINDOW:
                if (callbacks.onRequestDeviceListWindow != null) {
                    callbacks.onRequestDeviceListWindow.handle(RequestDeviceListWindowMessage.decode(payload));
                }
                break;
            case REQUEST_DEVICE_PAGE_NAMES_WINDOW:
                if (callbacks.onRequestDevicePageNamesWindow != null) {
                    callbacks.onRequestDevicePageNamesWindow.handle(RequestDevicePageNamesWindowMessage.decode(payload));
                }
                break;
            case VIEW_STATE:
                if (callbacks.onViewState != null) {
                    callbacks.onViewState.handle(ViewStateMessage.decode(payload));
                }
                break;
            case LAST_CLICKED_TOUCH:
                if (callbacks.onLastClickedTouch != null) {
                    callbacks.onLastClickedTouch.handle(LastClickedTouchMessage.decode(payload));
                }
                break;
            case LAST_CLICKED_UPDATE:
                if (callbacks.onLastClickedUpdate != null) {
                    callbacks.onLastClickedUpdate.handle(LastClickedUpdateMessage.decode(payload));
                }
                break;
            case LAST_CLICKED_VALUE:
                if (callbacks.onLastClickedValue != null) {
                    callbacks.onLastClickedValue.handle(LastClickedValueMessage.decode(payload));
                }
                break;
            case LAST_CLICKED_VALUE_STATE:
                if (callbacks.onLastClickedValueState != null) {
                    callbacks.onLastClickedValueState.handle(LastClickedValueStateMessage.decode(payload));
                }
                break;
            case HOST_DEACTIVATED:
                if (callbacks.onHostDeactivated != null) {
                    callbacks.onHostDeactivated.handle(HostDeactivatedMessage.decode(payload));
                }
                break;
            case HOST_INITIALIZED:
                if (callbacks.onHostInitialized != null) {
                    callbacks.onHostInitialized.handle(HostInitializedMessage.decode(payload));
                }
                break;
            case REQUEST_HOST_STATUS:
                if (callbacks.onRequestHostStatus != null) {
                    callbacks.onRequestHostStatus.handle(RequestHostStatusMessage.decode(payload));
                }
                break;
            case ENTER_TRACK_GROUP:
                if (callbacks.onEnterTrackGroup != null) {
                    callbacks.onEnterTrackGroup.handle(EnterTrackGroupMessage.decode(payload));
                }
                break;
            case EXIT_TRACK_GROUP:
                if (callbacks.onExitTrackGroup != null) {
                    callbacks.onExitTrackGroup.handle(ExitTrackGroupMessage.decode(payload));
                }
                break;
            case REQUEST_SEND_DESTINATIONS:
                if (callbacks.onRequestSendDestinations != null) {
                    callbacks.onRequestSendDestinations.handle(RequestSendDestinationsMessage.decode(payload));
                }
                break;
            case REQUEST_TRACK_LIST_WINDOW:
                if (callbacks.onRequestTrackListWindow != null) {
                    callbacks.onRequestTrackListWindow.handle(RequestTrackListWindowMessage.decode(payload));
                }
                break;
            case REQUEST_TRACK_SEND_LIST:
                if (callbacks.onRequestTrackSendList != null) {
                    callbacks.onRequestTrackSendList.handle(RequestTrackSendListMessage.decode(payload));
                }
                break;
            case SELECT_MIX_SEND:
                if (callbacks.onSelectMixSend != null) {
                    callbacks.onSelectMixSend.handle(SelectMixSendMessage.decode(payload));
                }
                break;
            case SEND_DESTINATIONS_LIST:
                if (callbacks.onSendDestinationsList != null) {
                    callbacks.onSendDestinationsList.handle(SendDestinationsListMessage.decode(payload));
                }
                break;
            case TRACK_ACTIVATE:
                if (callbacks.onTrackActivate != null) {
                    callbacks.onTrackActivate.handle(TrackActivateMessage.decode(payload));
                }
                break;
            case TRACK_ARM:
                if (callbacks.onTrackArm != null) {
                    callbacks.onTrackArm.handle(TrackArmMessage.decode(payload));
                }
                break;
            case TRACK_ARM_STATE:
                if (callbacks.onTrackArmState != null) {
                    callbacks.onTrackArmState.handle(TrackArmStateMessage.decode(payload));
                }
                break;
            case TRACK_CHANGE:
                if (callbacks.onTrackChange != null) {
                    callbacks.onTrackChange.handle(TrackChangeMessage.decode(payload));
                }
                break;
            case TRACK_LIST_WINDOW:
                if (callbacks.onTrackListWindow != null) {
                    callbacks.onTrackListWindow.handle(TrackListWindowMessage.decode(payload));
                }
                break;
            case TRACK_MUTE:
                if (callbacks.onTrackMute != null) {
                    callbacks.onTrackMute.handle(TrackMuteMessage.decode(payload));
                }
                break;
            case TRACK_MUTED_BY_SOLO_STATE:
                if (callbacks.onTrackMutedBySoloState != null) {
                    callbacks.onTrackMutedBySoloState.handle(TrackMutedBySoloStateMessage.decode(payload));
                }
                break;
            case TRACK_MUTE_STATE:
                if (callbacks.onTrackMuteState != null) {
                    callbacks.onTrackMuteState.handle(TrackMuteStateMessage.decode(payload));
                }
                break;
            case TRACK_PAN:
                if (callbacks.onTrackPan != null) {
                    callbacks.onTrackPan.handle(TrackPanMessage.decode(payload));
                }
                break;
            case TRACK_PAN_HAS_AUTOMATION_STATE:
                if (callbacks.onTrackPanHasAutomationState != null) {
                    callbacks.onTrackPanHasAutomationState.handle(TrackPanHasAutomationStateMessage.decode(payload));
                }
                break;
            case TRACK_PAN_MODULATED_VALUE_STATE:
                if (callbacks.onTrackPanModulatedValueState != null) {
                    callbacks.onTrackPanModulatedValueState.handle(TrackPanModulatedValueStateMessage.decode(payload));
                }
                break;
            case TRACK_PAN_STATE:
                if (callbacks.onTrackPanState != null) {
                    callbacks.onTrackPanState.handle(TrackPanStateMessage.decode(payload));
                }
                break;
            case TRACK_PAN_TOUCH:
                if (callbacks.onTrackPanTouch != null) {
                    callbacks.onTrackPanTouch.handle(TrackPanTouchMessage.decode(payload));
                }
                break;
            case TRACK_SELECT:
                if (callbacks.onTrackSelect != null) {
                    callbacks.onTrackSelect.handle(TrackSelectMessage.decode(payload));
                }
                break;
            case TRACK_SEND_ENABLED:
                if (callbacks.onTrackSendEnabled != null) {
                    callbacks.onTrackSendEnabled.handle(TrackSendEnabledMessage.decode(payload));
                }
                break;
            case TRACK_SEND_ENABLED_STATE:
                if (callbacks.onTrackSendEnabledState != null) {
                    callbacks.onTrackSendEnabledState.handle(TrackSendEnabledStateMessage.decode(payload));
                }
                break;
            case TRACK_SEND_HAS_AUTOMATION_STATE:
                if (callbacks.onTrackSendHasAutomationState != null) {
                    callbacks.onTrackSendHasAutomationState.handle(TrackSendHasAutomationStateMessage.decode(payload));
                }
                break;
            case TRACK_SEND_LIST:
                if (callbacks.onTrackSendList != null) {
                    callbacks.onTrackSendList.handle(TrackSendListMessage.decode(payload));
                }
                break;
            case TRACK_SEND_MODE:
                if (callbacks.onTrackSendMode != null) {
                    callbacks.onTrackSendMode.handle(TrackSendModeMessage.decode(payload));
                }
                break;
            case TRACK_SEND_MODE_STATE:
                if (callbacks.onTrackSendModeState != null) {
                    callbacks.onTrackSendModeState.handle(TrackSendModeStateMessage.decode(payload));
                }
                break;
            case TRACK_SEND_MODULATED_VALUE_STATE:
                if (callbacks.onTrackSendModulatedValueState != null) {
                    callbacks.onTrackSendModulatedValueState.handle(TrackSendModulatedValueStateMessage.decode(payload));
                }
                break;
            case TRACK_SEND_PRE_FADER_STATE:
                if (callbacks.onTrackSendPreFaderState != null) {
                    callbacks.onTrackSendPreFaderState.handle(TrackSendPreFaderStateMessage.decode(payload));
                }
                break;
            case TRACK_SEND_TOUCH:
                if (callbacks.onTrackSendTouch != null) {
                    callbacks.onTrackSendTouch.handle(TrackSendTouchMessage.decode(payload));
                }
                break;
            case TRACK_SEND_VALUE:
                if (callbacks.onTrackSendValue != null) {
                    callbacks.onTrackSendValue.handle(TrackSendValueMessage.decode(payload));
                }
                break;
            case TRACK_SEND_VALUE_STATE:
                if (callbacks.onTrackSendValueState != null) {
                    callbacks.onTrackSendValueState.handle(TrackSendValueStateMessage.decode(payload));
                }
                break;
            case TRACK_SOLO:
                if (callbacks.onTrackSolo != null) {
                    callbacks.onTrackSolo.handle(TrackSoloMessage.decode(payload));
                }
                break;
            case TRACK_SOLO_STATE:
                if (callbacks.onTrackSoloState != null) {
                    callbacks.onTrackSoloState.handle(TrackSoloStateMessage.decode(payload));
                }
                break;
            case TRACK_VOLUME:
                if (callbacks.onTrackVolume != null) {
                    callbacks.onTrackVolume.handle(TrackVolumeMessage.decode(payload));
                }
                break;
            case TRACK_VOLUME_HAS_AUTOMATION_STATE:
                if (callbacks.onTrackVolumeHasAutomationState != null) {
                    callbacks.onTrackVolumeHasAutomationState.handle(TrackVolumeHasAutomationStateMessage.decode(payload));
                }
                break;
            case TRACK_VOLUME_MODULATED_VALUE_STATE:
                if (callbacks.onTrackVolumeModulatedValueState != null) {
                    callbacks.onTrackVolumeModulatedValueState.handle(TrackVolumeModulatedValueStateMessage.decode(payload));
                }
                break;
            case TRACK_VOLUME_STATE:
                if (callbacks.onTrackVolumeState != null) {
                    callbacks.onTrackVolumeState.handle(TrackVolumeStateMessage.decode(payload));
                }
                break;
            case TRACK_VOLUME_TOUCH:
                if (callbacks.onTrackVolumeTouch != null) {
                    callbacks.onTrackVolumeTouch.handle(TrackVolumeTouchMessage.decode(payload));
                }
                break;
            case RESET_AUTOMATION_OVERRIDES:
                if (callbacks.onResetAutomationOverrides != null) {
                    callbacks.onResetAutomationOverrides.handle(ResetAutomationOverridesMessage.decode(payload));
                }
                break;
            case TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED:
                if (callbacks.onTransportArrangerAutomationWriteEnabled != null) {
                    callbacks.onTransportArrangerAutomationWriteEnabled.handle(TransportArrangerAutomationWriteEnabledMessage.decode(payload));
                }
                break;
            case TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE:
                if (callbacks.onTransportArrangerAutomationWriteEnabledState != null) {
                    callbacks.onTransportArrangerAutomationWriteEnabledState.handle(TransportArrangerAutomationWriteEnabledStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_ARRANGER_OVERDUB_ENABLED:
                if (callbacks.onTransportArrangerOverdubEnabled != null) {
                    callbacks.onTransportArrangerOverdubEnabled.handle(TransportArrangerOverdubEnabledMessage.decode(payload));
                }
                break;
            case TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE:
                if (callbacks.onTransportArrangerOverdubEnabledState != null) {
                    callbacks.onTransportArrangerOverdubEnabledState.handle(TransportArrangerOverdubEnabledStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE:
                if (callbacks.onTransportAutomationOverrideActiveState != null) {
                    callbacks.onTransportAutomationOverrideActiveState.handle(TransportAutomationOverrideActiveStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_AUTOMATION_WRITE_MODE:
                if (callbacks.onTransportAutomationWriteMode != null) {
                    callbacks.onTransportAutomationWriteMode.handle(TransportAutomationWriteModeMessage.decode(payload));
                }
                break;
            case TRANSPORT_AUTOMATION_WRITE_MODE_STATE:
                if (callbacks.onTransportAutomationWriteModeState != null) {
                    callbacks.onTransportAutomationWriteModeState.handle(TransportAutomationWriteModeStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED:
                if (callbacks.onTransportClipLauncherAutomationWriteEnabled != null) {
                    callbacks.onTransportClipLauncherAutomationWriteEnabled.handle(TransportClipLauncherAutomationWriteEnabledMessage.decode(payload));
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_STATE:
                if (callbacks.onTransportClipLauncherAutomationWriteEnabledState != null) {
                    callbacks.onTransportClipLauncherAutomationWriteEnabledState.handle(TransportClipLauncherAutomationWriteEnabledStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED:
                if (callbacks.onTransportClipLauncherOverdubEnabled != null) {
                    callbacks.onTransportClipLauncherOverdubEnabled.handle(TransportClipLauncherOverdubEnabledMessage.decode(payload));
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE:
                if (callbacks.onTransportClipLauncherOverdubEnabledState != null) {
                    callbacks.onTransportClipLauncherOverdubEnabledState.handle(TransportClipLauncherOverdubEnabledStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_PLAY:
                if (callbacks.onTransportPlay != null) {
                    callbacks.onTransportPlay.handle(TransportPlayMessage.decode(payload));
                }
                break;
            case TRANSPORT_PLAYING_STATE:
                if (callbacks.onTransportPlayingState != null) {
                    callbacks.onTransportPlayingState.handle(TransportPlayingStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_RECORD:
                if (callbacks.onTransportRecord != null) {
                    callbacks.onTransportRecord.handle(TransportRecordMessage.decode(payload));
                }
                break;
            case TRANSPORT_RECORDING_STATE:
                if (callbacks.onTransportRecordingState != null) {
                    callbacks.onTransportRecordingState.handle(TransportRecordingStateMessage.decode(payload));
                }
                break;
            case TRANSPORT_STOP:
                if (callbacks.onTransportStop != null) {
                    callbacks.onTransportStop.handle(TransportStopMessage.decode(payload));
                }
                break;
            case TRANSPORT_TEMPO:
                if (callbacks.onTransportTempo != null) {
                    callbacks.onTransportTempo.handle(TransportTempoMessage.decode(payload));
                }
                break;
            case TRANSPORT_TEMPO_STATE:
                if (callbacks.onTransportTempoState != null) {
                    callbacks.onTransportTempoState.handle(TransportTempoStateMessage.decode(payload));
                }
                break;
            default:
                // Unknown message type - silently ignore
                break;
        }
    }

    // Utility class - prevent instantiation
    private DecoderRegistry() {}
}
