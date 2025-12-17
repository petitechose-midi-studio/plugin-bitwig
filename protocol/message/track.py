from field.track import *
from field.send import *
from protocol_codegen.core.message import Message

# ============================================================================
# TRACK CHANGE NOTIFICATION (Composite - Full State)
# ============================================================================

# Host → Controller: Track context changed (cursor track selection)
# Includes all channel parameters for the selected track
TRACK_CHANGE = Message(
    description='Track context change notification with full channel state',
    fields=[
        track_name,            # STRING (max 16 chars)
        track_color,           # UINT32 (0xRRGGBB)
        track_index,           # UINT8 (current track position)
        track_type,            # UINT8 (0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master)
        track_is_activated,    # BOOL
        track_is_mute,         # BOOL
        track_is_solo,         # BOOL
        track_is_muted_by_solo,# BOOL (read-only)
        track_is_arm,          # BOOL
        track_volume,          # FLOAT32 (0.0-1.0)
        track_volume_display,  # STRING ("0 dB", "-inf")
        track_pan,             # FLOAT32 (0.0-1.0, center=0.5)
        track_pan_display      # STRING ("L50", "C", "R25")
    ]
)

# ============================================================================
# TRACK NAVIGATION MESSAGES (Legacy - DEPRECATED)
# ============================================================================
# These messages are deprecated. Use windowed versions below for large lists.

# Controller → Host: Request list of tracks in current context
# DEPRECATED: Use REQUEST_TRACK_LIST_WINDOW instead
REQUEST_TRACK_LIST = Message(
    description='[DEPRECATED] Request list of tracks in current context',
    fields=[]
)

# Host → Controller: Track list response with navigation context
# DEPRECATED: Use TRACK_LIST_WINDOW instead
TRACK_LIST = Message(
    description='[DEPRECATED] List of tracks in current context with navigation info',
    fields=[
        track_count,           # Total tracks in bank window (uint8)
        track_index,           # Currently selected track index (uint8)
        track_is_nested,       # Are we in a group? (bool)
        parent_group_name,     # Parent group name if nested (string)
        track_list             # Array[32] of TrackInfo
    ]
)

# ============================================================================
# TRACK NAVIGATION MESSAGES (Windowed - New)
# ============================================================================
# Windowed loading pattern for large track lists (>16 tracks)
# Pattern: REQUEST(startIndex) -> RESPONSE(total, start, current, items[16])
# - Firmware requests 16 items at a time starting from startIndex
# - Host responds with actual startIndex (clamped if out of range)
# - Firmware accumulates in local cache, prefetches when cursor approaches end
# - Selection always uses absolute index

# Controller → Host: Request windowed track list
REQUEST_TRACK_LIST_WINDOW = Message(
    description='Request track list starting at index (windowed, 16 items)',
    fields=[track_start_index]  # UINT8: start index for window
)

# Host → Controller: Windowed track list response
TRACK_LIST_WINDOW = Message(
    description='Windowed track list response (16 items max)',
    fields=[
        track_count,           # UINT8: total tracks (absolute)
        track_start_index,     # UINT8: actual start index (may be clamped)
        track_index,           # UINT8: currently selected track index
        track_is_nested,       # Are we in a group? (bool)
        parent_group_name,     # Parent group name if nested (string) - sent on every window
        track_list_window      # Array[16] of TrackInfo: this window's tracks
    ]
)

# Controller → Host: Select track by index
TRACK_SELECT_BY_INDEX = Message(
    description='Select track by index in current context',
    fields=[
        track_index            # Track index (uint8)
    ]
)

# Controller → Host: Enter track group (navigate into group's children)
ENTER_TRACK_GROUP = Message(
    description='Navigate into a track group to see its children',
    fields=[
        track_index            # Group track index (uint8)
    ]
)

# Controller → Host: Exit track group (navigate back to parent)
EXIT_TRACK_GROUP = Message(
    description='Navigate back to parent track context',
    fields=[]
)

