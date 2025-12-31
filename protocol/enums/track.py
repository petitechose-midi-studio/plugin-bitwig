# ============================================================================
# TRACK ENUMS
# ============================================================================
# Enum definitions for track-related types.

from protocol_codegen.core.enum_def import EnumDef

# TrackType: Type of track in the DAW
TrackType = EnumDef(
    name="TrackType",
    description="Type of track in the DAW",
    values={
        "AUDIO": 0,
        "INSTRUMENT": 1,
        "HYBRID": 2,
        "GROUP": 3,
        "EFFECT": 4,
        "MASTER": 5,
    },
    string_mapping={
        "Audio": "AUDIO",
        "Instrument": "INSTRUMENT",
        "Hybrid": "HYBRID",
        "Group": "GROUP",
        "Effect": "EFFECT",
        "Master": "MASTER",
    },
)
