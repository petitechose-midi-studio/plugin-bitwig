/**
 * ProtocolMethods.hpp - Explicit Protocol API
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * This file provides explicit API methods instead of generic send<T>().
 * Include this file in your Protocol class definition.
 *
 * Pattern:
 *   TO_HOST messages (Controller -> Host): void methodName(params)
 *   TO_CONTROLLER messages (Host -> Controller): std::function<void(...)> onMethodName
 */

#pragma once

#include "MessageStructure.hpp"
#include <functional>
#include <string>
#include <array>
#include <vector>

namespace Protocol {

/**
 * Explicit protocol methods (include in Protocol class)
 */
class ProtocolMethods {
public:
    // =========================================================================
    // COMMANDS (Controller -> Host)
    // =========================================================================

    void devicePageSelect(uint8_t devicePageIndex) {
        send(DevicePageSelectByIndexMessage{devicePageIndex});
    }

    void deviceRemoteControlRestoreAutomation(uint8_t remoteControlIndex) {
        send(DeviceRemoteControlRestoreAutomationMessage{remoteControlIndex});
    }

    void deviceRemoteControlTouch(uint8_t remoteControlIndex, bool isTouched) {
        send(DeviceRemoteControlTouchMessage{remoteControlIndex, isTouched});
    }

    void deviceSelect(uint8_t deviceIndex) {
        send(DeviceSelectByIndexMessage{deviceIndex});
    }

    void deviceState(uint8_t deviceIndex, bool isEnabled) {
        send(DeviceStateChangeMessage{deviceIndex, isEnabled});
    }

    void enterDeviceChild(uint8_t deviceIndex, uint8_t childType, uint8_t childIndex) {
        send(EnterDeviceChildMessage{deviceIndex, childType, childIndex});
    }

    void exitToParent() {
        send(ExitToParentMessage{});
    }

    void remoteControlValue(uint8_t remoteControlIndex, float parameterValue) {
        send(RemoteControlValueMessage{remoteControlIndex, parameterValue});
    }

    void requestDeviceChildren(uint8_t deviceIndex, uint8_t childType) {
        send(RequestDeviceChildrenMessage{deviceIndex, childType});
    }

    void requestDeviceListWindow(uint8_t deviceStartIndex) {
        send(RequestDeviceListWindowMessage{deviceStartIndex});
    }

    void requestDevicePageNamesWindow(uint8_t pageStartIndex) {
        send(RequestDevicePageNamesWindowMessage{pageStartIndex});
    }

    void viewState(uint8_t viewType, bool selectorActive) {
        send(ViewStateChangeMessage{viewType, selectorActive});
    }

    void lastClickedTouch(bool isTouched) {
        send(LastClickedTouchMessage{isTouched});
    }

    void lastClickedValue(float parameterValue) {
        send(LastClickedValueMessage{parameterValue});
    }

    void requestHostStatus() {
        send(RequestHostStatusMessage{});
    }

    void enterTrackGroup(uint8_t trackIndex) {
        send(EnterTrackGroupMessage{trackIndex});
    }

    void exitTrackGroup() {
        send(ExitTrackGroupMessage{});
    }

    void requestSendDestinations() {
        send(RequestSendDestinationsMessage{});
    }

    void requestTrackListWindow(uint8_t trackStartIndex) {
        send(RequestTrackListWindowMessage{trackStartIndex});
    }

    void requestTrackSendList() {
        send(RequestTrackSendListMessage{});
    }

    void selectMixSend(uint8_t sendIndex) {
        send(SelectMixSendMessage{sendIndex});
    }

    void trackActivate(uint8_t trackIndex) {
        send(TrackActivateMessage{trackIndex});
    }

    void trackArm(uint8_t trackIndex, bool isArm) {
        send(TrackArmMessage{trackIndex, isArm});
    }

    void trackMute(uint8_t trackIndex, bool isMute) {
        send(TrackMuteMessage{trackIndex, isMute});
    }

