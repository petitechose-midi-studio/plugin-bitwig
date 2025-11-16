from protocol import PrimitiveField, CompositeField, Type
from .color import color_rgb

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

# Navigation context
track_is_nested = PrimitiveField('isNested', type_name=Type.BOOL)
parent_group_name = PrimitiveField('parentGroupName', type_name=Type.STRING)

# ============================================================================
# COMPOSITE STRUCTS FOR TRACK NAVIGATION
# ============================================================================

# TrackInfo: Information about a track in the list
track_info = [
    track_index,           # Position in bank (uint8)
    track_name,            # Track name (string)
    track_color,           # Track color as RGB hex (uint32)
    track_is_activated,    # Track activated/deactivated (bool)
    track_is_mute,         # Mute state (bool)
    track_is_solo,         # Solo state (bool)
    track_is_group         # Is this track a group? (bool)
]

# Array of tracks (max 32 in bank window)
# Memory impact: 32 tracks * ~25 bytes = 800 bytes (acceptable for Teensy)
track_list = CompositeField('tracks', fields=track_info, array=32)
