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
     * @param fromHost Origin flag (true if message from host, false if from controller)
     */
    public static void dispatch(
        ProtocolCallbacks callbacks,
        MessageID messageId,
        byte[] payload,
        boolean fromHost
    ) {
        switch (messageId) {
            case DEVICE_CHANGE:
                if (callbacks.onDeviceChange != null) {
                    DeviceChangeMessage msg = DeviceChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChange.handle(msg);
                }
                break;
            case DEVICE_CHANGE_HEADER:
                if (callbacks.onDeviceChangeHeader != null) {
                    DeviceChangeHeaderMessage msg = DeviceChangeHeaderMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChangeHeader.handle(msg);
                }
                break;
            case DEVICE_CHILDREN:
                if (callbacks.onDeviceChildren != null) {
                    DeviceChildrenMessage msg = DeviceChildrenMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChildren.handle(msg);
                }
                break;
            case DEVICE_ENABLED_STATE:
                if (callbacks.onDeviceEnabledState != null) {
                    DeviceEnabledStateMessage msg = DeviceEnabledStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceEnabledState.handle(msg);
                }
                break;
            case DEVICE_LIST_WINDOW:
                if (callbacks.onDeviceListWindow != null) {
                    DeviceListWindowMessage msg = DeviceListWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceListWindow.handle(msg);
                }
                break;
            case DEVICE_PAGE_CHANGE:
                if (callbacks.onDevicePageChange != null) {
                    DevicePageChangeMessage msg = DevicePageChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageChange.handle(msg);
                }
                break;
            case DEVICE_PAGE_NAMES_WINDOW:
                if (callbacks.onDevicePageNamesWindow != null) {
                    DevicePageNamesWindowMessage msg = DevicePageNamesWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageNamesWindow.handle(msg);
                }
                break;
            case DEVICE_PAGE_SELECT:
                if (callbacks.onDevicePageSelect != null) {
                    DevicePageSelectMessage msg = DevicePageSelectMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageSelect.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROLS_BATCH:
                if (callbacks.onDeviceRemoteControlsBatch != null) {
                    DeviceRemoteControlsBatchMessage msg = DeviceRemoteControlsBatchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlsBatch.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_DISCRETE_VALUES:
                if (callbacks.onDeviceRemoteControlDiscreteValues != null) {
                    DeviceRemoteControlDiscreteValuesMessage msg = DeviceRemoteControlDiscreteValuesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlDiscreteValues.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE:
                if (callbacks.onDeviceRemoteControlHasAutomationChange != null) {
                    DeviceRemoteControlHasAutomationChangeMessage msg = DeviceRemoteControlHasAutomationChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlHasAutomationChange.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE:
                if (callbacks.onDeviceRemoteControlIsModulatedChange != null) {
                    DeviceRemoteControlIsModulatedChangeMessage msg = DeviceRemoteControlIsModulatedChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlIsModulatedChange.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_NAME_CHANGE:
                if (callbacks.onDeviceRemoteControlNameChange != null) {
                    DeviceRemoteControlNameChangeMessage msg = DeviceRemoteControlNameChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlNameChange.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE:
                if (callbacks.onDeviceRemoteControlOriginChange != null) {
                    DeviceRemoteControlOriginChangeMessage msg = DeviceRemoteControlOriginChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlOriginChange.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION:
                if (callbacks.onDeviceRemoteControlRestoreAutomation != null) {
                    DeviceRemoteControlRestoreAutomationMessage msg = DeviceRemoteControlRestoreAutomationMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlRestoreAutomation.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_TOUCH:
                if (callbacks.onDeviceRemoteControlTouch != null) {
                    DeviceRemoteControlTouchMessage msg = DeviceRemoteControlTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlTouch.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_UPDATE:
                if (callbacks.onDeviceRemoteControlUpdate != null) {
                    DeviceRemoteControlUpdateMessage msg = DeviceRemoteControlUpdateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlUpdate.handle(msg);
                }
                break;
            case DEVICE_SELECT:
                if (callbacks.onDeviceSelect != null) {
                    DeviceSelectMessage msg = DeviceSelectMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceSelect.handle(msg);
                }
                break;
            case DEVICE_STATE:
                if (callbacks.onDeviceState != null) {
                    DeviceStateMessage msg = DeviceStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceState.handle(msg);
                }
                break;
            case ENTER_DEVICE_CHILD:
                if (callbacks.onEnterDeviceChild != null) {
                    EnterDeviceChildMessage msg = EnterDeviceChildMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onEnterDeviceChild.handle(msg);
                }
                break;
            case EXIT_TO_PARENT:
                if (callbacks.onExitToParent != null) {
                    ExitToParentMessage msg = ExitToParentMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onExitToParent.handle(msg);
                }
                break;
            case REMOTE_CONTROL_VALUE:
                if (callbacks.onRemoteControlValue != null) {
                    RemoteControlValueMessage msg = RemoteControlValueMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRemoteControlValue.handle(msg);
                }
                break;
            case REMOTE_CONTROL_VALUE_STATE:
                if (callbacks.onRemoteControlValueState != null) {
                    RemoteControlValueStateMessage msg = RemoteControlValueStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRemoteControlValueState.handle(msg);
                }
                break;
            case REQUEST_DEVICE_CHILDREN:
                if (callbacks.onRequestDeviceChildren != null) {
                    RequestDeviceChildrenMessage msg = RequestDeviceChildrenMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceChildren.handle(msg);
                }
                break;
            case REQUEST_DEVICE_LIST_WINDOW:
                if (callbacks.onRequestDeviceListWindow != null) {
                    RequestDeviceListWindowMessage msg = RequestDeviceListWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceListWindow.handle(msg);
                }
                break;
            case REQUEST_DEVICE_PAGE_NAMES_WINDOW:
                if (callbacks.onRequestDevicePageNamesWindow != null) {
                    RequestDevicePageNamesWindowMessage msg = RequestDevicePageNamesWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDevicePageNamesWindow.handle(msg);
                }
                break;
            case VIEW_STATE:
                if (callbacks.onViewState != null) {
                    ViewStateMessage msg = ViewStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onViewState.handle(msg);
                }
                break;
            case LAST_CLICKED_TOUCH:
                if (callbacks.onLastClickedTouch != null) {
                    LastClickedTouchMessage msg = LastClickedTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedTouch.handle(msg);
                }
                break;
            case LAST_CLICKED_UPDATE:
                if (callbacks.onLastClickedUpdate != null) {
                    LastClickedUpdateMessage msg = LastClickedUpdateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedUpdate.handle(msg);
                }
                break;
            case LAST_CLICKED_VALUE:
                if (callbacks.onLastClickedValue != null) {
                    LastClickedValueMessage msg = LastClickedValueMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedValue.handle(msg);
                }
                break;
            case LAST_CLICKED_VALUE_STATE:
                if (callbacks.onLastClickedValueState != null) {
                    LastClickedValueStateMessage msg = LastClickedValueStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedValueState.handle(msg);
                }
                break;
            case HOST_DEACTIVATED:
                if (callbacks.onHostDeactivated != null) {
                    HostDeactivatedMessage msg = HostDeactivatedMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onHostDeactivated.handle(msg);
                }
                break;
            case HOST_INITIALIZED:
                if (callbacks.onHostInitialized != null) {
                    HostInitializedMessage msg = HostInitializedMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onHostInitialized.handle(msg);
                }
                break;
            case REQUEST_HOST_STATUS:
                if (callbacks.onRequestHostStatus != null) {
                    RequestHostStatusMessage msg = RequestHostStatusMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestHostStatus.handle(msg);
                }
                break;
            case ENTER_TRACK_GROUP:
                if (callbacks.onEnterTrackGroup != null) {
                    EnterTrackGroupMessage msg = EnterTrackGroupMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onEnterTrackGroup.handle(msg);
                }
                break;
            case EXIT_TRACK_GROUP:
                if (callbacks.onExitTrackGroup != null) {
                    ExitTrackGroupMessage msg = ExitTrackGroupMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onExitTrackGroup.handle(msg);
                }
                break;
            case REQUEST_SEND_DESTINATIONS:
                if (callbacks.onRequestSendDestinations != null) {
                    RequestSendDestinationsMessage msg = RequestSendDestinationsMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestSendDestinations.handle(msg);
                }
                break;
            case REQUEST_TRACK_LIST_WINDOW:
                if (callbacks.onRequestTrackListWindow != null) {
                    RequestTrackListWindowMessage msg = RequestTrackListWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackListWindow.handle(msg);
                }
                break;
            case REQUEST_TRACK_SEND_LIST:
                if (callbacks.onRequestTrackSendList != null) {
                    RequestTrackSendListMessage msg = RequestTrackSendListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackSendList.handle(msg);
                }
                break;
            case SELECT_MIX_SEND:
                if (callbacks.onSelectMixSend != null) {
                    SelectMixSendMessage msg = SelectMixSendMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onSelectMixSend.handle(msg);
                }
                break;
            case SEND_DESTINATIONS_LIST:
                if (callbacks.onSendDestinationsList != null) {
                    SendDestinationsListMessage msg = SendDestinationsListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onSendDestinationsList.handle(msg);
                }
                break;
            case TRACK_ACTIVATE:
                if (callbacks.onTrackActivate != null) {
                    TrackActivateMessage msg = TrackActivateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackActivate.handle(msg);
                }
                break;
            case TRACK_ARM:
                if (callbacks.onTrackArm != null) {
                    TrackArmMessage msg = TrackArmMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackArm.handle(msg);
                }
                break;
            case TRACK_ARM_STATE:
                if (callbacks.onTrackArmState != null) {
                    TrackArmStateMessage msg = TrackArmStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackArmState.handle(msg);
                }
                break;
            case TRACK_CHANGE:
                if (callbacks.onTrackChange != null) {
                    TrackChangeMessage msg = TrackChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackChange.handle(msg);
                }
                break;
            case TRACK_LIST_WINDOW:
                if (callbacks.onTrackListWindow != null) {
                    TrackListWindowMessage msg = TrackListWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackListWindow.handle(msg);
                }
                break;
            case TRACK_MUTE:
                if (callbacks.onTrackMute != null) {
                    TrackMuteMessage msg = TrackMuteMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMute.handle(msg);
                }
                break;
            case TRACK_MUTED_BY_SOLO_STATE:
                if (callbacks.onTrackMutedBySoloState != null) {
                    TrackMutedBySoloStateMessage msg = TrackMutedBySoloStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMutedBySoloState.handle(msg);
                }
                break;
            case TRACK_MUTE_STATE:
                if (callbacks.onTrackMuteState != null) {
                    TrackMuteStateMessage msg = TrackMuteStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMuteState.handle(msg);
                }
                break;
            case TRACK_PAN:
                if (callbacks.onTrackPan != null) {
                    TrackPanMessage msg = TrackPanMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPan.handle(msg);
                }
                break;
            case TRACK_PAN_HAS_AUTOMATION_STATE:
                if (callbacks.onTrackPanHasAutomationState != null) {
                    TrackPanHasAutomationStateMessage msg = TrackPanHasAutomationStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanHasAutomationState.handle(msg);
                }
                break;
            case TRACK_PAN_MODULATED_VALUE_STATE:
                if (callbacks.onTrackPanModulatedValueState != null) {
                    TrackPanModulatedValueStateMessage msg = TrackPanModulatedValueStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanModulatedValueState.handle(msg);
                }
                break;
            case TRACK_PAN_STATE:
                if (callbacks.onTrackPanState != null) {
                    TrackPanStateMessage msg = TrackPanStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanState.handle(msg);
                }
                break;
            case TRACK_PAN_TOUCH:
                if (callbacks.onTrackPanTouch != null) {
                    TrackPanTouchMessage msg = TrackPanTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanTouch.handle(msg);
                }
                break;
            case TRACK_SELECT:
                if (callbacks.onTrackSelect != null) {
                    TrackSelectMessage msg = TrackSelectMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSelect.handle(msg);
                }
                break;
            case TRACK_SEND_ENABLED:
                if (callbacks.onTrackSendEnabled != null) {
                    TrackSendEnabledMessage msg = TrackSendEnabledMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendEnabled.handle(msg);
                }
                break;
            case TRACK_SEND_ENABLED_STATE:
                if (callbacks.onTrackSendEnabledState != null) {
                    TrackSendEnabledStateMessage msg = TrackSendEnabledStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendEnabledState.handle(msg);
                }
                break;
            case TRACK_SEND_HAS_AUTOMATION_STATE:
                if (callbacks.onTrackSendHasAutomationState != null) {
                    TrackSendHasAutomationStateMessage msg = TrackSendHasAutomationStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendHasAutomationState.handle(msg);
                }
                break;
            case TRACK_SEND_LIST:
                if (callbacks.onTrackSendList != null) {
                    TrackSendListMessage msg = TrackSendListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendList.handle(msg);
                }
                break;
            case TRACK_SEND_MODE:
                if (callbacks.onTrackSendMode != null) {
                    TrackSendModeMessage msg = TrackSendModeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendMode.handle(msg);
                }
                break;
            case TRACK_SEND_MODE_STATE:
                if (callbacks.onTrackSendModeState != null) {
                    TrackSendModeStateMessage msg = TrackSendModeStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendModeState.handle(msg);
                }
                break;
            case TRACK_SEND_MODULATED_VALUE_STATE:
                if (callbacks.onTrackSendModulatedValueState != null) {
                    TrackSendModulatedValueStateMessage msg = TrackSendModulatedValueStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendModulatedValueState.handle(msg);
                }
                break;
            case TRACK_SEND_PRE_FADER_STATE:
                if (callbacks.onTrackSendPreFaderState != null) {
                    TrackSendPreFaderStateMessage msg = TrackSendPreFaderStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendPreFaderState.handle(msg);
                }
                break;
            case TRACK_SEND_TOUCH:
                if (callbacks.onTrackSendTouch != null) {
                    TrackSendTouchMessage msg = TrackSendTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendTouch.handle(msg);
                }
                break;
            case TRACK_SEND_VALUE:
                if (callbacks.onTrackSendValue != null) {
                    TrackSendValueMessage msg = TrackSendValueMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendValue.handle(msg);
                }
                break;
            case TRACK_SEND_VALUE_STATE:
                if (callbacks.onTrackSendValueState != null) {
                    TrackSendValueStateMessage msg = TrackSendValueStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendValueState.handle(msg);
                }
                break;
            case TRACK_SOLO:
                if (callbacks.onTrackSolo != null) {
                    TrackSoloMessage msg = TrackSoloMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSolo.handle(msg);
                }
                break;
            case TRACK_SOLO_STATE:
                if (callbacks.onTrackSoloState != null) {
                    TrackSoloStateMessage msg = TrackSoloStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSoloState.handle(msg);
                }
                break;
            case TRACK_VOLUME:
                if (callbacks.onTrackVolume != null) {
                    TrackVolumeMessage msg = TrackVolumeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolume.handle(msg);
                }
                break;
            case TRACK_VOLUME_HAS_AUTOMATION_STATE:
                if (callbacks.onTrackVolumeHasAutomationState != null) {
                    TrackVolumeHasAutomationStateMessage msg = TrackVolumeHasAutomationStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeHasAutomationState.handle(msg);
                }
                break;
            case TRACK_VOLUME_MODULATED_VALUE_STATE:
                if (callbacks.onTrackVolumeModulatedValueState != null) {
                    TrackVolumeModulatedValueStateMessage msg = TrackVolumeModulatedValueStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeModulatedValueState.handle(msg);
                }
                break;
            case TRACK_VOLUME_STATE:
                if (callbacks.onTrackVolumeState != null) {
                    TrackVolumeStateMessage msg = TrackVolumeStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeState.handle(msg);
                }
                break;
            case TRACK_VOLUME_TOUCH:
                if (callbacks.onTrackVolumeTouch != null) {
                    TrackVolumeTouchMessage msg = TrackVolumeTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeTouch.handle(msg);
                }
                break;
            case RESET_AUTOMATION_OVERRIDES:
                if (callbacks.onResetAutomationOverrides != null) {
                    ResetAutomationOverridesMessage msg = ResetAutomationOverridesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onResetAutomationOverrides.handle(msg);
                }
                break;
            case TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED:
                if (callbacks.onTransportArrangerAutomationWriteEnabled != null) {
                    TransportArrangerAutomationWriteEnabledMessage msg = TransportArrangerAutomationWriteEnabledMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerAutomationWriteEnabled.handle(msg);
                }
                break;
            case TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE:
                if (callbacks.onTransportArrangerAutomationWriteEnabledState != null) {
                    TransportArrangerAutomationWriteEnabledStateMessage msg = TransportArrangerAutomationWriteEnabledStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerAutomationWriteEnabledState.handle(msg);
                }
                break;
            case TRANSPORT_ARRANGER_OVERDUB_ENABLED:
                if (callbacks.onTransportArrangerOverdubEnabled != null) {
                    TransportArrangerOverdubEnabledMessage msg = TransportArrangerOverdubEnabledMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerOverdubEnabled.handle(msg);
                }
                break;
            case TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE:
                if (callbacks.onTransportArrangerOverdubEnabledState != null) {
                    TransportArrangerOverdubEnabledStateMessage msg = TransportArrangerOverdubEnabledStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerOverdubEnabledState.handle(msg);
                }
                break;
            case TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE:
                if (callbacks.onTransportAutomationOverrideActiveState != null) {
                    TransportAutomationOverrideActiveStateMessage msg = TransportAutomationOverrideActiveStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationOverrideActiveState.handle(msg);
                }
                break;
            case TRANSPORT_AUTOMATION_WRITE_MODE:
                if (callbacks.onTransportAutomationWriteMode != null) {
                    TransportAutomationWriteModeMessage msg = TransportAutomationWriteModeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationWriteMode.handle(msg);
                }
                break;
            case TRANSPORT_AUTOMATION_WRITE_MODE_STATE:
                if (callbacks.onTransportAutomationWriteModeState != null) {
                    TransportAutomationWriteModeStateMessage msg = TransportAutomationWriteModeStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationWriteModeState.handle(msg);
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED:
                if (callbacks.onTransportClipLauncherAutomationWriteEnabled != null) {
                    TransportClipLauncherAutomationWriteEnabledMessage msg = TransportClipLauncherAutomationWriteEnabledMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherAutomationWriteEnabled.handle(msg);
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_STATE:
                if (callbacks.onTransportClipLauncherAutomationWriteEnabledState != null) {
                    TransportClipLauncherAutomationWriteEnabledStateMessage msg = TransportClipLauncherAutomationWriteEnabledStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherAutomationWriteEnabledState.handle(msg);
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED:
                if (callbacks.onTransportClipLauncherOverdubEnabled != null) {
                    TransportClipLauncherOverdubEnabledMessage msg = TransportClipLauncherOverdubEnabledMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherOverdubEnabled.handle(msg);
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE:
                if (callbacks.onTransportClipLauncherOverdubEnabledState != null) {
                    TransportClipLauncherOverdubEnabledStateMessage msg = TransportClipLauncherOverdubEnabledStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherOverdubEnabledState.handle(msg);
                }
                break;
            case TRANSPORT_PLAY:
                if (callbacks.onTransportPlay != null) {
                    TransportPlayMessage msg = TransportPlayMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportPlay.handle(msg);
                }
                break;
            case TRANSPORT_PLAYING_STATE:
                if (callbacks.onTransportPlayingState != null) {
                    TransportPlayingStateMessage msg = TransportPlayingStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportPlayingState.handle(msg);
                }
                break;
            case TRANSPORT_RECORD:
                if (callbacks.onTransportRecord != null) {
                    TransportRecordMessage msg = TransportRecordMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportRecord.handle(msg);
                }
                break;
            case TRANSPORT_RECORDING_STATE:
                if (callbacks.onTransportRecordingState != null) {
                    TransportRecordingStateMessage msg = TransportRecordingStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportRecordingState.handle(msg);
                }
                break;
            case TRANSPORT_STOP:
                if (callbacks.onTransportStop != null) {
                    TransportStopMessage msg = TransportStopMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportStop.handle(msg);
                }
                break;
            case TRANSPORT_TEMPO:
                if (callbacks.onTransportTempo != null) {
                    TransportTempoMessage msg = TransportTempoMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportTempo.handle(msg);
                }
                break;
            case TRANSPORT_TEMPO_STATE:
                if (callbacks.onTransportTempoState != null) {
                    TransportTempoStateMessage msg = TransportTempoStateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportTempoState.handle(msg);
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
