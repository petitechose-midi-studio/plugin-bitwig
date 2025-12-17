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
            case DEVICE_LIST:
                if (callbacks.onDeviceList != null) {
                    DeviceListMessage msg = DeviceListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceList.handle(msg);
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
            case DEVICE_PAGE_NAMES:
                if (callbacks.onDevicePageNames != null) {
                    DevicePageNamesMessage msg = DevicePageNamesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageNames.handle(msg);
                }
                break;
            case DEVICE_PAGE_NAMES_WINDOW:
                if (callbacks.onDevicePageNamesWindow != null) {
                    DevicePageNamesWindowMessage msg = DevicePageNamesWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageNamesWindow.handle(msg);
                }
                break;
            case DEVICE_PAGE_SELECT_BY_INDEX:
                if (callbacks.onDevicePageSelectByIndex != null) {
                    DevicePageSelectByIndexMessage msg = DevicePageSelectByIndexMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageSelectByIndex.handle(msg);
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
            case DEVICE_REMOTE_CONTROL_MODULATED_VALUE_CHANGE:
                if (callbacks.onDeviceRemoteControlModulatedValueChange != null) {
                    DeviceRemoteControlModulatedValueChangeMessage msg = DeviceRemoteControlModulatedValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlModulatedValueChange.handle(msg);
                }
                break;
            case DEVICE_REMOTE_CONTROL_NAME_CHANGE:
                if (callbacks.onDeviceRemoteControlNameChange != null) {
                    DeviceRemoteControlNameChangeMessage msg = DeviceRemoteControlNameChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlNameChange.handle(msg);
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
            case DEVICE_REMOTE_CONTROL_VALUE_CHANGE:
                if (callbacks.onDeviceRemoteControlValueChange != null) {
                    DeviceRemoteControlValueChangeMessage msg = DeviceRemoteControlValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceRemoteControlValueChange.handle(msg);
                }
                break;
            case DEVICE_SELECT_BY_INDEX:
                if (callbacks.onDeviceSelectByIndex != null) {
                    DeviceSelectByIndexMessage msg = DeviceSelectByIndexMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceSelectByIndex.handle(msg);
                }
                break;
            case DEVICE_STATE_CHANGE:
                if (callbacks.onDeviceStateChange != null) {
                    DeviceStateChangeMessage msg = DeviceStateChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceStateChange.handle(msg);
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
            case REQUEST_DEVICE_CHILDREN:
                if (callbacks.onRequestDeviceChildren != null) {
                    RequestDeviceChildrenMessage msg = RequestDeviceChildrenMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceChildren.handle(msg);
                }
                break;
            case REQUEST_DEVICE_LIST:
                if (callbacks.onRequestDeviceList != null) {
                    RequestDeviceListMessage msg = RequestDeviceListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceList.handle(msg);
                }
                break;
            case REQUEST_DEVICE_LIST_WINDOW:
                if (callbacks.onRequestDeviceListWindow != null) {
                    RequestDeviceListWindowMessage msg = RequestDeviceListWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceListWindow.handle(msg);
                }
                break;
            case REQUEST_DEVICE_PAGE_NAMES:
                if (callbacks.onRequestDevicePageNames != null) {
                    RequestDevicePageNamesMessage msg = RequestDevicePageNamesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDevicePageNames.handle(msg);
                }
                break;
            case REQUEST_DEVICE_PAGE_NAMES_WINDOW:
                if (callbacks.onRequestDevicePageNamesWindow != null) {
                    RequestDevicePageNamesWindowMessage msg = RequestDevicePageNamesWindowMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDevicePageNamesWindow.handle(msg);
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
            case LAST_CLICKED_VALUE_CHANGE:
                if (callbacks.onLastClickedValueChange != null) {
                    LastClickedValueChangeMessage msg = LastClickedValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedValueChange.handle(msg);
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
            case REQUEST_TRACK_LIST:
                if (callbacks.onRequestTrackList != null) {
                    RequestTrackListMessage msg = RequestTrackListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackList.handle(msg);
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
            case TRACK_ARM_CHANGE:
                if (callbacks.onTrackArmChange != null) {
                    TrackArmChangeMessage msg = TrackArmChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackArmChange.handle(msg);
                }
                break;
            case TRACK_CHANGE:
                if (callbacks.onTrackChange != null) {
                    TrackChangeMessage msg = TrackChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackChange.handle(msg);
                }
                break;
            case TRACK_LIST:
                if (callbacks.onTrackList != null) {
                    TrackListMessage msg = TrackListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackList.handle(msg);
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
            case TRACK_MUTED_BY_SOLO_CHANGE:
                if (callbacks.onTrackMutedBySoloChange != null) {
                    TrackMutedBySoloChangeMessage msg = TrackMutedBySoloChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMutedBySoloChange.handle(msg);
                }
                break;
            case TRACK_PAN_CHANGE:
                if (callbacks.onTrackPanChange != null) {
                    TrackPanChangeMessage msg = TrackPanChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanChange.handle(msg);
                }
                break;
            case TRACK_PAN_HAS_AUTOMATION_CHANGE:
                if (callbacks.onTrackPanHasAutomationChange != null) {
                    TrackPanHasAutomationChangeMessage msg = TrackPanHasAutomationChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanHasAutomationChange.handle(msg);
                }
                break;
            case TRACK_PAN_MODULATED_VALUE_CHANGE:
                if (callbacks.onTrackPanModulatedValueChange != null) {
                    TrackPanModulatedValueChangeMessage msg = TrackPanModulatedValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanModulatedValueChange.handle(msg);
                }
                break;
            case TRACK_PAN_TOUCH:
                if (callbacks.onTrackPanTouch != null) {
                    TrackPanTouchMessage msg = TrackPanTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackPanTouch.handle(msg);
                }
                break;
            case TRACK_SELECT_BY_INDEX:
                if (callbacks.onTrackSelectByIndex != null) {
                    TrackSelectByIndexMessage msg = TrackSelectByIndexMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSelectByIndex.handle(msg);
                }
                break;
            case TRACK_SEND_ENABLED_CHANGE:
                if (callbacks.onTrackSendEnabledChange != null) {
                    TrackSendEnabledChangeMessage msg = TrackSendEnabledChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendEnabledChange.handle(msg);
                }
                break;
            case TRACK_SEND_HAS_AUTOMATION_CHANGE:
                if (callbacks.onTrackSendHasAutomationChange != null) {
                    TrackSendHasAutomationChangeMessage msg = TrackSendHasAutomationChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendHasAutomationChange.handle(msg);
                }
                break;
            case TRACK_SEND_LIST:
                if (callbacks.onTrackSendList != null) {
                    TrackSendListMessage msg = TrackSendListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendList.handle(msg);
                }
                break;
            case TRACK_SEND_MODE_CHANGE:
                if (callbacks.onTrackSendModeChange != null) {
                    TrackSendModeChangeMessage msg = TrackSendModeChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendModeChange.handle(msg);
                }
                break;
            case TRACK_SEND_MODULATED_VALUE_CHANGE:
                if (callbacks.onTrackSendModulatedValueChange != null) {
                    TrackSendModulatedValueChangeMessage msg = TrackSendModulatedValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendModulatedValueChange.handle(msg);
                }
                break;
            case TRACK_SEND_PRE_FADER_CHANGE:
                if (callbacks.onTrackSendPreFaderChange != null) {
                    TrackSendPreFaderChangeMessage msg = TrackSendPreFaderChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendPreFaderChange.handle(msg);
                }
                break;
            case TRACK_SEND_TOUCH:
                if (callbacks.onTrackSendTouch != null) {
                    TrackSendTouchMessage msg = TrackSendTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendTouch.handle(msg);
                }
                break;
            case TRACK_SEND_VALUE_CHANGE:
                if (callbacks.onTrackSendValueChange != null) {
                    TrackSendValueChangeMessage msg = TrackSendValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSendValueChange.handle(msg);
                }
                break;
            case TRACK_SOLO:
                if (callbacks.onTrackSolo != null) {
                    TrackSoloMessage msg = TrackSoloMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSolo.handle(msg);
                }
                break;
            case TRACK_VOLUME_CHANGE:
                if (callbacks.onTrackVolumeChange != null) {
                    TrackVolumeChangeMessage msg = TrackVolumeChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeChange.handle(msg);
                }
                break;
            case TRACK_VOLUME_HAS_AUTOMATION_CHANGE:
                if (callbacks.onTrackVolumeHasAutomationChange != null) {
                    TrackVolumeHasAutomationChangeMessage msg = TrackVolumeHasAutomationChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeHasAutomationChange.handle(msg);
                }
                break;
            case TRACK_VOLUME_MODULATED_VALUE_CHANGE:
                if (callbacks.onTrackVolumeModulatedValueChange != null) {
                    TrackVolumeModulatedValueChangeMessage msg = TrackVolumeModulatedValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackVolumeModulatedValueChange.handle(msg);
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
            case TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE:
                if (callbacks.onTransportArrangerAutomationWriteEnabledChange != null) {
                    TransportArrangerAutomationWriteEnabledChangeMessage msg = TransportArrangerAutomationWriteEnabledChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerAutomationWriteEnabledChange.handle(msg);
                }
                break;
            case TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE:
                if (callbacks.onTransportArrangerOverdubEnabledChange != null) {
                    TransportArrangerOverdubEnabledChangeMessage msg = TransportArrangerOverdubEnabledChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportArrangerOverdubEnabledChange.handle(msg);
                }
                break;
            case TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE:
                if (callbacks.onTransportAutomationOverrideActiveChange != null) {
                    TransportAutomationOverrideActiveChangeMessage msg = TransportAutomationOverrideActiveChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationOverrideActiveChange.handle(msg);
                }
                break;
            case TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE:
                if (callbacks.onTransportAutomationWriteModeChange != null) {
                    TransportAutomationWriteModeChangeMessage msg = TransportAutomationWriteModeChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportAutomationWriteModeChange.handle(msg);
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE:
                if (callbacks.onTransportClipLauncherAutomationWriteEnabledChange != null) {
                    TransportClipLauncherAutomationWriteEnabledChangeMessage msg = TransportClipLauncherAutomationWriteEnabledChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherAutomationWriteEnabledChange.handle(msg);
                }
                break;
            case TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE:
                if (callbacks.onTransportClipLauncherOverdubEnabledChange != null) {
                    TransportClipLauncherOverdubEnabledChangeMessage msg = TransportClipLauncherOverdubEnabledChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportClipLauncherOverdubEnabledChange.handle(msg);
                }
                break;
            case TRANSPORT_PLAY:
                if (callbacks.onTransportPlay != null) {
                    TransportPlayMessage msg = TransportPlayMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportPlay.handle(msg);
                }
                break;
            case TRANSPORT_RECORD:
                if (callbacks.onTransportRecord != null) {
                    TransportRecordMessage msg = TransportRecordMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportRecord.handle(msg);
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
            default:
                // Unknown message type - silently ignore
                break;
        }
    }

    // Utility class - prevent instantiation
    private DecoderRegistry() {}
}