    void trackPan(uint8_t trackIndex, float pan) {
        send(TrackPanMessage{trackIndex, pan});
    }

    void trackPanTouch(uint8_t trackIndex, bool isTouched) {
        send(TrackPanTouchMessage{trackIndex, isTouched});
    }

    void trackSelect(uint8_t trackIndex) {
        send(TrackSelectByIndexMessage{trackIndex});
    }

    void trackSendEnabled(uint8_t trackIndex, uint8_t sendIndex, bool sendIsEnabled) {
        send(TrackSendEnabledMessage{trackIndex, sendIndex, sendIsEnabled});
    }

    void trackSendMode(uint8_t trackIndex, uint8_t sendIndex, const std::string& sendMode) {
        send(TrackSendModeMessage{trackIndex, sendIndex, sendMode});
    }

    void trackSendTouch(uint8_t trackIndex, uint8_t sendIndex, bool isTouched) {
        send(TrackSendTouchMessage{trackIndex, sendIndex, isTouched});
    }

    void trackSendValue(uint8_t trackIndex, uint8_t sendIndex, float sendValue) {
        send(TrackSendValueMessage{trackIndex, sendIndex, sendValue});
    }

    void trackSolo(uint8_t trackIndex, bool isSolo) {
        send(TrackSoloMessage{trackIndex, isSolo});
    }

    void trackVolume(uint8_t trackIndex, float volume) {
        send(TrackVolumeMessage{trackIndex, volume});
    }

    void trackVolumeTouch(uint8_t trackIndex, bool isTouched) {
        send(TrackVolumeTouchMessage{trackIndex, isTouched});
    }

    void resetAutomationOverrides() {
        send(ResetAutomationOverridesMessage{});
    }

    void transportArrangerAutomationWriteEnabled(bool isArrangerAutomationWriteEnabled) {
        send(TransportArrangerAutomationWriteEnabledMessage{isArrangerAutomationWriteEnabled});
    }

    void transportArrangerOverdubEnabled(bool isArrangerOverdubEnabled) {
        send(TransportArrangerOverdubEnabledMessage{isArrangerOverdubEnabled});
    }

    void transportAutomationWriteMode(uint8_t automationWriteMode) {
        send(TransportAutomationWriteModeMessage{automationWriteMode});
    }

    void transportClipLauncherAutomationWriteEnabled(bool isClipLauncherAutomationWriteEnabled) {
        send(TransportClipLauncherAutomationWriteEnabledMessage{isClipLauncherAutomationWriteEnabled});
    }

    void transportClipLauncherOverdubEnabled(bool isClipLauncherOverdubEnabled) {
        send(TransportClipLauncherOverdubEnabledMessage{isClipLauncherOverdubEnabled});
    }

    void transportPlay(bool isPlaying) {
        send(TransportPlayMessage{isPlaying});
    }

    void transportRecord(bool isRecording) {
        send(TransportRecordMessage{isRecording});
    }

    void transportStop() {
        send(TransportStopMessage{});
    }

    void transportTempo(float tempo) {
        send(TransportTempoMessage{tempo});
    }

    // =========================================================================
    // NOTIFICATIONS (Host -> Controller) - Callbacks
    // =========================================================================

