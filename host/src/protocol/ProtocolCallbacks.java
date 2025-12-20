package protocol;

import protocol.struct.*;

/**
 * ProtocolCallbacks - Typed callbacks for all messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Base class providing typed callbacks for each message type.
 * Protocol extends this and DecoderRegistry calls these callbacks.
 *
 * Usage:
 *   protocol.onTransportPlay = msg -> {
 *       System.out.println("Playing: " + msg.isPlaying());
 *   };
 */
public class ProtocolCallbacks {

    /**
     * Functional interface for message handlers
     */
    @FunctionalInterface
    public interface MessageHandler<T> {
        void handle(T message);
    }

    // ========================================================================
    // Typed callbacks (one per message)
    // ========================================================================

    public MessageHandler<DeviceChangeMessage> onDeviceChange;
    public MessageHandler<DeviceChangeHeaderMessage> onDeviceChangeHeader;
    public MessageHandler<DeviceChildrenMessage> onDeviceChildren;
    public MessageHandler<DeviceListMessage> onDeviceList;
    public MessageHandler<DeviceListWindowMessage> onDeviceListWindow;
    public MessageHandler<DevicePageChangeMessage> onDevicePageChange;
    public MessageHandler<DevicePageNamesMessage> onDevicePageNames;
    public MessageHandler<DevicePageNamesWindowMessage> onDevicePageNamesWindow;
    public MessageHandler<DevicePageSelectByIndexMessage> onDevicePageSelectByIndex;
    public MessageHandler<DeviceRemoteControlsModulatedValuesBatchMessage> onDeviceRemoteControlsModulatedValuesBatch;
    public MessageHandler<DeviceRemoteControlDiscreteValuesMessage> onDeviceRemoteControlDiscreteValues;
    public MessageHandler<DeviceRemoteControlHasAutomationChangeMessage> onDeviceRemoteControlHasAutomationChange;
    public MessageHandler<DeviceRemoteControlIsModulatedChangeMessage> onDeviceRemoteControlIsModulatedChange;
    public MessageHandler<DeviceRemoteControlNameChangeMessage> onDeviceRemoteControlNameChange;
    public MessageHandler<DeviceRemoteControlOriginChangeMessage> onDeviceRemoteControlOriginChange;
    public MessageHandler<DeviceRemoteControlTouchMessage> onDeviceRemoteControlTouch;
    public MessageHandler<DeviceRemoteControlUpdateMessage> onDeviceRemoteControlUpdate;
    public MessageHandler<DeviceRemoteControlValueChangeMessage> onDeviceRemoteControlValueChange;
    public MessageHandler<DeviceSelectByIndexMessage> onDeviceSelectByIndex;
    public MessageHandler<DeviceStateChangeMessage> onDeviceStateChange;
    public MessageHandler<EnterDeviceChildMessage> onEnterDeviceChild;
    public MessageHandler<ExitToParentMessage> onExitToParent;
    public MessageHandler<RequestDeviceChildrenMessage> onRequestDeviceChildren;
    public MessageHandler<RequestDeviceListMessage> onRequestDeviceList;
    public MessageHandler<RequestDeviceListWindowMessage> onRequestDeviceListWindow;
    public MessageHandler<RequestDevicePageNamesMessage> onRequestDevicePageNames;
    public MessageHandler<RequestDevicePageNamesWindowMessage> onRequestDevicePageNamesWindow;
    public MessageHandler<ViewStateChangeMessage> onViewStateChange;
    public MessageHandler<LastClickedTouchMessage> onLastClickedTouch;
    public MessageHandler<LastClickedUpdateMessage> onLastClickedUpdate;
    public MessageHandler<LastClickedValueChangeMessage> onLastClickedValueChange;
    public MessageHandler<HostDeactivatedMessage> onHostDeactivated;
    public MessageHandler<HostInitializedMessage> onHostInitialized;
    public MessageHandler<RequestHostStatusMessage> onRequestHostStatus;
    public MessageHandler<EnterTrackGroupMessage> onEnterTrackGroup;
    public MessageHandler<ExitTrackGroupMessage> onExitTrackGroup;
    public MessageHandler<RequestSendDestinationsMessage> onRequestSendDestinations;
    public MessageHandler<RequestTrackListMessage> onRequestTrackList;
    public MessageHandler<RequestTrackListWindowMessage> onRequestTrackListWindow;
    public MessageHandler<RequestTrackSendListMessage> onRequestTrackSendList;
    public MessageHandler<SelectMixSendMessage> onSelectMixSend;
    public MessageHandler<SendDestinationsListMessage> onSendDestinationsList;
    public MessageHandler<TrackActivateMessage> onTrackActivate;
    public MessageHandler<TrackArmChangeMessage> onTrackArmChange;
    public MessageHandler<TrackChangeMessage> onTrackChange;
    public MessageHandler<TrackListMessage> onTrackList;
    public MessageHandler<TrackListWindowMessage> onTrackListWindow;
    public MessageHandler<TrackMuteMessage> onTrackMute;
    public MessageHandler<TrackMutedBySoloChangeMessage> onTrackMutedBySoloChange;
    public MessageHandler<TrackPanChangeMessage> onTrackPanChange;
    public MessageHandler<TrackPanHasAutomationChangeMessage> onTrackPanHasAutomationChange;
    public MessageHandler<TrackPanModulatedValueChangeMessage> onTrackPanModulatedValueChange;
    public MessageHandler<TrackPanTouchMessage> onTrackPanTouch;
    public MessageHandler<TrackSelectByIndexMessage> onTrackSelectByIndex;
    public MessageHandler<TrackSendEnabledChangeMessage> onTrackSendEnabledChange;
    public MessageHandler<TrackSendHasAutomationChangeMessage> onTrackSendHasAutomationChange;
    public MessageHandler<TrackSendListMessage> onTrackSendList;
    public MessageHandler<TrackSendModeChangeMessage> onTrackSendModeChange;
    public MessageHandler<TrackSendModulatedValueChangeMessage> onTrackSendModulatedValueChange;
    public MessageHandler<TrackSendPreFaderChangeMessage> onTrackSendPreFaderChange;
    public MessageHandler<TrackSendTouchMessage> onTrackSendTouch;
    public MessageHandler<TrackSendValueChangeMessage> onTrackSendValueChange;
    public MessageHandler<TrackSoloMessage> onTrackSolo;
    public MessageHandler<TrackVolumeChangeMessage> onTrackVolumeChange;
    public MessageHandler<TrackVolumeHasAutomationChangeMessage> onTrackVolumeHasAutomationChange;
    public MessageHandler<TrackVolumeModulatedValueChangeMessage> onTrackVolumeModulatedValueChange;
    public MessageHandler<TrackVolumeTouchMessage> onTrackVolumeTouch;
    public MessageHandler<ResetAutomationOverridesMessage> onResetAutomationOverrides;
    public MessageHandler<TransportArrangerAutomationWriteEnabledChangeMessage> onTransportArrangerAutomationWriteEnabledChange;
    public MessageHandler<TransportArrangerOverdubEnabledChangeMessage> onTransportArrangerOverdubEnabledChange;
    public MessageHandler<TransportAutomationOverrideActiveChangeMessage> onTransportAutomationOverrideActiveChange;
    public MessageHandler<TransportAutomationWriteModeChangeMessage> onTransportAutomationWriteModeChange;
    public MessageHandler<TransportClipLauncherAutomationWriteEnabledChangeMessage> onTransportClipLauncherAutomationWriteEnabledChange;
    public MessageHandler<TransportClipLauncherOverdubEnabledChangeMessage> onTransportClipLauncherOverdubEnabledChange;
    public MessageHandler<TransportPlayMessage> onTransportPlay;
    public MessageHandler<TransportRecordMessage> onTransportRecord;
    public MessageHandler<TransportStopMessage> onTransportStop;
    public MessageHandler<TransportTempoMessage> onTransportTempo;

    protected ProtocolCallbacks() {}
}