# Bidirectional: Toggle track mute (Controller → Host) or notify state (Host → Controller)
TRACK_MUTE = Message(
    description='Toggle track mute state',
    fields=[
        track_index,           # Track index (uint8)
        track_is_mute          # Mute state (bool)
    ]
)

# Bidirectional: Toggle track solo (Controller → Host) or notify state (Host → Controller)
TRACK_SOLO = Message(
    description='Toggle track solo state',
    fields=[
        track_index,           # Track index (uint8)
        track_is_solo          # Solo state (bool)
    ]
)

# Controller → Host: Toggle track activated state
TRACK_ACTIVATE = Message(
    description='Toggle track activated/deactivated state',
    fields=[
        track_index            # Track index (uint8)
    ]
)

# ============================================================================
# TRACK CHANNEL PARAMETER MESSAGES (Granular - Frequent Updates)
# ============================================================================
# Aligned with Bitwig API Channel.java naming
# These messages are for individual parameter updates (like remote control granular messages)

# Bidirectional: Track volume change with echo flag for optimistic updates
TRACK_VOLUME_CHANGE = Message(
    description='Track volume change (bidirectional with echo)',
    fields=[
        track_index,           # Track index (uint8)
        track_volume,          # Volume value (float32, 0.0-1.0)
        track_volume_display,  # Display value (string, "0 dB", "-inf")
        parameter_is_echo      # Echo flag for optimistic updates (bool)
    ]
)

# Bidirectional: Track pan change with echo flag for optimistic updates
TRACK_PAN_CHANGE = Message(
    description='Track pan change (bidirectional with echo)',
    fields=[
        track_index,           # Track index (uint8)
        track_pan,             # Pan value (float32, 0.0-1.0, center=0.5)
        track_pan_display,     # Display value (string, "L50", "C", "R25")
        parameter_is_echo      # Echo flag for optimistic updates (bool)
    ]
)

# Bidirectional: Track record arm toggle
TRACK_ARM_CHANGE = Message(
    description='Track record arm state change (bidirectional)',
    fields=[
        track_index,           # Track index (uint8)
        track_is_arm           # Arm state (bool)
    ]
)

# Host → Controller: Track muted by solo state (read-only notification)
TRACK_MUTED_BY_SOLO_CHANGE = Message(
    description='Track muted by solo state changed (Host → Controller only)',
    fields=[
        track_index,           # Track index (uint8)
        track_is_muted_by_solo # Muted by solo state (bool)
    ]
)

# ============================================================================
# TRACK PARAMETER AUTOMATION MESSAGES (Granular)
# ============================================================================
# Touch for automation recording, hasAutomation/modulatedValue for display

# Bidirectional: Touch automation for track volume
TRACK_VOLUME_TOUCH = Message(
    description='Touch automation start/stop for track volume',
    fields=[
        track_index,           # Track index (uint8)
        parameter_touched      # Touch state (bool)
    ]
)

# Bidirectional: Touch automation for track pan
TRACK_PAN_TOUCH = Message(
    description='Touch automation start/stop for track pan',
    fields=[
        track_index,           # Track index (uint8)
        parameter_touched      # Touch state (bool)
    ]
)

# Host → Controller: Track volume has automation state changed
TRACK_VOLUME_HAS_AUTOMATION_CHANGE = Message(
    description='Track volume hasAutomation() state changed',
    fields=[
        track_index,           # Track index (uint8)
        parameter_has_automation  # Has automation (bool)
    ]
)

# Host → Controller: Track pan has automation state changed
TRACK_PAN_HAS_AUTOMATION_CHANGE = Message(
    description='Track pan hasAutomation() state changed',
    fields=[
        track_index,           # Track index (uint8)
        parameter_has_automation  # Has automation (bool)
    ]
)

# Host → Controller: Track volume modulated value changed
TRACK_VOLUME_MODULATED_VALUE_CHANGE = Message(
    description='Track volume modulatedValue() changed (automation/modulation applied)',
    fields=[
        track_index,           # Track index (uint8)
        parameter_modulated_value  # Modulated value (float32)
    ]
)