    std::function<void(const DeviceChangeMessage&)> onDevice;
    std::function<void(const DeviceChangeHeaderMessage&)> onDeviceChangeHeader;
    std::function<void(const DeviceChildrenMessage&)> onDeviceChildren;
    std::function<void(const DeviceListWindowMessage&)> onDeviceListWindow;
    std::function<void(const DevicePageChangeMessage&)> onDevicePage;
    std::function<void(const DevicePageNamesWindowMessage&)> onDevicePageNamesWindow;
    std::function<void(const DeviceRemoteControlsBatchMessage&)> onDeviceRemoteControlsBatch;
    std::function<void(const DeviceRemoteControlDiscreteValuesMessage&)> onDeviceRemoteControlDiscreteValues;
    std::function<void(const DeviceRemoteControlHasAutomationChangeMessage&)> onDeviceRemoteControlHasAutomation;
    std::function<void(const DeviceRemoteControlIsModulatedChangeMessage&)> onDeviceRemoteControlIsModulated;
    std::function<void(const DeviceRemoteControlNameChangeMessage&)> onDeviceRemoteControlName;
    std::function<void(const DeviceRemoteControlOriginChangeMessage&)> onDeviceRemoteControlOrigin;
    std::function<void(const DeviceRemoteControlUpdateMessage&)> onDeviceRemoteControlUpdate;
    std::function<void(const RemoteControlValueStateMessage&)> onRemoteControlValueState;
    std::function<void(const LastClickedUpdateMessage&)> onLastClickedUpdate;
    std::function<void(const LastClickedValueStateMessage&)> onLastClickedValueState;
    std::function<void(const HostDeactivatedMessage&)> onHostDeactivated;
    std::function<void(const HostInitializedMessage&)> onHostInitialized;
    std::function<void(const SendDestinationsListMessage&)> onSendDestinationsList;
    std::function<void(const TrackArmStateMessage&)> onTrackArmState;
    std::function<void(const TrackChangeMessage&)> onTrack;
    std::function<void(const TrackListWindowMessage&)> onTrackListWindow;
    std::function<void(const TrackMutedBySoloStateMessage&)> onTrackMutedBySoloState;
    std::function<void(const TrackMuteStateMessage&)> onTrackMuteState;
    std::function<void(const TrackPanHasAutomationStateMessage&)> onTrackPanHasAutomationState;
    std::function<void(const TrackPanModulatedValueStateMessage&)> onTrackPanModulatedValueState;
    std::function<void(const TrackPanStateMessage&)> onTrackPanState;
    std::function<void(const TrackSendEnabledStateMessage&)> onTrackSendEnabledState;
    std::function<void(const TrackSendHasAutomationStateMessage&)> onTrackSendHasAutomationState;
    std::function<void(const TrackSendListMessage&)> onTrackSendList;
    std::function<void(const TrackSendModeStateMessage&)> onTrackSendModeState;
    std::function<void(const TrackSendModulatedValueStateMessage&)> onTrackSendModulatedValueState;
    std::function<void(const TrackSendPreFaderStateMessage&)> onTrackSendPreFaderState;
    std::function<void(const TrackSendValueStateMessage&)> onTrackSendValueState;
    std::function<void(const TrackSoloStateMessage&)> onTrackSoloState;
    std::function<void(const TrackVolumeHasAutomationStateMessage&)> onTrackVolumeHasAutomationState;
    std::function<void(const TrackVolumeModulatedValueStateMessage&)> onTrackVolumeModulatedValueState;
    std::function<void(const TrackVolumeStateMessage&)> onTrackVolumeState;
    std::function<void(const TransportArrangerAutomationWriteEnabledStateMessage&)> onTransportArrangerAutomationWriteEnabledState;
    std::function<void(const TransportArrangerOverdubEnabledStateMessage&)> onTransportArrangerOverdubEnabledState;
    std::function<void(const TransportAutomationOverrideActiveStateMessage&)> onTransportAutomationOverrideActiveState;
    std::function<void(const TransportAutomationWriteModeStateMessage&)> onTransportAutomationWriteModeState;
    std::function<void(const TransportClipLauncherAutomationWriteEnabledStateMessage&)> onTransportClipLauncherAutomationWriteEnabledState;
    std::function<void(const TransportClipLauncherOverdubEnabledStateMessage&)> onTransportClipLauncherOverdubEnabledState;
    std::function<void(const TransportPlayingStateMessage&)> onTransportPlayingState;
    std::function<void(const TransportRecordingStateMessage&)> onTransportRecordingState;
    std::function<void(const TransportTempoStateMessage&)> onTransportTempoState;

protected:
    ProtocolMethods() = default;
    virtual ~ProtocolMethods() = default;

    // Subclass must implement send
    template <typename T>
    void send(const T& msg);
};

}  // namespace Protocol
