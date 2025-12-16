/**
 * MessageStructure.hpp - Umbrella header for all protocol messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * This file includes all message struct definitions.
 * Use this single include in your code instead of including individual structs.
 *
 * Usage:
 *   #include "MessageStructure.hpp"
 *
 *   TransportPlayMessage msg{true};
 *   msg.encode(buffer, bufferSize);
 */

#pragma once

// Include all message structs
#include "struct/DeviceChangeMessage.hpp"
#include "struct/DeviceChangeHeaderMessage.hpp"
#include "struct/DeviceChildrenMessage.hpp"
#include "struct/DeviceListMessage.hpp"
#include "struct/DevicePageChangeMessage.hpp"
#include "struct/DevicePageNamesMessage.hpp"
#include "struct/DevicePageSelectByIndexMessage.hpp"
#include "struct/DeviceRemoteControlDiscreteValuesMessage.hpp"
#include "struct/DeviceRemoteControlNameChangeMessage.hpp"
#include "struct/DeviceRemoteControlTouchMessage.hpp"
#include "struct/DeviceRemoteControlUpdateMessage.hpp"
#include "struct/DeviceRemoteControlValueChangeMessage.hpp"
#include "struct/DeviceSelectByIndexMessage.hpp"
#include "struct/DeviceStateChangeMessage.hpp"
#include "struct/EnterDeviceChildMessage.hpp"
#include "struct/ExitToParentMessage.hpp"
#include "struct/RequestDeviceChildrenMessage.hpp"
#include "struct/RequestDeviceListMessage.hpp"
#include "struct/RequestDevicePageNamesMessage.hpp"
#include "struct/LastClickedTouchMessage.hpp"
#include "struct/LastClickedUpdateMessage.hpp"
#include "struct/LastClickedValueChangeMessage.hpp"
#include "struct/HostDeactivatedMessage.hpp"
#include "struct/HostInitializedMessage.hpp"
#include "struct/RequestHostStatusMessage.hpp"
#include "struct/EnterTrackGroupMessage.hpp"
#include "struct/ExitTrackGroupMessage.hpp"
#include "struct/RequestTrackListMessage.hpp"
#include "struct/TrackActivateMessage.hpp"
#include "struct/TrackChangeMessage.hpp"
#include "struct/TrackListMessage.hpp"
#include "struct/TrackMuteMessage.hpp"
#include "struct/TrackSelectByIndexMessage.hpp"
#include "struct/TrackSoloMessage.hpp"
#include "struct/TransportPlayMessage.hpp"
#include "struct/TransportRecordMessage.hpp"
#include "struct/TransportStopMessage.hpp"
#include "struct/TransportTempoMessage.hpp"
