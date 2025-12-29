"""
Device and Remote Controls Messages

Separated into explicit directions:
- COMMANDS (Controller → Host): User actions
- QUERIES (Controller → Host): Request data with expected RESPONSE
- RESPONSES (Host → Controller): Data responses to queries
- NOTIFICATIONS (Host → Controller): State updates

DEVICE MESSAGES:
- DEVICE_SELECT: Select device by index (command)
- DEVICE_STATE_CHANGE: Toggle device enabled (command)
- DEVICE_PAGE_SELECT: Select page by index (command)
- DEVICE_CHANGE_HEADER: Device context changed (notify)
- DEVICE_PAGE_CHANGE: Page changed with remote controls (notify)
- etc.

REMOTE CONTROL MESSAGES:
- REMOTE_CONTROL_VALUE: Set parameter value (command)
- REMOTE_CONTROL_VALUE_STATE: Value state feedback (notify)
- REMOTE_CONTROL_TOUCH: Touch automation (command)
- DEVICE_REMOTE_CONTROLS_BATCH: Batch update (notify)
- etc.

NAVIGATION MESSAGES:
- REQUEST_DEVICE_LIST_WINDOW: Request device list (query)
- DEVICE_LIST_WINDOW: Device list response (response)
- ENTER_DEVICE_CHILD: Navigate into child (command)
- EXIT_TO_PARENT: Navigate back (command)
- etc.
"""

from field.device import *
from protocol_codegen.core.enums import Direction, Intent
from protocol_codegen.core.message import Message


# ============================================================================
# COMMANDS (Controller → Host)
# ============================================================================

DEVICE_SELECT = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Select device by index in current chain',
    fields=[device_index]
)

DEVICE_STATE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Toggle device enabled/bypassed by index',
    fields=[device_index, device_state]
)

DEVICE_PAGE_SELECT = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Select device page by index (modulo pageCount)',
    fields=[device_page_index]
)

REMOTE_CONTROL_VALUE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set remote control value',
    fields=[remote_control_index, parameter_value]  # No displayValue, no isEcho
)

DEVICE_REMOTE_CONTROL_TOUCH = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Touch automation start/stop for remote control parameter',
    fields=[remote_control_index, parameter_touched]
)

DEVICE_REMOTE_CONTROL_RESTORE_AUTOMATION = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Controller requests host to restore automation playback for parameter',
    fields=[remote_control_index]
)

ENTER_DEVICE_CHILD = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Navigate into a child (slot/layer/drum pad)',
    fields=[device_index, child_type, child_index]
)

EXIT_TO_PARENT = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Navigate back to parent device chain',
    fields=[]
)

VIEW_STATE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Controller view state changed (view type or selector visibility)',
    fields=[view_type, selector_active]
)


# ============================================================================
# QUERIES (Controller → Host)
# ============================================================================

REQUEST_DEVICE_LIST_WINDOW = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request device list starting at index (windowed, 16 items)',
    fields=[device_start_index]
)

REQUEST_DEVICE_PAGE_NAMES_WINDOW = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request page names starting at index (windowed, 16 items)',
    fields=[device_page_start_index]
)

REQUEST_DEVICE_CHILDREN = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request children (slots/layers/drums) for device and type',
    fields=[device_index, child_type]
)


# ============================================================================
# RESPONSES (Host → Controller)
# ============================================================================

DEVICE_LIST_WINDOW = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='Windowed device list response (16 items max)',
    fields=[
        device_count,
        device_start_index,
        device_index,
        device_is_nested,
        parent_name,
        device_list_window
    ]
)

DEVICE_PAGE_NAMES_WINDOW = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='Windowed page names response (16 items max)',
    fields=[
        device_page_count,
        device_page_start_index,
        device_page_index,
        page_names_window
    ]
)

DEVICE_CHILDREN = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='List of children for a device of specific type',
    fields=[device_index, child_type, children_count, children_list]
)


# ============================================================================
# NOTIFICATIONS (Host → Controller)
# ============================================================================

# Device context notifications
DEVICE_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Complete device state (name, enabled, page, 8 remote controls)',
    fields=[device_track_name, device_name, device_state, page_info, remote_controls]
)

DEVICE_PAGE_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Page change with new remote controls set',
    fields=[page_info, remote_controls]
)

DEVICE_CHANGE_HEADER = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Device change header - lightweight message with device identity only',
    fields=[device_name, device_state, device_type, page_info, children_types]
)

DEVICE_ENABLED_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Device enabled state changed notification',
    fields=[device_index, device_state]
)

# Remote control notifications
DEVICE_REMOTE_CONTROL_UPDATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Complete remote control update - sent individually per parameter',
    fields=[
        remote_control_index,
        parameter_name,
        parameter_value,
        parameter_display_value,
        parameter_origin,
        parameter_exists,
        parameter_type,
        parameter_discrete_count,
        parameter_current_value_index,
        parameter_has_automation,
        parameter_modulated_value
    ]
)

DEVICE_REMOTE_CONTROL_DISCRETE_VALUES = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Full list of discrete values for List parameters (lazy-loaded on demand)',
    fields=[remote_control_index, parameter_discrete_value_names, parameter_current_value_index]
)

REMOTE_CONTROL_VALUE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Remote control value state (confirmation with display value)',
    fields=[remote_control_index, parameter_value, parameter_display_value]
)

DEVICE_REMOTE_CONTROL_NAME_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Single remote control name change',
    fields=[remote_control_index, parameter_name]
)

DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='hasAutomation() state changed for remote control',
    fields=[remote_control_index, parameter_has_automation]
)

DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='isModulated state changed for remote control',
    fields=[remote_control_index, parameter_is_modulated]
)

DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='origin changed for remote control (bipolar center point)',
    fields=[remote_control_index, parameter_origin]
)

DEVICE_REMOTE_CONTROLS_BATCH = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Combined batch update of all 8 remote control values and modulated values (sent at fixed rate ~50Hz)',
    fields=[
        batch_sequence_number,
        values_dirty_mask,
        values_echo_mask,      # Keep echo mask - it's a bitmask, not the deprecated isEcho field
        has_automation_mask,
        parameter_values_batch,
        parameter_modulated_values_batch,
        parameter_display_values_batch
    ]
)


# ============================================================================
# DEPRECATED MESSAGES
# ============================================================================
# These messages are kept for backward compatibility but should not be used

REQUEST_DEVICE_LIST = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='[DEPRECATED] Request list of devices in current device chain',
    fields=[],
    deprecated=True
)

DEVICE_LIST = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='[DEPRECATED] List of devices in current chain with context',
    fields=[device_count, device_index, device_is_nested, parent_name, device_list],
    deprecated=True
)

REQUEST_DEVICE_PAGE_NAMES = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='[DEPRECATED] Request list of all available page names',
    fields=[],
    deprecated=True
)

DEVICE_PAGE_NAMES = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.RESPONSE,
    description='[DEPRECATED] List of all available page names',
    fields=[device_page_count, device_page_index, page_names],
    deprecated=True
)

# Legacy bidirectional message - replaced by REMOTE_CONTROL_VALUE + REMOTE_CONTROL_VALUE_STATE
DEVICE_REMOTE_CONTROL_VALUE_CHANGE = Message(
    direction=Direction.TO_HOST,  # Keep as command for compatibility
    intent=Intent.COMMAND,
    description='[DEPRECATED] Single remote control value update - use REMOTE_CONTROL_VALUE instead',
    fields=[remote_control_index, parameter_value, parameter_display_value, parameter_is_echo],
    deprecated=True
)