# Host → Controller: Track pan modulated value changed
TRACK_PAN_MODULATED_VALUE_CHANGE = Message(
    description='Track pan modulatedValue() changed (automation/modulation applied)',
    fields=[
        track_index,           # Track index (uint8)
        parameter_modulated_value  # Modulated value (float32)
    ]
)

# ============================================================================
# TRACK SEND MESSAGES (Composite + Granular)
# ============================================================================
# Aligned with Bitwig API Send.java (extends Parameter)
# Each track can send audio to effect return tracks

# Controller → Host: Request sends list for current track
REQUEST_TRACK_SEND_LIST = Message(
    description='Request list of sends for current track',
    fields=[]
)

# Host → Controller: Send list for current track (composite)
TRACK_SEND_LIST = Message(
    description='List of sends for current track',
    fields=[
        track_index,           # Track index (uint8)
        send_count,            # Total sends available (uint8)
        send_list              # Array[8] of SendInfo
    ]
)

# Bidirectional: Send value change with echo flag for optimistic updates
TRACK_SEND_VALUE_CHANGE = Message(
    description='Track send value change (bidirectional with echo)',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        send_value,            # Send value (float32, 0.0-1.0)
        send_display_value,    # Display value (string, "0 dB", "-inf")
        parameter_is_echo      # Echo flag for optimistic updates (bool)
    ]
)

# Bidirectional: Send enabled state change
TRACK_SEND_ENABLED_CHANGE = Message(
    description='Track send enabled state change (bidirectional)',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        send_is_enabled        # Enabled state (bool)
    ]
)

# Bidirectional: Send mode change (AUTO, PRE, POST)
TRACK_SEND_MODE_CHANGE = Message(
    description='Track send mode change (bidirectional)',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        send_mode              # Send mode (string: "AUTO", "PRE", "POST")
    ]
)

# Host → Controller: Send pre-fader state changed (read-only, derived from mode)
TRACK_SEND_PRE_FADER_CHANGE = Message(
    description='Track send pre-fader state changed (Host → Controller only)',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        send_is_pre_fader      # Pre-fader state (bool)
    ]
)

# Bidirectional: Touch automation for send
TRACK_SEND_TOUCH = Message(
    description='Touch automation start/stop for track send',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        parameter_touched      # Touch state (bool)
    ]
)

# Host → Controller: Send has automation state changed
TRACK_SEND_HAS_AUTOMATION_CHANGE = Message(
    description='Track send hasAutomation() state changed',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        parameter_has_automation  # Has automation (bool)
    ]
)

# Host → Controller: Send modulated value changed
TRACK_SEND_MODULATED_VALUE_CHANGE = Message(
    description='Track send modulatedValue() changed (automation/modulation applied)',
    fields=[
        track_index,           # Track index (uint8)
        send_index,            # Send index (uint8)
        parameter_modulated_value  # Modulated value (float32)
    ]
)

# ============================================================================
# MIX VIEW SEND SELECTION MESSAGES
# ============================================================================
# These messages support the MixView feature where 4 tracks share a single
# selected send that can be controlled. The controller selects which send
# to observe/control across all 4 tracks.

# Controller → Host: Select which send to observe for MixView
# Send index 0-7 to select a send, or 255 for none
SELECT_MIX_SEND = Message(
    description='Select which send to observe for MixView (Controller → Host)',
    fields=[
        send_index             # UINT8 (0-7, or 255 for none)
    ]
)

# Controller → Host: Request list of send destination names (effect tracks)
REQUEST_SEND_DESTINATIONS = Message(
    description='Request list of send destination names (Controller → Host)',
    fields=[]
)

# Host → Controller: List of send destination names
SEND_DESTINATIONS_LIST = Message(
    description='List of send destination names (effect track names)',
    fields=[
        send_count,            # UINT8 - number of send destinations available
        send_destination_list  # Array[8] of {sendIndex, sendDestinationName}
    ]
)
