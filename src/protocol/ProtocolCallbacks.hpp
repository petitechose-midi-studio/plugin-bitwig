/**
 * ProtocolCallbacks.hpp - Typed callbacks for all messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Base class providing typed callbacks for each message type.
 * Protocol inherits this and DecoderRegistry calls these callbacks.
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
    std::function<void(const DeviceListMessage&)> onDeviceList;
    std::function<void(const DeviceListWindowMessage&)> onDeviceListWindow;
    std::function<void(const DevicePageChangeMessage&)> onDevicePageChange;
    std::function<void(const DevicePageNamesMessage&)> onDevicePageNames;
    std::function<void(const DevicePageNamesWindowMessage&)> onDevicePageNamesWindow;
    std::function<void(const DevicePageSelectByIndexMessage&)> onDevicePageSelectByIndex;
    std::function<void(const DeviceRemoteControlsBatchMessage&)> onDeviceRemoteControlsBatch;
    std::function<void(const DeviceRemoteControlDiscreteValuesMessage&)> onDeviceRemoteControlDiscreteValues;
    std::function<void(const DeviceRemoteControlHasAutomationChangeMessage&)> onDeviceRemoteControlHasAutomationChange;
    std::function<void(const DeviceRemoteControlIsModulatedChangeMessage&)> onDeviceRemoteControlIsModulatedChange;
    std::function<void(const DeviceRemoteControlNameChangeMessage&)> onDeviceRemoteControlNameChange;
    std::function<void(const DeviceRemoteControlOriginChangeMessage&)> onDeviceRemoteControlOriginChange;
    std::function<void(const DeviceRemoteControlRestoreAutomationMessage&)> onDeviceRemoteControlRestoreAutomation;
    std::function<void(const DeviceRemoteControlTouchMessage&)> onDeviceRemoteControlTouch;
    std::function<void(const DeviceRemoteControlUpdateMessage&)> onDeviceRemoteControlUpdate;
    std::function<void(const DeviceRemoteControlValueChangeMessage&)> onDeviceRemoteControlValueChange;
    std::function<void(const DeviceSelectByIndexMessage&)> onDeviceSelectByIndex;
    std::function<void(const DeviceStateChangeMessage&)> onDeviceStateChange;
    std::function<void(const EnterDeviceChildMessage&)> onEnterDeviceChild;
    std::function<void(const ExitToParentMessage&)> onExitToParent;
    std::function<void(const RequestDeviceChildrenMessage&)> onRequestDeviceChildren;
    std::function<void(const RequestDeviceListMessage&)> onRequestDeviceList;
    std::function<void(const RequestDeviceListWindowMessage&)> onRequestDeviceListWindow;
    std::function<void(const RequestDevicePageNamesMessage&)> onRequestDevicePageNames;
    std::function<void(const RequestDevicePageNamesWindowMessage&)> onRequestDevicePageNamesWindow;
    std::function<void(const ViewStateChangeMessage&)> onViewStateChange;
    std::function<void(const LastClickedTouchMessage&)> onLastClickedTouch;
    std::function<void(const LastClickedUpdateMessage&)> onLastClickedUpdate;
    std::function<void(const LastClickedValueChangeMessage&)> onLastClickedValueChange;
    std::function<void(const HostDeactivatedMessage&)> onHostDeactivated;
    std::function<void(const HostInitializedMessage&)> onHostInitialized;
    std::function<void(const RequestHostStatusMessage&)> onRequestHostStatus;
    std::function<void(const EnterTrackGroupMessage&)> onEnterTrackGroup;
    std::function<void(const ExitTrackGroupMessage&)> onExitTrackGroup;
    std::function<void(const RequestSendDestinationsMessage&)> onRequestSendDestinations;
    std::function<void(const RequestTrackListMessage&)> onRequestTrackList;
    std::function<void(const RequestTrackListWindowMessage&)> onRequestTrackListWindow;
    std::function<void(const RequestTrackSendListMessage&)> onRequestTrackSendList;
    std::function<void(const SelectMixSendMessage&)> onSelectMixSend;
    std::function<void(const SendDestinationsListMessage&)> onSendDestinationsList;
    std::function<void(const TrackActivateMessage&)> onTrackActivate;
    std::function<void(const TrackArmChangeMessage&)> onTrackArmChange;
    std::function<void(const TrackChangeMessage&)> onTrackChange;
    std::function<void(const TrackListMessage&)> onTrackList;
    std::function<void(const TrackListWindowMessage&)> onTrackListWindow;
    std::function<void(const TrackMuteMessage&)> onTrackMute;
    std::function<void(const TrackMutedBySoloChangeMessage&)> onTrackMutedBySoloChange;
    std::function<void(const TrackPanChangeMessage&)> onTrackPanChange;
    std::function<void(const TrackPanHasAutomationChangeMessage&)> onTrackPanHasAutomationChange;
    std::function<void(const TrackPanModulatedValueChangeMessage&)> onTrackPanModulatedValueChange;
    std::function<void(const TrackPanTouchMessage&)> onTrackPanTouch;
    std::function<void(const TrackSelectByIndexMessage&)> onTrackSelectByIndex;
    std::function<void(const TrackSendEnabledChangeMessage&)> onTrackSendEnabledChange;
    std::function<void(const TrackSendHasAutomationChangeMessage&)> onTrackSendHasAutomationChange;
    std::function<void(const TrackSendListMessage&)> onTrackSendList;
    std::function<void(const TrackSendModeChangeMessage&)> onTrackSendModeChange;
    std::function<void(const TrackSendModulatedValueChangeMessage&)> onTrackSendModulatedValueChange;
    std::function<void(const TrackSendPreFaderChangeMessage&)> onTrackSendPreFaderChange;
    std::function<void(const TrackSendTouchMessage&)> onTrackSendTouch;
    std::function<void(const TrackSendValueChangeMessage&)> onTrackSendValueChange;
    std::function<void(const TrackSoloMessage&)> onTrackSolo;
    std::function<void(const TrackVolumeChangeMessage&)> onTrackVolumeChange;
    std::function<void(const TrackVolumeHasAutomationChangeMessage&)> onTrackVolumeHasAutomationChange;
    std::function<void(const TrackVolumeModulatedValueChangeMessage&)> onTrackVolumeModulatedValueChange;
    std::function<void(const TrackVolumeTouchMessage&)> onTrackVolumeTouch;
    std::function<void(const ResetAutomationOverridesMessage&)> onResetAutomationOverrides;
    std::function<void(const TransportArrangerAutomationWriteEnabledChangeMessage&)> onTransportArrangerAutomationWriteEnabledChange;
    std::function<void(const TransportArrangerOverdubEnabledChangeMessage&)> onTransportArrangerOverdubEnabledChange;
    std::function<void(const TransportAutomationOverrideActiveChangeMessage&)> onTransportAutomationOverrideActiveChange;
    std::function<void(const TransportAutomationWriteModeChangeMessage&)> onTransportAutomationWriteModeChange;
    std::function<void(const TransportClipLauncherAutomationWriteEnabledChangeMessage&)> onTransportClipLauncherAutomationWriteEnabledChange;
    std::function<void(const TransportClipLauncherOverdubEnabledChangeMessage&)> onTransportClipLauncherOverdubEnabledChange;
    std::function<void(const TransportPlayMessage&)> onTransportPlay;
    std::function<void(const TransportRecordMessage&)> onTransportRecord;
    std::function<void(const TransportStopMessage&)> onTransportStop;
    std::function<void(const TransportTempoMessage&)> onTransportTempo;

protected:
    ProtocolCallbacks() = default;
    virtual ~ProtocolCallbacks() = default;
};

}  // namespace Protocol
