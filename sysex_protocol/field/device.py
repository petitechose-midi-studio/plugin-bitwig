from protocol import PrimitiveField, CompositeField, Type
from .parameter import *  # Import generic parameter fields
from .color import color_rgb

# ============================================================================
# DEVICE-SPECIFIC FIELDS
# ============================================================================
# Fields that are specific to device macros (not generic to all parameters)

# Device macro index (0-7) - DEVICE SPECIFIC
macro_index = PrimitiveField('parameterIndex', type_name=Type.UINT8)

# Device context fields
device_state = PrimitiveField('isEnabled', type_name=Type.BOOL)
device_name = PrimitiveField('deviceName', type_name=Type.STRING)
device_track_name = PrimitiveField('deviceTrackName', type_name=Type.STRING)
device_track_color = color_rgb  # Track color as uint32 RGB hex (0xRRGGBB)
device_page_index = PrimitiveField('devicePageIndex', type_name=Type.UINT8)
device_page_count = PrimitiveField('devicePageCount', type_name=Type.UINT8)
device_page_name = PrimitiveField('devicePageName', type_name=Type.STRING)

# ============================================================================
# PARAMETER INFO (Composite Struct - Array[8])
# ============================================================================
# Device macro = index (device-specific) + parameter fields (generic)
#
# parameterType interpretation (from parameter.py):
# - 0: Knob (continuous or unrecognized discrete)
# - 1: Button (discreteCount == 2 AND values are On/Off)
# - 2: List (discreteCount > 2)

macro = [
    macro_index,                        # DEVICE SPECIFIC: Parameter index (0-7)
    parameter_value,                    # GENERIC: Normalized value (0.0-1.0)
    parameter_name,                     # GENERIC: Parameter name (max 16 chars)
    parameter_origin,                   # GENERIC: Origin value (0.0 or 0.5)
    parameter_exists,                   # GENERIC: Does parameter exist (true/false)
    parameter_discrete_count,           # GENERIC: Discrete value count (-1=continuous, 2=button, N=list)
    parameter_display_value,            # GENERIC: Formatted display value from Bitwig
    parameter_type,                     # GENERIC: Detected parameter type (0=Knob, 1=Button, 2=List)
    parameter_discrete_value_names,     # GENERIC: Array of discrete value names
    parameter_current_value_index       # GENERIC: Current index in discreteValueNames array
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


# Array of 8 parameters (maps to MACRO_1-8 encoders)
# Generated C++ type: etl::array<Macro, 8>
macros = CompositeField('macros', fields=macro, array=8)

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
page_names = PrimitiveField('pageNames', type_name=Type.STRING, array=32)

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

# ============================================================================
# COMPOSITE STRUCTS FOR DEVICE NAVIGATION
# ============================================================================

# DeviceInfo: Information about a device in the list
device_info = [
    device_index,          # Position in bank (uint8)
    device_name,           # Device name (string) - already defined above
    device_state,          # Enabled/disabled state (bool)
    children_types         # Array of child types (uint8[4])
]

# Array of devices (max 32 in bank window)
# Memory impact: 32 devices * ~25 bytes = 800 bytes (acceptable for Teensy)
device_list = CompositeField('devices', fields=device_info, array=32)

# ChildInfo: Information about a child (slot/layer/pad)
child_info = [
    child_index,           # Index (0-N for slots/layers, MIDI note 0-127 for drums)
    child_name             # Name (with notation for drums: "Kick (C1)")
]

# Array of children (max 32: layers, slots, or drum pads)
# Memory impact: 32 children * ~20 bytes = 640 bytes (acceptable for Teensy)
children_list = CompositeField('children', fields=child_info, array=32)
