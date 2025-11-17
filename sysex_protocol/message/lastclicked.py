from field.parameter import *
from protocol_codegen.core.message import Message

# ============================================================================
# LAST CLICKED PARAMETER MESSAGES
# ============================================================================
# Control the last clicked parameter in Bitwig via OPT encoder
#
# Flow:
# 1. User clicks a parameter in Bitwig
# 2. Host → Controller: LAST_CLICKED_UPDATE (full parameter info)
# 3. Controller configures OPT encoder (continuous/discrete, position)
# 4. User turns OPT encoder
# 5. Controller → Host: LAST_CLICKED_VALUE_CHANGE
# 6. Host updates parameter in Bitwig
# 7. Host → Controller: LAST_CLICKED_VALUE_CHANGE (echo with displayValue)

# Host → Controller: Full parameter info when new parameter is clicked
LAST_CLICKED_UPDATE = Message(
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

# Bidirectional: Value change for last clicked parameter
LAST_CLICKED_VALUE_CHANGE = Message(
    description='Last clicked parameter value change (bidirectional with echo flag)',
    fields=[
        parameter_value,                # FLOAT32 (normalized 0.0-1.0)
        parameter_display_value,        # STRING (formatted value from Bitwig)
        parameter_is_echo               # BOOL (true if echo from controller change)
    ]
)

# Controller → Host: Touch automation start/stop
LAST_CLICKED_TOUCH = Message(
    description='Touch automation for last clicked parameter',
    fields=[parameter_touched]
)
