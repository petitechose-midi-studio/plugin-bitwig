"""
Track Messages

Separated into explicit directions:
- COMMANDS (Controller → Host): User actions
- QUERIES (Controller → Host): Request data with expected RESPONSE
- RESPONSES (Host → Controller): Data responses to queries
- NOTIFICATIONS (Host → Controller): State updates

TRACK MESSAGES:
- TRACK_SELECT: Select track by index (command)
- TRACK_MUTE/SOLO/ARM: Toggle track states (commands)
- TRACK_VOLUME/PAN: Set track parameters (commands)
- TRACK_CHANGE: Full track state (notify)
- TRACK_VOLUME_STATE/PAN_STATE: Parameter state feedback (notify)
- etc.

NAVIGATION MESSAGES:
- REQUEST_TRACK_LIST_WINDOW: Request track list (query)
- TRACK_LIST_WINDOW: Track list response (response)
- ENTER_TRACK_GROUP: Navigate into group (command)
- EXIT_TRACK_GROUP: Navigate back (command)

SEND MESSAGES:
- TRACK_SEND_VALUE: Set send value (command)
- TRACK_SEND_VALUE_STATE: Send value feedback (notify)
- etc.
"""

from field.track import *
from field.send import *
from protocol_codegen.core.enums import Direction, Intent
from protocol_codegen.core.message import Message


# ============================================================================
# COMMANDS (Controller → Host)
# ============================================================================

TRACK_SELECT = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Select track by index in current context',
    fields=[track_index]
)

ENTER_TRACK_GROUP = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Navigate into a track group to see its children',
    fields=[track_index]
)

EXIT_TRACK_GROUP = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Navigate back to parent track context',
    fields=[]
)

# Track state commands
TRACK_MUTE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track mute state',
    fields=[track_index, track_is_mute]
)

TRACK_SOLO = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track solo state',
    fields=[track_index, track_is_solo]
)

TRACK_ACTIVATE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Toggle track activated/deactivated state',
    fields=[track_index]
)

TRACK_ARM = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track record arm state',
    fields=[track_index, track_is_arm]
)

# Track parameter commands (no displayValue, no isEcho - direction is implicit)
TRACK_VOLUME = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track volume',
    fields=[track_index, track_volume]
)

TRACK_PAN = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track pan',
    fields=[track_index, track_pan]
)

# Touch automation commands
TRACK_VOLUME_TOUCH = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Touch automation start/stop for track volume',
    fields=[track_index, parameter_touched]
)

TRACK_PAN_TOUCH = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Touch automation start/stop for track pan',
    fields=[track_index, parameter_touched]
)

# Send commands
TRACK_SEND_VALUE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track send value',
    fields=[track_index, send_index, send_value]  # No displayValue, no isEcho
)

TRACK_SEND_ENABLED = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track send enabled state',
    fields=[track_index, send_index, send_is_enabled]
)

TRACK_SEND_MODE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set track send mode (AUTO, PRE, POST)',
    fields=[track_index, send_index, send_mode]
)

TRACK_SEND_TOUCH = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Touch automation start/stop for track send',
    fields=[track_index, send_index, parameter_touched]
)

# Mix view selection
SELECT_MIX_SEND = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Select which send to observe for MixView',
    fields=[send_index]
)


# ============================================================================
# QUERIES (Controller → Host)
# ============================================================================

REQUEST_TRACK_LIST_WINDOW = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request track list starting at index (windowed, 16 items)',
    fields=[track_start_index]
)

REQUEST_TRACK_SEND_LIST = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request list of sends for current track',
    fields=[]
)

REQUEST_SEND_DESTINATIONS = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request list of send destination names',
    fields=[]
)


# ============================================================================
# RESPONSES (Host → Controller)
# ============================================================================

TRACK_LIST_WINDOW = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='Windowed track list response (16 items max)',
    fields=[
        track_count,
        track_start_index,
        track_index,
        track_is_nested,
        parent_group_name,
        track_list_window
    ]
)

TRACK_SEND_LIST = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='List of sends for current track',
    fields=[track_index, send_count, send_list]
)

SEND_DESTINATIONS_LIST = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='List of send destination names (effect track names)',
    fields=[send_count, send_destination_list]
)


# ============================================================================
# NOTIFICATIONS (Host → Controller)
# ============================================================================

# Full track state
TRACK_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track context change notification with full channel state',
    fields=[
        track_name,
        track_color,
        track_index,
        track_type,
        track_is_activated,
        track_is_mute,
        track_is_solo,
        track_is_muted_by_solo,
        track_is_arm,
        track_volume,
        track_volume_display,
        track_pan,
        track_pan_display
    ]
)

# Track state notifications
TRACK_MUTE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track mute state changed',
    fields=[track_index, track_is_mute]
)

TRACK_SOLO_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track solo state changed',
    fields=[track_index, track_is_solo]
)

TRACK_ARM_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track record arm state changed',
    fields=[track_index, track_is_arm]
)

TRACK_MUTED_BY_SOLO_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track muted by solo state changed',
    fields=[track_index, track_is_muted_by_solo]
)

# Track parameter notifications (with displayValue for display)
TRACK_VOLUME_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track volume state',
    fields=[track_index, track_volume, track_volume_display]
)

TRACK_PAN_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track pan state',
    fields=[track_index, track_pan, track_pan_display]
)

# Track automation notifications
TRACK_VOLUME_HAS_AUTOMATION_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track volume hasAutomation() state changed',
    fields=[track_index, parameter_has_automation]
)

TRACK_PAN_HAS_AUTOMATION_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track pan hasAutomation() state changed',
    fields=[track_index, parameter_has_automation]
)

TRACK_VOLUME_MODULATED_VALUE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track volume modulatedValue() changed',
    fields=[track_index, parameter_modulated_value]
)

TRACK_PAN_MODULATED_VALUE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track pan modulatedValue() changed',
    fields=[track_index, parameter_modulated_value]
)

# Send notifications
TRACK_SEND_VALUE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track send value state',
    fields=[track_index, send_index, send_value, send_display_value]
)

TRACK_SEND_ENABLED_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track send enabled state changed',
    fields=[track_index, send_index, send_is_enabled]
)

TRACK_SEND_MODE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track send mode changed',
    fields=[track_index, send_index, send_mode]
)

TRACK_SEND_PRE_FADER_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track send pre-fader state changed',
    fields=[track_index, send_index, send_is_pre_fader]
)

TRACK_SEND_HAS_AUTOMATION_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track send hasAutomation() state changed',
    fields=[track_index, send_index, parameter_has_automation]
)

TRACK_SEND_MODULATED_VALUE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Track send modulatedValue() changed',
    fields=[track_index, send_index, parameter_modulated_value]
)
