from protocol_codegen.core.field import PrimitiveField, CompositeField, Type
from field.parameter import *  # Import generic parameter fields
from field.color import color_rgb

# ============================================================================
# DEVICE REMOTE CONTROLS FIELDS
# ============================================================================
# Fields that are specific to device remote controls (not generic to all parameters)

# Remote control index (0-7) - identifies which remote control slot
remote_control_index = PrimitiveField('remoteControlIndex', type_name=Type.UINT8)

# Device context fields
device_state = PrimitiveField('isEnabled', type_name=Type.BOOL)
device_name = PrimitiveField('deviceName', type_name=Type.STRING)
device_type = PrimitiveField('deviceType', type_name=Type.UINT8)
device_track_name = PrimitiveField('deviceTrackName', type_name=Type.STRING)
device_track_color = color_rgb  # Track color as uint32 RGB hex (0xRRGGBB)
device_page_index = PrimitiveField('devicePageIndex', type_name=Type.UINT8)
device_page_count = PrimitiveField('devicePageCount', type_name=Type.UINT8)
device_page_name = PrimitiveField('devicePageName', type_name=Type.STRING)

# ============================================================================
# REMOTE CONTROL INFO (Composite Struct - Array[8])
# ============================================================================
# RemoteControl = index (0-7) + parameter fields (generic)
# Aligned with Bitwig API: RemoteControl extends Parameter
#
# parameterType interpretation (from parameter.py):
# - 0: Knob (continuous or unrecognized discrete)
# - 1: Button (discreteCount == 2 AND values are On/Off)
# - 2: List (discreteCount > 2)

remote_control = [
    remote_control_index,               # Remote control slot index (0-7)
    parameter_value,                    # GENERIC: Normalized value (0.0-1.0)
    parameter_name,                     # GENERIC: Parameter name (max 16 chars)
    parameter_origin,                   # GENERIC: Origin value (0.0 or 0.5)
    parameter_exists,                   # GENERIC: Does parameter exist (true/false)
    parameter_discrete_count,           # GENERIC: Discrete value count (-1=continuous, 2=button, N=list)
    parameter_display_value,            # GENERIC: Formatted display value from Bitwig
    parameter_type,                     # GENERIC: Detected parameter type (0=Knob, 1=Button, 2=List)
    parameter_discrete_value_names,     # GENERIC: Array of discrete value names
    parameter_current_value_index,      # GENERIC: Current index in discreteValueNames array
    parameter_has_automation,           # GENERIC: Has automation data (from Bitwig API)
    parameter_modulated_value,          # GENERIC: Value after automation/modulation applied
    parameter_is_modulated              # GENERIC: Has modulation source (for ribbon display)
]

# ============================================================================
# PAGE INFO (Composite Struct - Single instance)
# ============================================================================
# Current remote control page information
# Used in: DEVICE_CHANGE, DEVICE_PAGE_CHANGE

page_info = CompositeField('pageInfo', fields=[
    device_page_index,      # Current page (0-N)
    device_page_count,      # Current page (0-N)
    device_page_name,      # Page name (max 16 chars)
])


# Array of 8 remote controls (maps to encoders 1-8)
# Aligned with Bitwig API: RemoteControlsPage contains N RemoteControl parameters
# Generated C++ type: etl::array<RemoteControl, 8>
remote_controls = CompositeField('remoteControls', fields=remote_control, array=8)

# ============================================================================
# PAGE NAVIGATION FIELDS
# ============================================================================
# For page list and navigation

# Single page name in list (used in array)
page_name = PrimitiveField('pageName', type_name=Type.STRING)

# Array of all available page names (max 32 pages)
# Dynamic size: encodes actual count first, then names
# Generated C++ type: etl::array<etl::string<16>, 32>
# Generated Java type: List<String>
# Memory impact: 32 * 17 bytes = 544 bytes (acceptable for Teensy)
# DEPRECATED: Use page_names_window instead for windowed loading
page_names = PrimitiveField('pageNames', type_name=Type.STRING, array=32)

