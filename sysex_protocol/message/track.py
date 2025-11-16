from protocol import Message
from ..field.track import *

# ============================================================================
# TRACK CHANGE NOTIFICATION
# ============================================================================

# Host → Controller: Track context changed (cursor track selection)
TRACK_CHANGE = Message(
    description='Track context change notification',
    fields=[
        track_name,            # STRING (max 16 chars)
        track_color,           # UINT32 (0xRRGGBB)
        track_index            # UINT8 (current track position)
    ]
)

# ============================================================================
# TRACK NAVIGATION MESSAGES (Hierarchical Navigation)
# ============================================================================

# Controller → Host: Request list of tracks in current context
REQUEST_TRACK_LIST = Message(
    description='Request list of tracks in current context',
    fields=[]
)

# Host → Controller: Track list response with navigation context
TRACK_LIST = Message(
    description='List of tracks in current context with navigation info',
    fields=[
        track_count,           # Total tracks in bank window (uint8)
        track_index,           # Currently selected track index (uint8)
        track_is_nested,       # Are we in a group? (bool)
        parent_group_name,     # Parent group name if nested (string)
        track_list             # Array[32] of TrackInfo
    ]
)

# Controller → Host: Select track by index
TRACK_SELECT_BY_INDEX = Message(
    description='Select track by index in current context',
    fields=[
        track_index            # Track index (uint8)
    ]
)

# Controller → Host: Enter track group (navigate into group's children)
ENTER_TRACK_GROUP = Message(
    description='Navigate into a track group to see its children',
    fields=[
        track_index            # Group track index (uint8)
    ]
)

# Controller → Host: Exit track group (navigate back to parent)
EXIT_TRACK_GROUP = Message(
    description='Navigate back to parent track context',
    fields=[]
)

# Bidirectional: Toggle track mute (Controller → Host) or notify state (Host → Controller)
TRACK_MUTE = Message(
    description='Toggle track mute state',
    fields=[
        track_index,           # Track index (uint8)
        track_is_mute          # Mute state (bool)
    ]
)

# Bidirectional: Toggle track solo (Controller → Host) or notify state (Host → Controller)
TRACK_SOLO = Message(
    description='Toggle track solo state',
    fields=[
        track_index,           # Track index (uint8)
        track_is_solo          # Solo state (bool)
    ]
)

# Controller → Host: Toggle track activated state
TRACK_ACTIVATE = Message(
    description='Toggle track activated/deactivated state',
    fields=[
        track_index            # Track index (uint8)
    ]
)
