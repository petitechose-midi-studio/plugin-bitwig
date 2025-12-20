from field.device import *
from protocol_codegen.core.message import Message

# ============================================================================
# COMPOSITE MESSAGES (Rare - Full State)
# ============================================================================

DEVICE_CHANGE = Message(
    description='Complete device state (name, enabled, page, 8 remote controls)',
    fields=[
        device_track_name,  # STRING (max 16 chars)
        device_name,        # STRING (max 16 chars)
        device_state,       # BOOL
        page_info,          # PageInfo struct { uint8, string16, uint8 }
        remote_controls     # RemoteControl[8] array of structs
    ]
)

DEVICE_PAGE_CHANGE = Message(
    description='Page change with new remote controls set',
    fields=[page_info, remote_controls]
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
        device_type,          # BOOL
        page_info,             # PageInfo struct { uint8, uint8, string16 }
        children_types         # UINT8[4] - Child types (1=Slot, 2=Layer, 3=DrumPad)
    ]
)

DEVICE_REMOTE_CONTROL_UPDATE = Message(
    description='Complete remote control update - sent individually per parameter (8x after header)',
    fields=[
        remote_control_index,           # UINT8 (0-7) - remote control slot
        parameter_name,                 # STRING (max 16 chars) - generic
        parameter_value,                # FLOAT32 (normalized 0.0-1.0) - generic
        parameter_display_value,        # STRING (formatted: "50%", "Saw", etc.) - generic
        parameter_origin,               # FLOAT32 (0.0 or 0.5) - generic
        parameter_exists,               # BOOL - generic
        parameter_type,                 # UINT8 (0=Knob, 1=Button, 2=List) - generic
        parameter_discrete_count,       # INT16 (-1=continuous, 2=button, N=list) - generic
        parameter_current_value_index,  # UINT8 (current index for encoder navigation) - generic
        parameter_has_automation,       # BOOL - hasAutomation() from Bitwig API
        parameter_modulated_value       # FLOAT32 - modulatedValue() from Bitwig API
    ]
)

DEVICE_REMOTE_CONTROL_DISCRETE_VALUES = Message(
    description='Full list of discrete values for List parameters (lazy-loaded on demand)',
    fields=[
        remote_control_index,               # UINT8 (0-7) - remote control slot
        parameter_discrete_value_names,     # STRING[32] dynamic array - generic
        parameter_current_value_index       # UINT8 (current position) - generic
    ]
)

DEVICE_REMOTE_CONTROL_VALUE_CHANGE = Message(
    description='Single remote control value update with echo flag for optimistic updates',
    fields=[remote_control_index, parameter_value, parameter_display_value, parameter_is_echo]
)

DEVICE_REMOTE_CONTROL_NAME_CHANGE = Message(
    description='Single remote control name change',
    fields=[remote_control_index, parameter_name]
)

DEVICE_REMOTE_CONTROL_TOUCH = Message(
    description='Touch automation start/stop for remote control parameter',
    fields=[remote_control_index, parameter_touched]
)

# ============================================================================
# REMOTE CONTROL AUTOMATION MESSAGES (Granular - Host → Controller)
# ============================================================================
# Aligned with Bitwig API Parameter.java naming

DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE = Message(
    description='hasAutomation() state changed for remote control',
    fields=[remote_control_index, parameter_has_automation]
)

DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE = Message(
    description='isModulated state changed for remote control (modulation source added/removed)',
    fields=[remote_control_index, parameter_is_modulated]
)

DEVICE_REMOTE_CONTROLS_MODULATED_VALUES_BATCH = Message(
    description='Batch update of all 8 remote control modulated values (sent at fixed rate ~30Hz)',
    fields=[
        batch_sequence_number,           # UINT8: Rolling sequence (0-255) for staleness detection
        parameter_modulated_values_batch # NORM8[8]: Modulated values for all 8 remote controls
    ]
)

DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE = Message(
    description='origin changed for remote control (bipolar center point)',
    fields=[remote_control_index, parameter_origin]
)

DEVICE_STATE_CHANGE = Message(
    description='Toggle device enabled/bypassed by index',
    fields=[device_index, device_state]
)

# ============================================================================
# PAGE NAVIGATION MESSAGES (Legacy - DEPRECATED)
# ============================================================================
# These messages are deprecated. Use windowed versions below for large lists.

# Controller → Host: Request full list of page names
# DEPRECATED: Use REQUEST_DEVICE_PAGE_NAMES_WINDOW instead
REQUEST_DEVICE_PAGE_NAMES = Message(
    description='[DEPRECATED] Request list of all available page names',
    fields=[]  # Empty request message
)

