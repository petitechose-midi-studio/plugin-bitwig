/**
 * ProtocolMethods.ipp - Explicit Protocol API (inline include)
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * This file provides explicit send methods for TO_HOST messages.
 * Include this file inside your Protocol class definition.
 *
 * Usage in BitwigProtocol.hpp:
 *   class BitwigProtocol : public Protocol::ProtocolCallbacks {
 *   public:
 *       template<typename T> void send(const T& msg) { ... }
 *
 *       // Explicit API methods (generated)
 *       #include "ProtocolMethods.ipp"
 *   };
 *
 * Then use: protocol.transportPlay(true) instead of protocol.send(TransportPlayMessage{true})
 */

// =============================================================================
// COMMANDS (Controller -> Host) - Explicit send methods
// =============================================================================

    void devicePageSelect(uint8_t devicePageIndex) {
        send(Protocol::DevicePageSelectMessage{devicePageIndex});
    }

    void deviceRemoteControlRestoreAutomation(uint8_t remoteControlIndex) {
        send(Protocol::DeviceRemoteControlRestoreAutomationMessage{remoteControlIndex});
    }

    void deviceRemoteControlTouch(uint8_t remoteControlIndex, bool isTouched) {
        send(Protocol::DeviceRemoteControlTouchMessage{remoteControlIndex, isTouched});
    }

    void deviceSelect(uint8_t deviceIndex) {
        send(Protocol::DeviceSelectMessage{deviceIndex});
    }

    void deviceState(uint8_t deviceIndex, bool isEnabled) {
        send(Protocol::DeviceStateMessage{deviceIndex, isEnabled});
    }

    void enterDeviceChild(uint8_t deviceIndex, uint8_t childType, uint8_t childIndex) {
        send(Protocol::EnterDeviceChildMessage{deviceIndex, childType, childIndex});
    }

    void exitToParent() {
        send(Protocol::ExitToParentMessage{});
    }

    void remoteControlValue(uint8_t remoteControlIndex, float parameterValue) {
        send(Protocol::RemoteControlValueMessage{remoteControlIndex, parameterValue});
    }

    void requestDeviceChildren(uint8_t deviceIndex, uint8_t childType) {
        send(Protocol::RequestDeviceChildrenMessage{deviceIndex, childType});
    }

    void requestDeviceListWindow(uint8_t deviceStartIndex) {
        send(Protocol::RequestDeviceListWindowMessage{deviceStartIndex});
    }

    void requestDevicePageNamesWindow(uint8_t pageStartIndex) {
        send(Protocol::RequestDevicePageNamesWindowMessage{pageStartIndex});
    }

    void viewState(ViewType viewType, bool selectorActive) {
        send(Protocol::ViewStateMessage{viewType, selectorActive});
    }

    void lastClickedTouch(bool isTouched) {
        send(Protocol::LastClickedTouchMessage{isTouched});
    }

    void lastClickedValue(float parameterValue) {
        send(Protocol::LastClickedValueMessage{parameterValue});
    }

    void requestHostStatus() {
        send(Protocol::RequestHostStatusMessage{});
    }

    void enterTrackGroup(uint8_t trackIndex) {
        send(Protocol::EnterTrackGroupMessage{trackIndex});
    }

    void exitTrackGroup() {
        send(Protocol::ExitTrackGroupMessage{});
    }

    void requestSendDestinations() {
        send(Protocol::RequestSendDestinationsMessage{});
    }

    void requestTrackListWindow(uint8_t trackStartIndex) {
        send(Protocol::RequestTrackListWindowMessage{trackStartIndex});
    }

    void requestTrackSendList() {
        send(Protocol::RequestTrackSendListMessage{});
    }

    void selectMixSend(uint8_t sendIndex) {
        send(Protocol::SelectMixSendMessage{sendIndex});
    }

    void trackActivate(uint8_t trackIndex) {
        send(Protocol::TrackActivateMessage{trackIndex});
    }

    void trackArm(uint8_t trackIndex, bool isArm) {
        send(Protocol::TrackArmMessage{trackIndex, isArm});
    }

    void trackMute(uint8_t trackIndex, bool isMute) {
        send(Protocol::TrackMuteMessage{trackIndex, isMute});
    }

    void trackPan(uint8_t trackIndex, float pan) {
        send(Protocol::TrackPanMessage{trackIndex, pan});
    }

    void trackPanTouch(uint8_t trackIndex, bool isTouched) {
        send(Protocol::TrackPanTouchMessage{trackIndex, isTouched});
    }

    void trackSelect(uint8_t trackIndex) {
        send(Protocol::TrackSelectMessage{trackIndex});
    }

    void trackSendEnabled(uint8_t trackIndex, uint8_t sendIndex, bool sendIsEnabled) {
        send(Protocol::TrackSendEnabledMessage{trackIndex, sendIndex, sendIsEnabled});
    }

    void trackSendMode(uint8_t trackIndex, uint8_t sendIndex, const std::string& sendMode) {
        send(Protocol::TrackSendModeMessage{trackIndex, sendIndex, sendMode});
    }

    void trackSendTouch(uint8_t trackIndex, uint8_t sendIndex, bool isTouched) {
        send(Protocol::TrackSendTouchMessage{trackIndex, sendIndex, isTouched});
    }

    void trackSendValue(uint8_t trackIndex, uint8_t sendIndex, float sendValue) {
        send(Protocol::TrackSendValueMessage{trackIndex, sendIndex, sendValue});
    }

    void trackSolo(uint8_t trackIndex, bool isSolo) {
        send(Protocol::TrackSoloMessage{trackIndex, isSolo});
    }

    void trackVolume(uint8_t trackIndex, float volume) {
        send(Protocol::TrackVolumeMessage{trackIndex, volume});
    }

    void trackVolumeTouch(uint8_t trackIndex, bool isTouched) {
        send(Protocol::TrackVolumeTouchMessage{trackIndex, isTouched});
    }

    void resetAutomationOverrides() {
        send(Protocol::ResetAutomationOverridesMessage{});
    }

    void transportArrangerAutomationWriteEnabled(bool isArrangerAutomationWriteEnabled) {
        send(Protocol::TransportArrangerAutomationWriteEnabledMessage{isArrangerAutomationWriteEnabled});
    }

    void transportArrangerOverdubEnabled(bool isArrangerOverdubEnabled) {
        send(Protocol::TransportArrangerOverdubEnabledMessage{isArrangerOverdubEnabled});
    }

    void transportAutomationWriteMode(uint8_t automationWriteMode) {
        send(Protocol::TransportAutomationWriteModeMessage{automationWriteMode});
    }

    void transportClipLauncherAutomationWriteEnabled(bool isClipLauncherAutomationWriteEnabled) {
        send(Protocol::TransportClipLauncherAutomationWriteEnabledMessage{isClipLauncherAutomationWriteEnabled});
    }

    void transportClipLauncherOverdubEnabled(bool isClipLauncherOverdubEnabled) {
        send(Protocol::TransportClipLauncherOverdubEnabledMessage{isClipLauncherOverdubEnabled});
    }

    void transportPlay(bool isPlaying) {
        send(Protocol::TransportPlayMessage{isPlaying});
    }

    void transportRecord(bool isRecording) {
        send(Protocol::TransportRecordMessage{isRecording});
    }

    void transportStop() {
        send(Protocol::TransportStopMessage{});
    }

    void transportTempo(float tempo) {
        send(Protocol::TransportTempoMessage{tempo});
    }