# ============================================================================
# WINDOWED PAGE NAVIGATION FIELDS (New)
# ============================================================================
# For lazy-loading large page lists (>16 pages)
# Pattern: REQUEST_XXX_WINDOW(startIndex) -> XXX_WINDOW(total, start, current, items[16])

# Start index for windowed requests
device_page_start_index = PrimitiveField('pageStartIndex', type_name=Type.UINT8)

# Array of 16 page names (one window)
# Memory impact: 16 * 17 bytes = 272 bytes per message
page_names_window = PrimitiveField('pageNames', type_name=Type.STRING, array=16)

# ============================================================================
# DEVICE NAVIGATION FIELDS (Hierarchical Navigation)
# ============================================================================

# Device/child index in current context
device_index = PrimitiveField('deviceIndex', type_name=Type.UINT8)
child_index = PrimitiveField('childIndex', type_name=Type.UINT8)  # 0-127 (MIDI note for drums)

# Counts
device_count = PrimitiveField('deviceCount', type_name=Type.UINT8)
children_count = PrimitiveField('childrenCount', type_name=Type.UINT8)

# Navigation context
device_is_nested = PrimitiveField('isNested', type_name=Type.BOOL)
parent_name = PrimitiveField('parentName', type_name=Type.STRING)

# Child type enum: 1=Slot, 2=Layer, 3=DrumPad
child_type = PrimitiveField('childType', type_name=Type.UINT8)

# Array of available child types (max 4: S, L, D, + 1 reserved)
# Example: [1, 2, 0, 0] = has Slots AND Layers
children_types = PrimitiveField('childrenTypes', type_name=Type.UINT8, array=4)

# Child name (includes notation for drums: "Kick (C1)")
child_name = PrimitiveField('childName', type_name=Type.STRING)

# Item type for children (1=Slot, 2=Layer, 3=DrumPad)
child_item_type = PrimitiveField('itemType', type_name=Type.UINT8)

# ============================================================================
# COMPOSITE STRUCTS FOR DEVICE NAVIGATION
# ============================================================================

# DeviceInfo: Information about a device in the list
device_info = [
    device_index,          # Position in bank (uint8)
    device_name,           # Device name (string) - already defined above
    device_state,          # Enabled/disabled state (bool)
    device_type,           # Device Type (FX, Instrument, VST)
    children_types         # Array of child types (uint8[4])
]

# Array of devices (max 32 in bank window)
# Memory impact: 32 devices * ~25 bytes = 800 bytes (acceptable for Teensy)
# DEPRECATED: Use device_list_window instead for windowed loading
device_list = CompositeField('devices', fields=device_info, array=32)

# ============================================================================
# WINDOWED DEVICE LIST FIELDS (New)
# ============================================================================
# For lazy-loading large device lists (>16 devices)
# Pattern: REQUEST_XXX_WINDOW(startIndex) -> XXX_WINDOW(total, start, current, items[16])

# Start index for windowed requests
device_start_index = PrimitiveField('deviceStartIndex', type_name=Type.UINT8)

# Array of 16 devices (one window)
# Memory impact: 16 devices * ~25 bytes = 400 bytes per message
device_list_window = CompositeField('devices', fields=device_info, array=16)

# ChildInfo: Information about a child (slot/layer/pad) with type
child_info = [
    child_index,           # Index (0-N for slots/layers, MIDI note 0-127 for drums)
    child_name,            # Name (with notation for drums: "Kick (C1)")
    child_item_type        # 1=Slot, 2=Layer, 3=DrumPad
]

# Array of children (max 16: flat list of slots + layers + drums)
# Memory impact: 16 children * ~20 bytes = 320 bytes (acceptable for Teensy)
children_list = CompositeField('children', fields=child_info, array=16)

# ============================================================================
# VIEW STATE FIELDS (Controller → Host)
# ============================================================================
# Used to inform host about controller's active view state
# Host uses this to control batch sends (only send when RemoteControls visible)

# View type enum: 0=REMOTE_CONTROLS, 1=MIX, 2=CLIP
view_type = PrimitiveField('viewType', type_name=Type.UINT8)

# Whether a selector/overlay is currently active (device selector, page selector, etc.)
selector_active = PrimitiveField('selectorActive', type_name=Type.BOOL)
