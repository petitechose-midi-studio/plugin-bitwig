/**
 * ProtocolMethods.java - Explicit Protocol API
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * This class provides explicit API methods instead of generic send().
 * Extend this class or include its methods in your Protocol class.
 *
 * Pattern:
 *   TO_HOST messages (Controller -> Host): Consumer<T> onMethodName
 *   TO_CONTROLLER messages (Host -> Controller): void methodName(params)
 */

package protocol;

import java.util.function.Consumer;
import protocol.struct.*;
import protocol.DeviceType;
import protocol.ParameterType;
import protocol.TrackType;
import protocol.ViewType;

/**
 * Explicit protocol methods base class
 */
public abstract class ProtocolMethods {

    // =========================================================================
    // COMMANDS (Controller -> Host) - Callbacks
    // =========================================================================

    public Consumer<DevicePageSelectMessage> onDevicePageSelect = null;
    public Consumer<DeviceRemoteControlRestoreAutomationMessage> onDeviceRemoteControlRestoreAutomation = null;
    public Consumer<DeviceRemoteControlTouchMessage> onDeviceRemoteControlTouch = null;
    public Consumer<DeviceSelectMessage> onDeviceSelect = null;
    public Consumer<DeviceStateMessage> onDeviceState = null;
    public Consumer<EnterDeviceChildMessage> onEnterDeviceChild = null;
    public Consumer<ExitToParentMessage> onExitToParent = null;
    public Consumer<RemoteControlValueMessage> onRemoteControlValue = null;
    public Consumer<RequestDeviceChildrenMessage> onRequestDeviceChildren = null;
    public Consumer<RequestDeviceListWindowMessage> onRequestDeviceListWindow = null;
    public Consumer<RequestDevicePageNamesWindowMessage> onRequestDevicePageNamesWindow = null;
    public Consumer<ViewStateMessage> onViewState = null;
    public Consumer<LastClickedTouchMessage> onLastClickedTouch = null;
    public Consumer<LastClickedValueMessage> onLastClickedValue = null;
    public Consumer<RequestHostStatusMessage> onRequestHostStatus = null;
    public Consumer<EnterTrackGroupMessage> onEnterTrackGroup = null;
    public Consumer<ExitTrackGroupMessage> onExitTrackGroup = null;
    public Consumer<RequestSendDestinationsMessage> onRequestSendDestinations = null;
    public Consumer<RequestTrackListWindowMessage> onRequestTrackListWindow = null;
    public Consumer<RequestTrackSendListMessage> onRequestTrackSendList = null;
    public Consumer<SelectMixSendMessage> onSelectMixSend = null;
    public Consumer<TrackActivateMessage> onTrackActivate = null;
    public Consumer<TrackArmMessage> onTrackArm = null;
    public Consumer<TrackMuteMessage> onTrackMute = null;
    public Consumer<TrackPanMessage> onTrackPan = null;
    public Consumer<TrackPanTouchMessage> onTrackPanTouch = null;
    public Consumer<TrackSelectMessage> onTrackSelect = null;
    public Consumer<TrackSendEnabledMessage> onTrackSendEnabled = null;
    public Consumer<TrackSendModeMessage> onTrackSendMode = null;
    public Consumer<TrackSendTouchMessage> onTrackSendTouch = null;
    public Consumer<TrackSendValueMessage> onTrackSendValue = null;
    public Consumer<TrackSoloMessage> onTrackSolo = null;
    public Consumer<TrackVolumeMessage> onTrackVolume = null;
    public Consumer<TrackVolumeTouchMessage> onTrackVolumeTouch = null;
    public Consumer<ResetAutomationOverridesMessage> onResetAutomationOverrides = null;
    public Consumer<TransportArrangerAutomationWriteEnabledMessage> onTransportArrangerAutomationWriteEnabled = null;
    public Consumer<TransportArrangerOverdubEnabledMessage> onTransportArrangerOverdubEnabled = null;
    public Consumer<TransportAutomationWriteModeMessage> onTransportAutomationWriteMode = null;
    public Consumer<TransportClipLauncherAutomationWriteEnabledMessage> onTransportClipLauncherAutomationWriteEnabled = null;
    public Consumer<TransportClipLauncherOverdubEnabledMessage> onTransportClipLauncherOverdubEnabled = null;
    public Consumer<TransportPlayMessage> onTransportPlay = null;
    public Consumer<TransportRecordMessage> onTransportRecord = null;
    public Consumer<TransportStopMessage> onTransportStop = null;
    public Consumer<TransportTempoMessage> onTransportTempo = null;

    // =========================================================================
    // NOTIFICATIONS (Host -> Controller) - Send Methods
    // =========================================================================

    public void deviceChange(String deviceTrackName, String deviceName, boolean isEnabled, DeviceChangeMessage.PageInfo pageInfo, DeviceChangeMessage.RemoteControls[] remoteControls) {
        send(new DeviceChangeMessage(deviceTrackName, deviceName, isEnabled, pageInfo, remoteControls));
    }

