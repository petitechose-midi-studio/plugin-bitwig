package protocol;

import protocol.struct.*;

/**
 * ProtocolCallbacks - Typed callbacks for all messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Base class providing typed callbacks for each message type.
 * Extends ProtocolMethods for explicit send API.
 * Protocol extends this and DecoderRegistry calls these callbacks.
 *
 * Inheritance: ProtocolMethods -> ProtocolCallbacks -> Protocol
 *
 * Usage:
 *   protocol.onTransportPlay = msg -> {
 *       System.out.println("Playing: " + msg.isPlaying());
 *   };
 */
public abstract class ProtocolCallbacks extends ProtocolMethods {

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
    public MessageHandler<DeviceEnabledStateMessage> onDeviceEnabledState;
    public MessageHandler<DeviceListWindowMessage> onDeviceListWindow;
    public MessageHandler<DevicePageChangeMessage> onDevicePageChange;
    public MessageHandler<DevicePageNamesWindowMessage> onDevicePageNamesWindow;
    public MessageHandler<DevicePageSelectMessage> onDevicePageSelect;
    public MessageHandler<DeviceRemoteControlsBatchMessage> onDeviceRemoteControlsBatch;
    public MessageHandler<DeviceRemoteControlDiscreteValuesMessage> onDeviceRemoteControlDiscreteValues;
    public MessageHandler<DeviceRemoteControlHasAutomationChangeMessage> onDeviceRemoteControlHasAutomationChange;
    public MessageHandler<DeviceRemoteControlIsModulatedChangeMessage> onDeviceRemoteControlIsModulatedChange;
    public MessageHandler<DeviceRemoteControlNameChangeMessage> onDeviceRemoteControlNameChange;
    public MessageHandler<DeviceRemoteControlOriginChangeMessage> onDeviceRemoteControlOriginChange;
    public MessageHandler<DeviceRemoteControlRestoreAutomationMessage> onDeviceRemoteControlRestoreAutomation;
    public MessageHandler<DeviceRemoteControlTouchMessage> onDeviceRemoteControlTouch;
    public MessageHandler<DeviceRemoteControlUpdateMessage> onDeviceRemoteControlUpdate;
    public MessageHandler<DeviceSelectMessage> onDeviceSelect;
    public MessageHandler<DeviceStateMessage> onDeviceState;
    public MessageHandler<EnterDeviceChildMessage> onEnterDeviceChild;
    public MessageHandler<ExitToParentMessage> onExitToParent;
    public MessageHandler<RemoteControlValueMessage> onRemoteControlValue;
    public MessageHandler<RemoteControlValueStateMessage> onRemoteControlValueState;
    public MessageHandler<RequestDeviceChildrenMessage> onRequestDeviceChildren;
    public MessageHandler<RequestDeviceListWindowMessage> onRequestDeviceListWindow;
    public MessageHandler<RequestDevicePageNamesWindowMessage> onRequestDevicePageNamesWindow;
    public MessageHandler<ViewStateMessage> onViewState;
    public MessageHandler<LastClickedTouchMessage> onLastClickedTouch;
    public MessageHandler<LastClickedUpdateMessage> onLastClickedUpdate;
    public MessageHandler<LastClickedValueMessage> onLastClickedValue;
    public MessageHandler<LastClickedValueStateMessage> onLastClickedValueState;
    public MessageHandler<HostDeactivatedMessage> onHostDeactivated;
    public MessageHandler<HostInitializedMessage> onHostInitialized;
    public MessageHandler<RequestHostStatusMessage> onRequestHostStatus;
    public MessageHandler<EnterTrackGroupMessage> onEnterTrackGroup;
    public MessageHandler<ExitTrackGroupMessage> onExitTrackGroup;
    public MessageHandler<RequestSendDestinationsMessage> onRequestSendDestinations;
    public MessageHandler<RequestTrackListWindowMessage> onRequestTrackListWindow;
    public MessageHandler<RequestTrackSendListMessage> onRequestTrackSendList;
    public MessageHandler<SelectMixSendMessage> onSelectMixSend;
    public MessageHandler<SendDestinationsListMessage> onSendDestinationsList;
    public MessageHandler<TrackActivateMessage> onTrackActivate;
    public MessageHandler<TrackArmMessage> onTrackArm;
    public MessageHandler<TrackArmStateMessage> onTrackArmState;
    public MessageHandler<TrackChangeMessage> onTrackChange;
    public MessageHandler<TrackListWindowMessage> onTrackListWindow;
    public MessageHandler<TrackMuteMessage> onTrackMute;
    public MessageHandler<TrackMutedBySoloStateMessage> onTrackMutedBySoloState;
    public MessageHandler<TrackMuteStateMessage> onTrackMuteState;
    public MessageHandler<TrackPanMessage> onTrackPan;
    public MessageHandler<TrackPanHasAutomationStateMessage> onTrackPanHasAutomationState;
    public MessageHandler<TrackPanModulatedValueStateMessage> onTrackPanModulatedValueState;
    public MessageHandler<TrackPanStateMessage> onTrackPanState;
    public MessageHandler<TrackPanTouchMessage> onTrackPanTouch;
    public MessageHandler<TrackSelectMessage> onTrackSelect;
    public MessageHandler<TrackSendEnabledMessage> onTrackSendEnabled;
    public MessageHandler<TrackSendEnabledStateMessage> onTrackSendEnabledState;
    public MessageHandler<TrackSendHasAutomationStateMessage> onTrackSendHasAutomationState;
    public MessageHandler<TrackSendListMessage> onTrackSendList;
    public MessageHandler<TrackSendModeMessage> onTrackSendMode;
    public MessageHandler<TrackSendModeStateMessage> onTrackSendModeState;
    public MessageHandler<TrackSendModulatedValueStateMessage> onTrackSendModulatedValueState;
    public MessageHandler<TrackSendPreFaderStateMessage> onTrackSendPreFaderState;
    public MessageHandler<TrackSendTouchMessage> onTrackSendTouch;
    public MessageHandler<TrackSendValueMessage> onTrackSendValue;
    public MessageHandler<TrackSendValueStateMessage> onTrackSendValueState;
    public MessageHandler<TrackSoloMessage> onTrackSolo;
    public MessageHandler<TrackSoloStateMessage> onTrackSoloState;
    public MessageHandler<TrackVolumeMessage> onTrackVolume;
    public MessageHandler<TrackVolumeHasAutomationStateMessage> onTrackVolumeHasAutomationState;
    public MessageHandler<TrackVolumeModulatedValueStateMessage> onTrackVolumeModulatedValueState;
    public MessageHandler<TrackVolumeStateMessage> onTrackVolumeState;
    public MessageHandler<TrackVolumeTouchMessage> onTrackVolumeTouch;
    public MessageHandler<ResetAutomationOverridesMessage> onResetAutomationOverrides;
    public MessageHandler<TransportArrangerAutomationWriteEnabledMessage> onTransportArrangerAutomationWriteEnabled;
    public MessageHandler<TransportArrangerAutomationWriteEnabledStateMessage> onTransportArrangerAutomationWriteEnabledState;
    public MessageHandler<TransportArrangerOverdubEnabledMessage> onTransportArrangerOverdubEnabled;
    public MessageHandler<TransportArrangerOverdubEnabledStateMessage> onTransportArrangerOverdubEnabledState;
    public MessageHandler<TransportAutomationOverrideActiveStateMessage> onTransportAutomationOverrideActiveState;
    public MessageHandler<TransportAutomationWriteModeMessage> onTransportAutomationWriteMode;
    public MessageHandler<TransportAutomationWriteModeStateMessage> onTransportAutomationWriteModeState;
    public MessageHandler<TransportClipLauncherAutomationWriteEnabledMessage> onTransportClipLauncherAutomationWriteEnabled;
    public MessageHandler<TransportClipLauncherAutomationWriteEnabledStateMessage> onTransportClipLauncherAutomationWriteEnabledState;
    public MessageHandler<TransportClipLauncherOverdubEnabledMessage> onTransportClipLauncherOverdubEnabled;
    public MessageHandler<TransportClipLauncherOverdubEnabledStateMessage> onTransportClipLauncherOverdubEnabledState;
    public MessageHandler<TransportPlayMessage> onTransportPlay;
    public MessageHandler<TransportPlayingStateMessage> onTransportPlayingState;
    public MessageHandler<TransportRecordMessage> onTransportRecord;
    public MessageHandler<TransportRecordingStateMessage> onTransportRecordingState;
    public MessageHandler<TransportStopMessage> onTransportStop;
    public MessageHandler<TransportTempoMessage> onTransportTempo;
    public MessageHandler<TransportTempoStateMessage> onTransportTempoState;

    protected ProtocolCallbacks() {}
}
