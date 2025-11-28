from field.device import *
from protocol_codegen.core.message import Message

# ============================================================================
# COMPOSITE MESSAGES (Rare - Full State)
# ============================================================================

DEVICE_CHANGE = Message(
    description='Complete device state (name, enabled, page, 8 parameters)',
    fields=[
        device_track_name,  # STRING (max 16 chars)
        device_name,        # STRING (max 16 chars)
        device_state,       # BOOL
        page_info,          # PageInfo struct { uint8, string16, uint8 }
        macros              # Parameter[8] array of structs
    ]
)

DEVICE_PAGE_CHANGE = Message(
    description='Page change with new parameter set',
    fields=[page_info,macros]
)

# ============================================================================
# GRANULAR MESSAGES (Frequent - Optimistic Updates)
# ============================================================================

# New optimized messages for fast device switching
DEVICE_CHANGE_HEADER = Message(
    description='Device change header - lightweight message with device identity only (sent first)',
    fields=[
        device_name,           # STRING (max 16 chars)
        device_state,          # BOOL
        page_info,             # PageInfo struct { uint8, uint8, string16 }
        children_types         # UINT8[4] - Child types (1=Slot, 2=Layer, 3=DrumPad)
    ]
)

DEVICE_MACRO_UPDATE = Message(
    description='Complete macro parameter update - sent individually per parameter (8x after header)',
    fields=[
        macro_index,                    # UINT8 (0-7) - device specific
        parameter_name,                 # STRING (max 16 chars) - generic
        parameter_value,                # FLOAT32 (normalized 0.0-1.0) - generic
        parameter_display_value,        # STRING (formatted: "50%", "Saw", etc.) - generic
        parameter_origin,               # FLOAT32 (0.0 or 0.5) - generic
        parameter_exists,               # BOOL - generic
        parameter_type,                 # UINT8 (0=Knob, 1=Button, 2=List) - generic
        parameter_discrete_count,       # INT16 (-1=continuous, 2=button, N=list) - generic
        parameter_current_value_index   # UINT8 (current index for encoder navigation) - generic
    ]
)

DEVICE_MACRO_DISCRETE_VALUES = Message(
    description='Full list of discrete values for List parameters (lazy-loaded on demand)',
    fields=[
        macro_index,                        # UINT8 (0-7) - device specific
        parameter_discrete_value_names,     # STRING[32] dynamic array - generic
        parameter_current_value_index       # UINT8 (current position) - generic
    ]
)

DEVICE_MACRO_VALUE_CHANGE = Message(
    description='Single parameter value update with echo flag for optimistic updates',
    fields=[macro_index, parameter_value, parameter_display_value, parameter_is_echo]
)

DEVICE_MACRO_NAME_CHANGE = Message(
    description='Single parameter name change',
    fields=[macro_index, parameter_name]
)

DEVICE_MACRO_TOUCH = Message(
    description='Touch automation start/stop for macro parameter',
    fields=[macro_index, parameter_touched]
)

DEVICE_STATE_CHANGE = Message(
    description='Toggle device enabled/bypassed by index',
    fields=[device_index, device_state]
)

# ============================================================================
# PAGE NAVIGATION MESSAGES (New)
# ============================================================================

# Controller → Host: Request full list of page names
REQUEST_DEVICE_PAGE_NAMES = Message(
    description='Request list of all available page names',
    fields=[]  # Empty request message
)

# Controller → Host: Select page by index (with modulo wrap)
DEVICE_PAGE_SELECT_BY_INDEX = Message(
    description='Select device page by index (modulo pageCount)',
    fields=[device_page_index]
)

# Host → Controller: Full list of available pages
DEVICE_PAGE_NAMES = Message(
    description='List of all available page names',
    fields=[
        device_page_count,      # Total number of pages (uint8)
        device_page_index,      # Current selected page index (uint8)
        page_names              # Array of page names (String[16])
    ]
)

# NOTE: DEVICE_PAGE_CHANGE already exists above and handles page change notification

# ============================================================================
# DEVICE NAVIGATION MESSAGES (Hierarchical Navigation)
# ============================================================================

# Controller → Host: Request list of devices in current chain context
REQUEST_DEVICE_LIST = Message(
    description='Request list of devices in current device chain',
    fields=[]
)

# Host → Controller: Device list response with navigation context
DEVICE_LIST = Message(
    description='List of devices in current chain with context',
    fields=[
        device_count,          # Total devices in bank window (uint8)
        device_index,          # Currently selected device index (uint8)
        device_is_nested,      # Are we in nested chain? (bool)
        parent_name,           # Parent name if nested (string)
        device_list            # Array[16] of DeviceInfo
    ]
)

# Controller → Host: Request children of a specific type
REQUEST_DEVICE_CHILDREN = Message(
    description='Request children (slots/layers/drums) for device and type',
    fields=[
        device_index,          # Device index (uint8)
        child_type             # Type: 1=Slot, 2=Layer, 3=DrumPad (uint8)
    ]
)

# Host → Controller: Children list response
DEVICE_CHILDREN = Message(
    description='List of children for a device of specific type',
    fields=[
        device_index,          # Parent device (uint8)
        child_type,            # Type: 1=Slot, 2=Layer, 3=DrumPad (uint8)
        children_count,        # Number of children (uint8)
        children_list          # Array[16] of ChildInfo
    ]
)

# Controller → Host: Navigate into a child
ENTER_DEVICE_CHILD = Message(
    description='Navigate into a child (slot/layer/drum pad)',
    fields=[
        device_index,          # Parent device index (uint8)
        child_type,            # Type: 1=Slot, 2=Layer, 3=DrumPad (uint8)
        child_index            # Child index (0-N) or MIDI note (0-127 for drums)
    ]
)

# Controller → Host: Navigate back to parent chain
EXIT_TO_PARENT = Message(
    description='Navigate back to parent device chain',
    fields=[]
)

# Controller → Host: Select device by index
DEVICE_SELECT_BY_INDEX = Message(
    description='Select device by index in current chain',
    fields=[
        device_index           # Device index (uint8)
    ]
)