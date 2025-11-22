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
    std::function<void(const DeviceMacroDiscreteValuesMessage&)> onDeviceMacroDiscreteValues;
    std::function<void(const DeviceMacroNameChangeMessage&)> onDeviceMacroNameChange;
    std::function<void(const DeviceMacroTouchMessage&)> onDeviceMacroTouch;
    std::function<void(const DeviceMacroUpdateMessage&)> onDeviceMacroUpdate;
    std::function<void(const DeviceMacroValueChangeMessage&)> onDeviceMacroValueChange;
    std::function<void(const DevicePageChangeMessage&)> onDevicePageChange;
    std::function<void(const DevicePageNamesMessage&)> onDevicePageNames;
    std::function<void(const DevicePageSelectByIndexMessage&)> onDevicePageSelectByIndex;
    std::function<void(const DeviceSelectByIndexMessage&)> onDeviceSelectByIndex;
    std::function<void(const DeviceStateChangeMessage&)> onDeviceStateChange;
    std::function<void(const EnterDeviceChildMessage&)> onEnterDeviceChild;
    std::function<void(const ExitToParentMessage&)> onExitToParent;
    std::function<void(const RequestDeviceChildrenMessage&)> onRequestDeviceChildren;
    std::function<void(const RequestDeviceListMessage&)> onRequestDeviceList;
    std::function<void(const RequestDevicePageNamesMessage&)> onRequestDevicePageNames;
    std::function<void(const LastClickedTouchMessage&)> onLastClickedTouch;
    std::function<void(const LastClickedUpdateMessage&)> onLastClickedUpdate;
    std::function<void(const LastClickedValueChangeMessage&)> onLastClickedValueChange;
    std::function<void(const HostDeactivatedMessage&)> onHostDeactivated;
    std::function<void(const HostInitializedMessage&)> onHostInitialized;
    std::function<void(const RequestHostStatusMessage&)> onRequestHostStatus;
    std::function<void(const EnterTrackGroupMessage&)> onEnterTrackGroup;
    std::function<void(const ExitTrackGroupMessage&)> onExitTrackGroup;
    std::function<void(const RequestTrackListMessage&)> onRequestTrackList;
    std::function<void(const TrackActivateMessage&)> onTrackActivate;
    std::function<void(const TrackChangeMessage&)> onTrackChange;
    std::function<void(const TrackListMessage&)> onTrackList;
    std::function<void(const TrackMuteMessage&)> onTrackMute;
    std::function<void(const TrackSelectByIndexMessage&)> onTrackSelectByIndex;
    std::function<void(const TrackSoloMessage&)> onTrackSolo;
    std::function<void(const TransportPlayMessage&)> onTransportPlay;
    std::function<void(const TransportRecordMessage&)> onTransportRecord;
    std::function<void(const TransportStopMessage&)> onTransportStop;
    std::function<void(const TransportTempoMessage&)> onTransportTempo;

protected:
    ProtocolCallbacks() = default;
    virtual ~ProtocolCallbacks() = default;
};

}  // namespace Protocol