    public void deviceChangeHeader(String deviceName, boolean isEnabled, DeviceType deviceType, DeviceChangeHeaderMessage.PageInfo pageInfo, int[] childrenTypes) {
        send(new DeviceChangeHeaderMessage(deviceName, isEnabled, deviceType, pageInfo, childrenTypes));
    }

    public void deviceChildren(int deviceIndex, int childType, int childrenCount, DeviceChildrenMessage.Children[] children) {
        send(new DeviceChildrenMessage(deviceIndex, childType, childrenCount, children));
    }

    public void deviceEnabledState(int deviceIndex, boolean isEnabled) {
        send(new DeviceEnabledStateMessage(deviceIndex, isEnabled));
    }

    public void deviceListWindow(int deviceCount, int deviceStartIndex, int deviceIndex, boolean isNested, String parentName, DeviceListWindowMessage.Devices[] devices) {
        send(new DeviceListWindowMessage(deviceCount, deviceStartIndex, deviceIndex, isNested, parentName, devices));
    }

    public void devicePageChange(DevicePageChangeMessage.PageInfo pageInfo, DevicePageChangeMessage.RemoteControls[] remoteControls) {
        send(new DevicePageChangeMessage(pageInfo, remoteControls));
    }

    public void devicePageNamesWindow(int devicePageCount, int pageStartIndex, int devicePageIndex, String[] pageNames) {
        send(new DevicePageNamesWindowMessage(devicePageCount, pageStartIndex, devicePageIndex, pageNames));
    }

    public void deviceRemoteControlsBatch(int sequenceNumber, int dirtyMask, int echoMask, int hasAutomationMask, float[] values, float[] modulatedValues, String[] displayValues) {
        send(new DeviceRemoteControlsBatchMessage(sequenceNumber, dirtyMask, echoMask, hasAutomationMask, values, modulatedValues, displayValues));
    }

    public void deviceRemoteControlDiscreteValues(int remoteControlIndex, String[] discreteValueNames, int currentValueIndex) {
        send(new DeviceRemoteControlDiscreteValuesMessage(remoteControlIndex, discreteValueNames, currentValueIndex));
    }

    public void deviceRemoteControlHasAutomationChange(int remoteControlIndex, boolean hasAutomation) {
        send(new DeviceRemoteControlHasAutomationChangeMessage(remoteControlIndex, hasAutomation));
    }

    public void deviceRemoteControlIsModulatedChange(int remoteControlIndex, boolean isModulated) {
        send(new DeviceRemoteControlIsModulatedChangeMessage(remoteControlIndex, isModulated));
    }

    public void deviceRemoteControlNameChange(int remoteControlIndex, String parameterName) {
        send(new DeviceRemoteControlNameChangeMessage(remoteControlIndex, parameterName));
    }

    public void deviceRemoteControlOriginChange(int remoteControlIndex, float parameterOrigin) {
        send(new DeviceRemoteControlOriginChangeMessage(remoteControlIndex, parameterOrigin));
    }

    public void deviceRemoteControlUpdate(int remoteControlIndex, String parameterName, float parameterValue, String displayValue, float parameterOrigin, boolean parameterExists, ParameterType parameterType, short discreteValueCount, int currentValueIndex, boolean hasAutomation, float modulatedValue) {
        send(new DeviceRemoteControlUpdateMessage(remoteControlIndex, parameterName, parameterValue, displayValue, parameterOrigin, parameterExists, parameterType, discreteValueCount, currentValueIndex, hasAutomation, modulatedValue));
    }

    public void remoteControlValueState(int remoteControlIndex, float parameterValue, String displayValue) {
        send(new RemoteControlValueStateMessage(remoteControlIndex, parameterValue, displayValue));
    }

    public void lastClickedUpdate(String parameterName, float parameterValue, String displayValue, float parameterOrigin, boolean parameterExists, ParameterType parameterType, short discreteValueCount, int currentValueIndex) {
        send(new LastClickedUpdateMessage(parameterName, parameterValue, displayValue, parameterOrigin, parameterExists, parameterType, discreteValueCount, currentValueIndex));
    }

    public void lastClickedValueState(float parameterValue, String displayValue) {
        send(new LastClickedValueStateMessage(parameterValue, displayValue));
    }

    public void hostDeactivated(boolean isHostActive) {
        send(new HostDeactivatedMessage(isHostActive));
    }

    public void hostInitialized(boolean isHostActive) {
        send(new HostInitializedMessage(isHostActive));
    }

    public void sendDestinationsList(int sendCount, SendDestinationsListMessage.SendDestinations[] sendDestinations) {
        send(new SendDestinationsListMessage(sendCount, sendDestinations));
    }

    public void trackArmState(int trackIndex, boolean isArm) {
        send(new TrackArmStateMessage(trackIndex, isArm));
    }

    public void trackChange(String trackName, long color, int trackIndex, TrackType trackType, boolean isActivated, boolean isMute, boolean isSolo, boolean isMutedBySolo, boolean isArm, float volume, String volumeDisplay, float pan, String panDisplay) {
        send(new TrackChangeMessage(trackName, color, trackIndex, trackType, isActivated, isMute, isSolo, isMutedBySolo, isArm, volume, volumeDisplay, pan, panDisplay));
    }

