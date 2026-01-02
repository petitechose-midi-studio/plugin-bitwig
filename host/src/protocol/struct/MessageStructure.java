package protocol.struct;

/**
 * MessageStructure - Central reference for all protocol messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * This class provides a central point listing all available message types.
 * Use individual message classes directly or reference this class for discovery.
 *
 * Usage:
 *   // Direct usage (recommended):
 *   TransportPlayMessage msg = new TransportPlayMessage(true);
 *   msg.encode(buffer, 0);
 *
 *   // Or use wildcard import:
 *   import protocol.struct.*;
 */
public final class MessageStructure {

    private MessageStructure() {
        // Prevent instantiation
    }

    // ============================================================================
    // Message Class References
    // ============================================================================

    /** @see DeviceChangeMessage */
    public static final Class<DeviceChangeMessage> DEVICE_CHANGE = DeviceChangeMessage.class;
    /** @see DeviceChangeHeaderMessage */
    public static final Class<DeviceChangeHeaderMessage> DEVICE_CHANGE_HEADER = DeviceChangeHeaderMessage.class;
    /** @see DeviceChildrenMessage */
    public static final Class<DeviceChildrenMessage> DEVICE_CHILDREN = DeviceChildrenMessage.class;
    /** @see DeviceEnabledStateMessage */
    public static final Class<DeviceEnabledStateMessage> DEVICE_ENABLED_STATE = DeviceEnabledStateMessage.class;
    /** @see DeviceListWindowMessage */
    public static final Class<DeviceListWindowMessage> DEVICE_LIST_WINDOW = DeviceListWindowMessage.class;
    /** @see DevicePageChangeMessage */
    public static final Class<DevicePageChangeMessage> DEVICE_PAGE_CHANGE = DevicePageChangeMessage.class;
    /** @see DevicePageNamesWindowMessage */
    public static final Class<DevicePageNamesWindowMessage> DEVICE_PAGE_NAMES_WINDOW = DevicePageNamesWindowMessage.class;
    /** @see DevicePageSelectMessage */
    public static final Class<DevicePageSelectMessage> DEVICE_PAGE_SELECT = DevicePageSelectMessage.class;
    /** @see DeviceRemoteControlsBatchMessage */
    public static final Class<DeviceRemoteControlsBatchMessage> DEVICE_REMOTE_CONTROLS_BATCH = DeviceRemoteControlsBatchMessage.class;
    /** @see DeviceRemoteControlDiscreteValuesMessage */
    public static final Class<DeviceRemoteControlDiscreteValuesMessage> DEVICE_REMOTE_CONTROL_DISCRETE_VALUES = DeviceRemoteControlDiscreteValuesMessage.class;
    /** @see DeviceRemoteControlHasAutomationChangeMessage */
    public static final Class<DeviceRemoteControlHasAutomationChangeMessage> DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE = DeviceRemoteControlHasAutomationChangeMessage.class;
    /** @see DeviceRemoteControlIsModulatedChangeMessage */
    public static final Class<DeviceRemoteControlIsModulatedChangeMessage> DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE = DeviceRemoteControlIsModulatedChangeMessage.class;
    /** @see DeviceRemoteControlNameChangeMessage */
    public static final Class<DeviceRemoteControlNameChangeMessage> DEVICE_REMOTE_CONTROL_NAME_CHANGE = DeviceRemoteControlNameChangeMessage.class;
    /** @see DeviceRemoteControlOriginChangeMessage */
    public static final Class<DeviceRemoteControlOriginChangeMessage> DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE = DeviceRemoteControlOriginChangeMessage.class;
    /** @see DeviceRemoteControlRestoreAutomationMessage */
    public static final Class<DeviceRemoteControlRestoreAutomationMessage> DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION = DeviceRemoteControlRestoreAutomationMessage.class;
    /** @see DeviceRemoteControlTouchMessage */
    public static final Class<DeviceRemoteControlTouchMessage> DEVICE_REMOTE_CONTROL_TOUCH = DeviceRemoteControlTouchMessage.class;
    /** @see DeviceRemoteControlUpdateMessage */
    public static final Class<DeviceRemoteControlUpdateMessage> DEVICE_REMOTE_CONTROL_UPDATE = DeviceRemoteControlUpdateMessage.class;
    /** @see DeviceSelectMessage */
    public static final Class<DeviceSelectMessage> DEVICE_SELECT = DeviceSelectMessage.class;
    /** @see DeviceStateMessage */
    public static final Class<DeviceStateMessage> DEVICE_STATE = DeviceStateMessage.class;
    /** @see EnterDeviceChildMessage */
    public static final Class<EnterDeviceChildMessage> ENTER_DEVICE_CHILD = EnterDeviceChildMessage.class;
    /** @see ExitToParentMessage */
    public static final Class<ExitToParentMessage> EXIT_TO_PARENT = ExitToParentMessage.class;
    /** @see RemoteControlValueMessage */
    public static final Class<RemoteControlValueMessage> REMOTE_CONTROL_VALUE = RemoteControlValueMessage.class;
    /** @see RemoteControlValueStateMessage */
    public static final Class<RemoteControlValueStateMessage> REMOTE_CONTROL_VALUE_STATE = RemoteControlValueStateMessage.class;
    /** @see RequestDeviceChildrenMessage */
    public static final Class<RequestDeviceChildrenMessage> REQUEST_DEVICE_CHILDREN = RequestDeviceChildrenMessage.class;
    /** @see RequestDeviceListWindowMessage */
    public static final Class<RequestDeviceListWindowMessage> REQUEST_DEVICE_LIST_WINDOW = RequestDeviceListWindowMessage.class;
    /** @see RequestDevicePageNamesWindowMessage */
    public static final Class<RequestDevicePageNamesWindowMessage> REQUEST_DEVICE_PAGE_NAMES_WINDOW = RequestDevicePageNamesWindowMessage.class;
    /** @see ViewStateMessage */
    public static final Class<ViewStateMessage> VIEW_STATE = ViewStateMessage.class;
    /** @see LastClickedTouchMessage */
    public static final Class<LastClickedTouchMessage> LAST_CLICKED_TOUCH = LastClickedTouchMessage.class;
    /** @see LastClickedUpdateMessage */
    public static final Class<LastClickedUpdateMessage> LAST_CLICKED_UPDATE = LastClickedUpdateMessage.class;
    /** @see LastClickedValueMessage */
    public static final Class<LastClickedValueMessage> LAST_CLICKED_VALUE = LastClickedValueMessage.class;
    /** @see LastClickedValueStateMessage */
    public static final Class<LastClickedValueStateMessage> LAST_CLICKED_VALUE_STATE = LastClickedValueStateMessage.class;
    /** @see HostDeactivatedMessage */
    public static final Class<HostDeactivatedMessage> HOST_DEACTIVATED = HostDeactivatedMessage.class;
    /** @see HostInitializedMessage */
    public static final Class<HostInitializedMessage> HOST_INITIALIZED = HostInitializedMessage.class;
    /** @see RequestHostStatusMessage */
    public static final Class<RequestHostStatusMessage> REQUEST_HOST_STATUS = RequestHostStatusMessage.class;
    /** @see EnterTrackGroupMessage */
    public static final Class<EnterTrackGroupMessage> ENTER_TRACK_GROUP = EnterTrackGroupMessage.class;
    /** @see ExitTrackGroupMessage */
    public static final Class<ExitTrackGroupMessage> EXIT_TRACK_GROUP = ExitTrackGroupMessage.class;
    /** @see RequestSendDestinationsMessage */
    public static final Class<RequestSendDestinationsMessage> REQUEST_SEND_DESTINATIONS = RequestSendDestinationsMessage.class;
    /** @see RequestTrackListWindowMessage */
    public static final Class<RequestTrackListWindowMessage> REQUEST_TRACK_LIST_WINDOW = RequestTrackListWindowMessage.class;
    /** @see RequestTrackSendListMessage */
    public static final Class<RequestTrackSendListMessage> REQUEST_TRACK_SEND_LIST = RequestTrackSendListMessage.class;
    /** @see SelectMixSendMessage */
    public static final Class<SelectMixSendMessage> SELECT_MIX_SEND = SelectMixSendMessage.class;
    /** @see SendDestinationsListMessage */
    public static final Class<SendDestinationsListMessage> SEND_DESTINATIONS_LIST = SendDestinationsListMessage.class;
    /** @see TrackActivateMessage */
    public static final Class<TrackActivateMessage> TRACK_ACTIVATE = TrackActivateMessage.class;
    /** @see TrackArmMessage */
    public static final Class<TrackArmMessage> TRACK_ARM = TrackArmMessage.class;
    /** @see TrackArmStateMessage */
    public static final Class<TrackArmStateMessage> TRACK_ARM_STATE = TrackArmStateMessage.class;
    /** @see TrackChangeMessage */
    public static final Class<TrackChangeMessage> TRACK_CHANGE = TrackChangeMessage.class;
    /** @see TrackListWindowMessage */
    public static final Class<TrackListWindowMessage> TRACK_LIST_WINDOW = TrackListWindowMessage.class;
    /** @see TrackMuteMessage */
    public static final Class<TrackMuteMessage> TRACK_MUTE = TrackMuteMessage.class;
    /** @see TrackMutedBySoloStateMessage */
    public static final Class<TrackMutedBySoloStateMessage> TRACK_MUTED_BY_SOLO_STATE = TrackMutedBySoloStateMessage.class;
    /** @see TrackMuteStateMessage */
    public static final Class<TrackMuteStateMessage> TRACK_MUTE_STATE = TrackMuteStateMessage.class;
    /** @see TrackPanMessage */
    public static final Class<TrackPanMessage> TRACK_PAN = TrackPanMessage.class;
    /** @see TrackPanHasAutomationStateMessage */
    public static final Class<TrackPanHasAutomationStateMessage> TRACK_PAN_HAS_AUTOMATION_STATE = TrackPanHasAutomationStateMessage.class;
    /** @see TrackPanModulatedValueStateMessage */
    public static final Class<TrackPanModulatedValueStateMessage> TRACK_PAN_MODULATED_VALUE_STATE = TrackPanModulatedValueStateMessage.class;
    /** @see TrackPanStateMessage */
    public static final Class<TrackPanStateMessage> TRACK_PAN_STATE = TrackPanStateMessage.class;
    /** @see TrackPanTouchMessage */
    public static final Class<TrackPanTouchMessage> TRACK_PAN_TOUCH = TrackPanTouchMessage.class;
    /** @see TrackSelectMessage */
    public static final Class<TrackSelectMessage> TRACK_SELECT = TrackSelectMessage.class;
    /** @see TrackSendEnabledMessage */
    public static final Class<TrackSendEnabledMessage> TRACK_SEND_ENABLED = TrackSendEnabledMessage.class;
    /** @see TrackSendEnabledStateMessage */
    public static final Class<TrackSendEnabledStateMessage> TRACK_SEND_ENABLED_STATE = TrackSendEnabledStateMessage.class;
    /** @see TrackSendHasAutomationStateMessage */
    public static final Class<TrackSendHasAutomationStateMessage> TRACK_SEND_HAS_AUTOMATION_STATE = TrackSendHasAutomationStateMessage.class;
    /** @see TrackSendListMessage */
    public static final Class<TrackSendListMessage> TRACK_SEND_LIST = TrackSendListMessage.class;
    /** @see TrackSendModeMessage */
    public static final Class<TrackSendModeMessage> TRACK_SEND_MODE = TrackSendModeMessage.class;
    /** @see TrackSendModeStateMessage */
    public static final Class<TrackSendModeStateMessage> TRACK_SEND_MODE_STATE = TrackSendModeStateMessage.class;
    /** @see TrackSendModulatedValueStateMessage */
    public static final Class<TrackSendModulatedValueStateMessage> TRACK_SEND_MODULATED_VALUE_STATE = TrackSendModulatedValueStateMessage.class;
    /** @see TrackSendPreFaderStateMessage */
    public static final Class<TrackSendPreFaderStateMessage> TRACK_SEND_PRE_FADER_STATE = TrackSendPreFaderStateMessage.class;
    /** @see TrackSendTouchMessage */
    public static final Class<TrackSendTouchMessage> TRACK_SEND_TOUCH = TrackSendTouchMessage.class;
    /** @see TrackSendValueMessage */
    public static final Class<TrackSendValueMessage> TRACK_SEND_VALUE = TrackSendValueMessage.class;
    /** @see TrackSendValueStateMessage */
    public static final Class<TrackSendValueStateMessage> TRACK_SEND_VALUE_STATE = TrackSendValueStateMessage.class;
    /** @see TrackSoloMessage */
    public static final Class<TrackSoloMessage> TRACK_SOLO = TrackSoloMessage.class;
    /** @see TrackSoloStateMessage */
    public static final Class<TrackSoloStateMessage> TRACK_SOLO_STATE = TrackSoloStateMessage.class;
    /** @see TrackVolumeMessage */
    public static final Class<TrackVolumeMessage> TRACK_VOLUME = TrackVolumeMessage.class;
    /** @see TrackVolumeHasAutomationStateMessage */
    public static final Class<TrackVolumeHasAutomationStateMessage> TRACK_VOLUME_HAS_AUTOMATION_STATE = TrackVolumeHasAutomationStateMessage.class;
    /** @see TrackVolumeModulatedValueStateMessage */
    public static final Class<TrackVolumeModulatedValueStateMessage> TRACK_VOLUME_MODULATED_VALUE_STATE = TrackVolumeModulatedValueStateMessage.class;
    /** @see TrackVolumeStateMessage */
    public static final Class<TrackVolumeStateMessage> TRACK_VOLUME_STATE = TrackVolumeStateMessage.class;
    /** @see TrackVolumeTouchMessage */
    public static final Class<TrackVolumeTouchMessage> TRACK_VOLUME_TOUCH = TrackVolumeTouchMessage.class;
    /** @see ResetAutomationOverridesMessage */
    public static final Class<ResetAutomationOverridesMessage> RESET_AUTOMATION_OVERRIDES = ResetAutomationOverridesMessage.class;
    /** @see TransportArrangerAutomationWriteEnabledMessage */
    public static final Class<TransportArrangerAutomationWriteEnabledMessage> TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED = TransportArrangerAutomationWriteEnabledMessage.class;
    /** @see TransportArrangerAutomationWriteEnabledStateMessage */
    public static final Class<TransportArrangerAutomationWriteEnabledStateMessage> TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE = TransportArrangerAutomationWriteEnabledStateMessage.class;
    /** @see TransportArrangerOverdubEnabledMessage */
    public static final Class<TransportArrangerOverdubEnabledMessage> TRANSPORT_ARRANGER_OVERDUB_ENABLED = TransportArrangerOverdubEnabledMessage.class;
    /** @see TransportArrangerOverdubEnabledStateMessage */
    public static final Class<TransportArrangerOverdubEnabledStateMessage> TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE = TransportArrangerOverdubEnabledStateMessage.class;
    /** @see TransportAutomationOverrideActiveStateMessage */
    public static final Class<TransportAutomationOverrideActiveStateMessage> TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE = TransportAutomationOverrideActiveStateMessage.class;
    /** @see TransportAutomationWriteModeMessage */
    public static final Class<TransportAutomationWriteModeMessage> TRANSPORT_AUTOMATION_WRITE_MODE = TransportAutomationWriteModeMessage.class;
    /** @see TransportAutomationWriteModeStateMessage */
    public static final Class<TransportAutomationWriteModeStateMessage> TRANSPORT_AUTOMATION_WRITE_MODE_STATE = TransportAutomationWriteModeStateMessage.class;
    /** @see TransportClipLauncherAutomationWriteEnabledMessage */
    public static final Class<TransportClipLauncherAutomationWriteEnabledMessage> TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED = TransportClipLauncherAutomationWriteEnabledMessage.class;
    /** @see TransportClipLauncherAutomationWriteEnabledStateMessage */
    public static final Class<TransportClipLauncherAutomationWriteEnabledStateMessage> TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_STATE = TransportClipLauncherAutomationWriteEnabledStateMessage.class;
    /** @see TransportClipLauncherOverdubEnabledMessage */
    public static final Class<TransportClipLauncherOverdubEnabledMessage> TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED = TransportClipLauncherOverdubEnabledMessage.class;
    /** @see TransportClipLauncherOverdubEnabledStateMessage */
    public static final Class<TransportClipLauncherOverdubEnabledStateMessage> TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE = TransportClipLauncherOverdubEnabledStateMessage.class;
    /** @see TransportPlayMessage */
    public static final Class<TransportPlayMessage> TRANSPORT_PLAY = TransportPlayMessage.class;
    /** @see TransportPlayingStateMessage */
    public static final Class<TransportPlayingStateMessage> TRANSPORT_PLAYING_STATE = TransportPlayingStateMessage.class;
    /** @see TransportRecordMessage */
    public static final Class<TransportRecordMessage> TRANSPORT_RECORD = TransportRecordMessage.class;
    /** @see TransportRecordingStateMessage */
    public static final Class<TransportRecordingStateMessage> TRANSPORT_RECORDING_STATE = TransportRecordingStateMessage.class;
    /** @see TransportStopMessage */
    public static final Class<TransportStopMessage> TRANSPORT_STOP = TransportStopMessage.class;
    /** @see TransportTempoMessage */
    public static final Class<TransportTempoMessage> TRANSPORT_TEMPO = TransportTempoMessage.class;
    /** @see TransportTempoStateMessage */
    public static final Class<TransportTempoStateMessage> TRANSPORT_TEMPO_STATE = TransportTempoStateMessage.class;

}
