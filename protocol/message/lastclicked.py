"""
Last Clicked Parameter Messages

Control the last clicked parameter in Bitwig via OPT encoder.
Messages separated by direction:
- COMMANDS (Controller → Host): Set value, touch
- NOTIFICATIONS (Host → Controller): Full update, value state

Flow:
1. User clicks a parameter in Bitwig
2. Host → Controller: LAST_CLICKED_UPDATE (full parameter info)
3. Controller configures OPT encoder (continuous/discrete, position)
4. User turns OPT encoder
5. Controller → Host: LAST_CLICKED_VALUE (set new value)
6. Host → Controller: LAST_CLICKED_VALUE_STATE (confirmation with displayValue)
"""

from field.parameter import *
from protocol_codegen.core.enums import Direction, Intent
from protocol_codegen.core.message import Message


# ============================================================================
# COMMANDS (Controller → Host)
# ============================================================================

LAST_CLICKED_VALUE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set last clicked parameter value',
    fields=[parameter_value]  # No displayValue, no isEcho - direction is implicit
)

LAST_CLICKED_TOUCH = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Touch automation for last clicked parameter',
    fields=[parameter_touched]
)


# ============================================================================
# NOTIFICATIONS (Host → Controller)
# ============================================================================

LAST_CLICKED_UPDATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Last clicked parameter update - sent when user clicks a new parameter',
    fields=[
        parameter_name,                 # STRING (max 16 chars)
        parameter_value,                # FLOAT32 (normalized 0.0-1.0)
        parameter_display_value,        # STRING (formatted: "50%", "-12dB", etc.)
        parameter_origin,               # FLOAT32 (0.0 or 0.5 for centered)
        parameter_exists,               # BOOL (false if no parameter clicked)
        parameter_type,                 # UINT8 (0=Knob, 1=Button, 2=List)
        parameter_discrete_count,       # INT16 (-1=continuous, 2=button, N=list)
        parameter_current_value_index   # UINT8 (current index for encoder navigation)
    ]
)

LAST_CLICKED_VALUE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Last clicked parameter value state (confirmation after change)',
    fields=[
        parameter_value,                # FLOAT32 (normalized 0.0-1.0)
        parameter_display_value         # STRING (formatted value from Bitwig)
    ]
)