    public void trackListWindow(int trackCount, int trackStartIndex, int trackIndex, boolean isNested, String parentGroupName, TrackListWindowMessage.Tracks[] tracks) {
        send(new TrackListWindowMessage(trackCount, trackStartIndex, trackIndex, isNested, parentGroupName, tracks));
    }

    public void trackMutedBySoloState(int trackIndex, boolean isMutedBySolo) {
        send(new TrackMutedBySoloStateMessage(trackIndex, isMutedBySolo));
    }

    public void trackMuteState(int trackIndex, boolean isMute) {
        send(new TrackMuteStateMessage(trackIndex, isMute));
    }

    public void trackPanHasAutomationState(int trackIndex, boolean hasAutomation) {
        send(new TrackPanHasAutomationStateMessage(trackIndex, hasAutomation));
    }

    public void trackPanModulatedValueState(int trackIndex, float modulatedValue) {
        send(new TrackPanModulatedValueStateMessage(trackIndex, modulatedValue));
    }

    public void trackPanState(int trackIndex, float pan, String panDisplay) {
        send(new TrackPanStateMessage(trackIndex, pan, panDisplay));
    }

    public void trackSendEnabledState(int trackIndex, int sendIndex, boolean sendIsEnabled) {
        send(new TrackSendEnabledStateMessage(trackIndex, sendIndex, sendIsEnabled));
    }

    public void trackSendHasAutomationState(int trackIndex, int sendIndex, boolean hasAutomation) {
        send(new TrackSendHasAutomationStateMessage(trackIndex, sendIndex, hasAutomation));
    }

    public void trackSendList(int trackIndex, int sendCount, TrackSendListMessage.Sends[] sends) {
        send(new TrackSendListMessage(trackIndex, sendCount, sends));
    }

    public void trackSendModeState(int trackIndex, int sendIndex, String sendMode) {
        send(new TrackSendModeStateMessage(trackIndex, sendIndex, sendMode));
    }

    public void trackSendModulatedValueState(int trackIndex, int sendIndex, float modulatedValue) {
        send(new TrackSendModulatedValueStateMessage(trackIndex, sendIndex, modulatedValue));
    }

    public void trackSendPreFaderState(int trackIndex, int sendIndex, boolean sendIsPreFader) {
        send(new TrackSendPreFaderStateMessage(trackIndex, sendIndex, sendIsPreFader));
    }

    public void trackSendValueState(int trackIndex, int sendIndex, float sendValue, String sendDisplayValue) {
        send(new TrackSendValueStateMessage(trackIndex, sendIndex, sendValue, sendDisplayValue));
    }

    public void trackSoloState(int trackIndex, boolean isSolo) {
        send(new TrackSoloStateMessage(trackIndex, isSolo));
    }

    public void trackVolumeHasAutomationState(int trackIndex, boolean hasAutomation) {
        send(new TrackVolumeHasAutomationStateMessage(trackIndex, hasAutomation));
    }

    public void trackVolumeModulatedValueState(int trackIndex, float modulatedValue) {
        send(new TrackVolumeModulatedValueStateMessage(trackIndex, modulatedValue));
    }

    public void trackVolumeState(int trackIndex, float volume, String volumeDisplay) {
        send(new TrackVolumeStateMessage(trackIndex, volume, volumeDisplay));
    }

    public void transportArrangerAutomationWriteEnabledState(boolean isArrangerAutomationWriteEnabled) {
        send(new TransportArrangerAutomationWriteEnabledStateMessage(isArrangerAutomationWriteEnabled));
    }

    public void transportArrangerOverdubEnabledState(boolean isArrangerOverdubEnabled) {
        send(new TransportArrangerOverdubEnabledStateMessage(isArrangerOverdubEnabled));
    }

    public void transportAutomationOverrideActiveState(boolean isAutomationOverrideActive) {
        send(new TransportAutomationOverrideActiveStateMessage(isAutomationOverrideActive));
    }

    public void transportAutomationWriteModeState(int automationWriteMode) {
        send(new TransportAutomationWriteModeStateMessage(automationWriteMode));
    }

    public void transportClipLauncherAutomationWriteEnabledState(boolean isClipLauncherAutomationWriteEnabled) {
        send(new TransportClipLauncherAutomationWriteEnabledStateMessage(isClipLauncherAutomationWriteEnabled));
    }

    public void transportClipLauncherOverdubEnabledState(boolean isClipLauncherOverdubEnabled) {
        send(new TransportClipLauncherOverdubEnabledStateMessage(isClipLauncherOverdubEnabled));
    }

    public void transportPlayingState(boolean isPlaying) {
        send(new TransportPlayingStateMessage(isPlaying));
    }

    public void transportRecordingState(boolean isRecording) {
        send(new TransportRecordingStateMessage(isRecording));
    }

    public void transportTempoState(float tempo) {
        send(new TransportTempoStateMessage(tempo));
    }

    // =========================================================================
    // Abstract send method (implemented by subclass)
    // =========================================================================

    protected abstract void send(Object message);
}