# Controller → Host: Select page by index (with modulo wrap)
DEVICE_PAGE_SELECT_BY_INDEX = Message(
    description='Select device page by index (modulo pageCount)',
    fields=[device_page_index]
)

# Host → Controller: Full list of available pages
# DEPRECATED: Use DEVICE_PAGE_NAMES_WINDOW instead
DEVICE_PAGE_NAMES = Message(
    description='[DEPRECATED] List of all available page names',
    fields=[
        device_page_count,      # Total number of pages (uint8)
        device_page_index,      # Current selected page index (uint8)
        page_names              # Array of page names (String[32])
    ]
)

# ============================================================================
# PAGE NAVIGATION MESSAGES (Windowed - New)
# ============================================================================
# Windowed loading pattern for large page lists (>16 pages)
# Pattern: REQUEST(startIndex) -> RESPONSE(total, start, current, items[16])
# - Firmware requests 16 items at a time starting from startIndex
# - Host responds with actual startIndex (clamped if out of range)
# - Firmware accumulates in local cache, prefetches when cursor approaches end
# - Selection always uses absolute index

# Controller → Host: Request windowed page names
REQUEST_DEVICE_PAGE_NAMES_WINDOW = Message(
    description='Request page names starting at index (windowed, 16 items)',
    fields=[device_page_start_index]  # UINT8: start index for window
)

# Host → Controller: Windowed page names response
DEVICE_PAGE_NAMES_WINDOW = Message(
    description='Windowed page names response (16 items max)',
    fields=[
        device_page_count,        # UINT8: total pages (absolute)
        device_page_start_index,  # UINT8: actual start index (may be clamped)
        device_page_index,        # UINT8: currently selected page index
        page_names_window         # String[16]: this window's page names
    ]
)

# NOTE: DEVICE_PAGE_CHANGE already exists above and handles page change notification

# ============================================================================
# DEVICE NAVIGATION MESSAGES (Legacy - DEPRECATED)
# ============================================================================
# These messages are deprecated. Use windowed versions below for large lists.

# Controller → Host: Request list of devices in current chain context
# DEPRECATED: Use REQUEST_DEVICE_LIST_WINDOW instead
REQUEST_DEVICE_LIST = Message(
    description='[DEPRECATED] Request list of devices in current device chain',
    fields=[]
)

# Host → Controller: Device list response with navigation context
# DEPRECATED: Use DEVICE_LIST_WINDOW instead
DEVICE_LIST = Message(
    description='[DEPRECATED] List of devices in current chain with context',
    fields=[
        device_count,          # Total devices in bank window (uint8)
        device_index,          # Currently selected device index (uint8)
        device_is_nested,      # Are we in nested chain? (bool)
        parent_name,           # Parent name if nested (string)
        device_list            # Array[32] of DeviceInfo
    ]
)

# ============================================================================
# DEVICE NAVIGATION MESSAGES (Windowed - New)
# ============================================================================
# Windowed loading pattern for large device lists (>16 devices)
# Pattern: REQUEST(startIndex) -> RESPONSE(total, start, current, items[16])
# - Firmware requests 16 items at a time starting from startIndex
# - Host responds with actual startIndex (clamped if out of range)
# - Firmware accumulates in local cache, prefetches when cursor approaches end
# - Selection always uses absolute index

# Controller → Host: Request windowed device list
REQUEST_DEVICE_LIST_WINDOW = Message(
    description='Request device list starting at index (windowed, 16 items)',
    fields=[device_start_index]  # UINT8: start index for window
)

# Host → Controller: Windowed device list response
DEVICE_LIST_WINDOW = Message(
    description='Windowed device list response (16 items max)',
    fields=[
        device_count,          # UINT8: total devices (absolute)
        device_start_index,    # UINT8: actual start index (may be clamped)
        device_index,          # UINT8: currently selected device index
        device_is_nested,      # Are we in nested chain? (bool)
        parent_name,           # Parent name if nested (string) - sent on every window for simplicity
        device_list_window     # Array[16] of DeviceInfo: this window's devices
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

# ============================================================================
# VIEW STATE MESSAGES (Controller → Host)
# ============================================================================
# Controller informs host about active view state
# Host uses this to control batch sends (modulated values only needed for RemoteControls)

VIEW_STATE_CHANGE = Message(
    description='Controller view state changed (view type or selector visibility)',
    fields=[
        view_type,             # UINT8: 0=REMOTE_CONTROLS, 1=MIX, 2=CLIP
        selector_active        # BOOL: true if any selector/overlay is open
    ]
)