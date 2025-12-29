/**
 * MessageID.hpp - SysEx Message ID Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: sysex_messages.py + message_id_allocator
 *
 * This file defines the MessageID enum containing all valid SysEx message
 * identifiers. IDs are auto-allocated sequentially starting from 0x00.
 *
 * Total messages: 91
 */

#pragma once

#include <cstdint>

namespace Protocol {

/**
 * SysEx Message Identifiers
 *
 * Type-safe enum for all protocol messages.
 * Values are auto-allocated sequentially.
 */
enum class MessageID : uint8_t {

    // ========================================
    // Protocol Messages
    // ========================================

    DEVICE_CHANGE = 0x00,  // Complete device state (name, enabled, page, 8 remote controls)
    DEVICE_CHANGE_HEADER = 0x01,  // Device change header - lightweight message with device identity only
    DEVICE_CHILDREN = 0x02,  // List of children for a device of specific type
    DEVICE_LIST_WINDOW = 0x03,  // Windowed device list response (16 items max)
    DEVICE_PAGE_CHANGE = 0x04,  // Page change with new remote controls set
    DEVICE_PAGE_NAMES_WINDOW = 0x05,  // Windowed page names response (16 items max)
    DEVICE_PAGE_SELECT_BY_INDEX = 0x06,  // Select device page by index (modulo pageCount)
    DEVICE_REMOTE_CONTROLS_BATCH = 0x07,  // Combined batch update of all 8 remote control values and modulated values (sent at fixed rate ~50Hz)
    DEVICE_REMOTE_CONTROL_DISCRETE_VALUES = 0x08,  // Full list of discrete values for List parameters (lazy-loaded on demand)
    DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE = 0x09,  // hasAutomation() state changed for remote control
    DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE = 0x0A,  // isModulated state changed for remote control
    DEVICE_REMOTE_CONTROL_NAME_CHANGE = 0x0B,  // Single remote control name change
    DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE = 0x0C,  // origin changed for remote control (bipolar center point)
    DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION = 0x0D,  // Controller requests host to restore automation playback for parameter
    DEVICE_REMOTE_CONTROL_TOUCH = 0x0E,  // Touch automation start/stop for remote control parameter
    DEVICE_REMOTE_CONTROL_UPDATE = 0x0F,  // Complete remote control update - sent individually per parameter
    DEVICE_SELECT_BY_INDEX = 0x10,  // Select device by index in current chain
    DEVICE_STATE_CHANGE = 0x11,  // Toggle device enabled/bypassed by index
    ENTER_DEVICE_CHILD = 0x12,  // Navigate into a child (slot/layer/drum pad)
    ENTER_TRACK_GROUP = 0x13,  // Navigate into a track group to see its children
    EXIT_TO_PARENT = 0x14,  // Navigate back to parent device chain
    EXIT_TRACK_GROUP = 0x15,  // Navigate back to parent track context
    HOST_DEACTIVATED = 0x16,  // Host plugin deactivating
    HOST_INITIALIZED = 0x17,  // Host plugin initialized and active
    LAST_CLICKED_TOUCH = 0x18,  // Touch automation for last clicked parameter
    LAST_CLICKED_UPDATE = 0x19,  // Last clicked parameter update - sent when user clicks a new parameter
    LAST_CLICKED_VALUE = 0x1A,  // Set last clicked parameter value
    LAST_CLICKED_VALUE_STATE = 0x1B,  // Last clicked parameter value state (confirmation after change)
    REMOTE_CONTROL_VALUE = 0x1C,  // Set remote control value
    REMOTE_CONTROL_VALUE_STATE = 0x1D,  // Remote control value state (confirmation with display value)
    REQUEST_DEVICE_CHILDREN = 0x1E,  // Request children (slots/layers/drums) for device and type
    REQUEST_DEVICE_LIST_WINDOW = 0x1F,  // Request device list starting at index (windowed, 16 items)
    REQUEST_DEVICE_PAGE_NAMES_WINDOW = 0x20,  // Request page names starting at index (windowed, 16 items)
    REQUEST_HOST_STATUS = 0x21,  // Request current host status (triggers HOST_INITIALIZED response)
    REQUEST_SEND_DESTINATIONS = 0x22,  // Request list of send destination names
    REQUEST_TRACK_LIST_WINDOW = 0x23,  // Request track list starting at index (windowed, 16 items)
    REQUEST_TRACK_SEND_LIST = 0x24,  // Request list of sends for current track
    RESET_AUTOMATION_OVERRIDES = 0x25,  // Reset all automation overrides globally (resetAutomationOverrides())
    SELECT_MIX_SEND = 0x26,  // Select which send to observe for MixView
    SEND_DESTINATIONS_LIST = 0x27,  // List of send destination names (effect track names)
    TRACK_ACTIVATE = 0x28,  // Toggle track activated/deactivated state
    TRACK_ARM = 0x29,  // Set track record arm state
    TRACK_ARM_STATE = 0x2A,  // Track record arm state changed
    TRACK_CHANGE = 0x2B,  // Track context change notification with full channel state
    TRACK_LIST_WINDOW = 0x2C,  // Windowed track list response (16 items max)
    TRACK_MUTE = 0x2D,  // Set track mute state
    TRACK_MUTED_BY_SOLO_STATE = 0x2E,  // Track muted by solo state changed
    TRACK_MUTE_STATE = 0x2F,  // Track mute state changed
    TRACK_PAN = 0x30,  // Set track pan
    TRACK_PAN_HAS_AUTOMATION_STATE = 0x31,  // Track pan hasAutomation() state changed
    TRACK_PAN_MODULATED_VALUE_STATE = 0x32,  // Track pan modulatedValue() changed
    TRACK_PAN_STATE = 0x33,  // Track pan state
    TRACK_PAN_TOUCH = 0x34,  // Touch automation start/stop for track pan
    TRACK_SELECT_BY_INDEX = 0x35,  // Select track by index in current context
    TRACK_SEND_ENABLED = 0x36,  // Set track send enabled state
    TRACK_SEND_ENABLED_STATE = 0x37,  // Track send enabled state changed
    TRACK_SEND_HAS_AUTOMATION_STATE = 0x38,  // Track send hasAutomation() state changed
    TRACK_SEND_LIST = 0x39,  // List of sends for current track
    TRACK_SEND_MODE = 0x3A,  // Set track send mode (AUTO, PRE, POST)
    TRACK_SEND_MODE_STATE = 0x3B,  // Track send mode changed
    TRACK_SEND_MODULATED_VALUE_STATE = 0x3C,  // Track send modulatedValue() changed
    TRACK_SEND_PRE_FADER_STATE = 0x3D,  // Track send pre-fader state changed
    TRACK_SEND_TOUCH = 0x3E,  // Touch automation start/stop for track send
    TRACK_SEND_VALUE = 0x3F,  // Set track send value
    TRACK_SEND_VALUE_STATE = 0x40,  // Track send value state
    TRACK_SOLO = 0x41,  // Set track solo state
    TRACK_SOLO_STATE = 0x42,  // Track solo state changed
    TRACK_VOLUME = 0x43,  // Set track volume
    TRACK_VOLUME_HAS_AUTOMATION_STATE = 0x44,  // Track volume hasAutomation() state changed
    TRACK_VOLUME_MODULATED_VALUE_STATE = 0x45,  // Track volume modulatedValue() changed
    TRACK_VOLUME_STATE = 0x46,  // Track volume state
    TRACK_VOLUME_TOUCH = 0x47,  // Touch automation start/stop for track volume
    TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED = 0x48,  // Set arranger automation write enabled state
    TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE = 0x49,  // isArrangerAutomationWriteEnabled() state changed
    TRANSPORT_ARRANGER_OVERDUB_ENABLED = 0x4A,  // Set arranger overdub enabled state
    TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE = 0x4B,  // isArrangerOverdubEnabled() state changed
    TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE = 0x4C,  // isAutomationOverrideActive() state changed
    TRANSPORT_AUTOMATION_WRITE_MODE = 0x4D,  // Set automation write mode (latch/touch/write)
    TRANSPORT_AUTOMATION_WRITE_MODE_STATE = 0x4E,  // automationWriteMode() state changed
    TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED = 0x4F,  // Set clip launcher automation write enabled state
    TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_STATE = 0x50,  // isClipLauncherAutomationWriteEnabled() state changed
    TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED = 0x51,  // Set clip launcher overdub enabled state
    TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE = 0x52,  // isClipLauncherOverdubEnabled() state changed
    TRANSPORT_PLAY = 0x53,  // Set transport play state
    TRANSPORT_PLAYING_STATE = 0x54,  // Transport playing state changed
    TRANSPORT_RECORD = 0x55,  // Set transport record state
    TRANSPORT_RECORDING_STATE = 0x56,  // Transport recording state changed
    TRANSPORT_STOP = 0x57,  // Stop transport
    TRANSPORT_TEMPO = 0x58,  // Adjust tempo (relative or absolute)
    TRANSPORT_TEMPO_STATE = 0x59,  // Tempo value notification
    VIEW_STATE_CHANGE = 0x5A,  // Controller view state changed (view type or selector visibility)

};

/**
 * Total number of defined messages
 */
constexpr uint8_t MESSAGE_COUNT = 91;


}  // namespace Protocol
