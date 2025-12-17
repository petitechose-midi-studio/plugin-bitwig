from protocol_codegen.core.field import PrimitiveField, CompositeField, Type
from field.color import color_rgb
from field.parameter import parameter_has_automation, parameter_modulated_value, parameter_touched, parameter_is_echo

# ============================================================================
# TRACK FIELDS
# ============================================================================

# Track index and counts
track_index = PrimitiveField('trackIndex', type_name=Type.UINT8)
track_count = PrimitiveField('trackCount', type_name=Type.UINT8)

# Track properties
track_name = PrimitiveField('trackName', type_name=Type.STRING)
track_color = color_rgb  # Track color as uint32 RGB hex (0xRRGGBB)
track_is_activated = PrimitiveField('isActivated', type_name=Type.BOOL)
track_is_mute = PrimitiveField('isMute', type_name=Type.BOOL)
track_is_solo = PrimitiveField('isSolo', type_name=Type.BOOL)
track_is_group = PrimitiveField('isGroup', type_name=Type.BOOL)
# Track type: 0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master
track_type = PrimitiveField('trackType', type_name=Type.UINT8)

# Navigation context
track_is_nested = PrimitiveField('isNested', type_name=Type.BOOL)
parent_group_name = PrimitiveField('parentGroupName', type_name=Type.STRING)

# ============================================================================
# TRACK CHANNEL PARAMETERS (from Channel.java)
# ============================================================================
# Aligned with Bitwig API Channel.java naming

# volume() - Parameter (normalized 0.0-1.0)
track_volume = PrimitiveField('volume', type_name=Type.FLOAT32)
track_volume_display = PrimitiveField('volumeDisplay', type_name=Type.STRING)  # "0 dB", "-inf", etc.

# pan() - Parameter (normalized 0.0-1.0, center=0.5)
track_pan = PrimitiveField('pan', type_name=Type.FLOAT32)
track_pan_display = PrimitiveField('panDisplay', type_name=Type.STRING)  # "L50", "C", "R25"

# arm() - SettableBooleanValue (record arm)
track_is_arm = PrimitiveField('isArm', type_name=Type.BOOL)

# isMutedBySolo() - BooleanValue (read-only, muted by another track's solo)
track_is_muted_by_solo = PrimitiveField('isMutedBySolo', type_name=Type.BOOL)

# ============================================================================
# TRACK VOLUME/PAN AUTOMATION FIELDS
# ============================================================================
# Uses generic parameter automation fields from field/parameter.py:
# - parameter_has_automation (hasAutomation)
# - parameter_modulated_value (modulatedValue)
# - parameter_touched (isTouched)
# - parameter_is_echo (isEcho)

# Parameter selector for volume/pan granular messages
# 0 = volume, 1 = pan
track_param_type = PrimitiveField('paramType', type_name=Type.UINT8)

# ============================================================================
# COMPOSITE STRUCTS FOR TRACK NAVIGATION
# ============================================================================

# TrackInfo: Information about a track in the list
# Includes channel parameters (volume, pan) for mixer display
track_info = [
    track_index,           # Position in bank (uint8)
    track_name,            # Track name (string)
    track_color,           # Track color as RGB hex (uint32)
    track_is_activated,    # Track activated/deactivated (bool)
    track_is_mute,         # Mute state (bool)
    track_is_solo,         # Solo state (bool)
    track_is_muted_by_solo,# Muted by another solo (bool, read-only)
    track_is_arm,          # Record arm state (bool)
    track_is_group,        # Is this track a group? (bool)
    track_type,            # Track type: 0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master
    track_volume,          # Volume (float32, 0.0-1.0)
    track_pan              # Pan (float32, 0.0-1.0, center=0.5)
]

# Array of tracks (max 32 in bank window)
# Memory impact: 32 tracks * ~40 bytes = 1280 bytes (acceptable for Teensy)
# DEPRECATED: Use track_list_window instead for windowed loading
track_list = CompositeField('tracks', fields=track_info, array=32)

# ============================================================================
# WINDOWED TRACK LIST FIELDS (New)
# ============================================================================
# For lazy-loading large track lists (>16 tracks)
# Pattern: REQUEST_XXX_WINDOW(startIndex) -> XXX_WINDOW(total, start, current, items[16])

# Start index for windowed requests
track_start_index = PrimitiveField('trackStartIndex', type_name=Type.UINT8)

# Array of 16 tracks (one window)
# Memory impact: 16 tracks * ~40 bytes = 640 bytes per message
track_list_window = CompositeField('tracks', fields=track_info, array=16)
