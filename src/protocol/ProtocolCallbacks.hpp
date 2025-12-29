/**
 * ProtocolCallbacks.hpp - Typed callbacks for all messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Base class providing typed callbacks for each message type.
 * Protocol inherits this and DecoderRegistry calls these callbacks.
 *
 * For explicit send methods, include ProtocolMethods.inl in your Protocol class.
 *
 * Usage:
 *   protocol.onTransportPlay = [](const TransportPlayMessage& msg) {
 *       // Handle message
 *   };
 */

#pragma once

#include "MessageStructure.hpp"
#include <functional>

namespace Protocol {

/**
 * Base class with typed callbacks for each message
 */
class ProtocolCallbacks {
public:
    // ========================================================================
    // Typed callbacks (one per message)
    // ========================================================================

    std::function<void(const DeviceChangeMessage&)> onDeviceChange;
    std::function<void(const DeviceChangeHeaderMessage&)> onDeviceChangeHeader;
    std::function<void(const DeviceChildrenMessage&)> onDeviceChildren;
    std::function<void(const DeviceEnabledStateMessage&)> onDeviceEnabledState;
    std::function<void(const DeviceListWindowMessage&)> onDeviceListWindow;
    std::function<void(const DevicePageChangeMessage&)> onDevicePageChange;
    std::function<void(const DevicePageNamesWindowMessage&)> onDevicePageNamesWindow;
    std::function<void(const DevicePageSelectMessage&)> onDevicePageSelect;
    std::function<void(const DeviceRemoteControlsBatchMessage&)> onDeviceRemoteControlsBatch;
    std::function<void(const DeviceRemoteControlDiscreteValuesMessage&)> onDeviceRemoteControlDiscreteValues;
    std::function<void(const DeviceRemoteControlHasAutomationChangeMessage&)> onDeviceRemoteControlHasAutomationChange;
    std::function<void(const DeviceRemoteControlIsModulatedChangeMessage&)> onDeviceRemoteControlIsModulatedChange;
    std::function<void(const DeviceRemoteControlNameChangeMessage&)> onDeviceRemoteControlNameChange;
    std::function<void(const DeviceRemoteControlOriginChangeMessage&)> onDeviceRemoteControlOriginChange;
    std::function<void(const DeviceRemoteControlRestoreAutomationMessage&)> onDeviceRemoteControlRestoreAutomation;
    std::function<void(const DeviceRemoteControlTouchMessage&)> onDeviceRemoteControlTouch;
    std::function<void(const DeviceRemoteControlUpdateMessage&)> onDeviceRemoteControlUpdate;
    std::function<void(const DeviceSelectMessage&)> onDeviceSelect;
    std::function<void(const DeviceStateMessage&)> onDeviceState;
    std::function<void(const EnterDeviceChildMessage&)> onEnterDeviceChild;
    std::function<void(const ExitToParentMessage&)> onExitToParent;
    std::function<void(const RemoteControlValueMessage&)> onRemoteControlValue;
    std::function<void(const RemoteControlValueStateMessage&)> onRemoteControlValueState;
    std::function<void(const RequestDeviceChildrenMessage&)> onRequestDeviceChildren;
    std::function<void(const RequestDeviceListWindowMessage&)> onRequestDeviceListWindow;
    std::function<void(const RequestDevicePageNamesWindowMessage&)> onRequestDevicePageNamesWindow;
    std::function<void(const ViewStateMessage&)> onViewState;
    std::function<void(const LastClickedTouchMessage&)> onLastClickedTouch;
    std::function<void(const LastClickedUpdateMessage&)> onLastClickedUpdate;
    std::function<void(const LastClickedValueMessage&)> onLastClickedValue;
    std::function<void(const LastClickedValueStateMessage&)> onLastClickedValueState;
    std::function<void(const HostDeactivatedMessage&)> onHostDeactivated;
    std::function<void(const HostInitializedMessage&)> onHostInitialized;
    std::function<void(const RequestHostStatusMessage&)> onRequestHostStatus;
    std::function<void(const EnterTrackGroupMessage&)> onEnterTrackGroup;
    std::function<void(const ExitTrackGroupMessage&)> onExitTrackGroup;
    std::function<void(const RequestSendDestinationsMessage&)> onRequestSendDestinations;
    std::function<void(const RequestTrackListWindowMessage&)> onRequestTrackListWindow;
    std::function<void(const RequestTrackSendListMessage&)> onRequestTrackSendList;
    std::function<void(const SelectMixSendMessage&)> onSelectMixSend;
    std::function<void(const SendDestinationsListMessage&)> onSendDestinationsList;
    std::function<void(const TrackActivateMessage&)> onTrackActivate;
    std::function<void(const TrackArmMessage&)> onTrackArm;
    std::function<void(const TrackArmStateMessage&)> onTrackArmState;
    std::function<void(const TrackChangeMessage&)> onTrackChange;
    std::function<void(const TrackListWindowMessage&)> onTrackListWindow;
    std::function<void(const TrackMuteMessage&)> onTrackMute;
    std::function<void(const TrackMutedBySoloStateMessage&)> onTrackMutedBySoloState;
    std::function<void(const TrackMuteStateMessage&)> onTrackMuteState;
    std::function<void(const TrackPanMessage&)> onTrackPan;
    std::function<void(const TrackPanHasAutomationStateMessage&)> onTrackPanHasAutomationState;
    std::function<void(const TrackPanModulatedValueStateMessage&)> onTrackPanModulatedValueState;
    std::function<void(const TrackPanStateMessage&)> onTrackPanState;
    std::function<void(const TrackPanTouchMessage&)> onTrackPanTouch;
    std::function<void(const TrackSelectMessage&)> onTrackSelect;
    std::function<void(const TrackSendEnabledMessage&)> onTrackSendEnabled;
    std::function<void(const TrackSendEnabledStateMessage&)> onTrackSendEnabledState;
    std::function<void(const TrackSendHasAutomationStateMessage&)> onTrackSendHasAutomationState;
    std::function<void(const TrackSendListMessage&)> onTrackSendList;
    std::function<void(const TrackSendModeMessage&)> onTrackSendMode;
    std::function<void(const TrackSendModeStateMessage&)> onTrackSendModeState;
    std::function<void(const TrackSendModulatedValueStateMessage&)> onTrackSendModulatedValueState;
    std::function<void(const TrackSendPreFaderStateMessage&)> onTrackSendPreFaderState;
    std::function<void(const TrackSendTouchMessage&)> onTrackSendTouch;
    std::function<void(const TrackSendValueMessage&)> onTrackSendValue;
    std::function<void(const TrackSendValueStateMessage&)> onTrackSendValueState;
    std::function<void(const TrackSoloMessage&)> onTrackSolo;
    std::function<void(const TrackSoloStateMessage&)> onTrackSoloState;
    std::function<void(const TrackVolumeMessage&)> onTrackVolume;
    std::function<void(const TrackVolumeHasAutomationStateMessage&)> onTrackVolumeHasAutomationState;
    std::function<void(const TrackVolumeModulatedValueStateMessage&)> onTrackVolumeModulatedValueState;
    std::function<void(const TrackVolumeStateMessage&)> onTrackVolumeState;
    std::function<void(const TrackVolumeTouchMessage&)> onTrackVolumeTouch;
    std::function<void(const ResetAutomationOverridesMessage&)> onResetAutomationOverrides;
    std::function<void(const TransportArrangerAutomationWriteEnabledMessage&)> onTransportArrangerAutomationWriteEnabled;
    std::function<void(const TransportArrangerAutomationWriteEnabledStateMessage&)> onTransportArrangerAutomationWriteEnabledState;
    std::function<void(const TransportArrangerOverdubEnabledMessage&)> onTransportArrangerOverdubEnabled;
    std::function<void(const TransportArrangerOverdubEnabledStateMessage&)> onTransportArrangerOverdubEnabledState;
    std::function<void(const TransportAutomationOverrideActiveStateMessage&)> onTransportAutomationOverrideActiveState;
    std::function<void(const TransportAutomationWriteModeMessage&)> onTransportAutomationWriteMode;
    std::function<void(const TransportAutomationWriteModeStateMessage&)> onTransportAutomationWriteModeState;
    std::function<void(const TransportClipLauncherAutomationWriteEnabledMessage&)> onTransportClipLauncherAutomationWriteEnabled;
    std::function<void(const TransportClipLauncherAutomationWriteEnabledStateMessage&)> onTransportClipLauncherAutomationWriteEnabledState;
    std::function<void(const TransportClipLauncherOverdubEnabledMessage&)> onTransportClipLauncherOverdubEnabled;
    std::function<void(const TransportClipLauncherOverdubEnabledStateMessage&)> onTransportClipLauncherOverdubEnabledState;
    std::function<void(const TransportPlayMessage&)> onTransportPlay;
    std::function<void(const TransportPlayingStateMessage&)> onTransportPlayingState;
    std::function<void(const TransportRecordMessage&)> onTransportRecord;
    std::function<void(const TransportRecordingStateMessage&)> onTransportRecordingState;
    std::function<void(const TransportStopMessage&)> onTransportStop;
    std::function<void(const TransportTempoMessage&)> onTransportTempo;
    std::function<void(const TransportTempoStateMessage&)> onTransportTempoState;

protected:
    ProtocolCallbacks() = default;
    virtual ~ProtocolCallbacks() = default;
};

}  // namespace Protocol
