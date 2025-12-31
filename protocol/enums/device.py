# ============================================================================
# DEVICE ENUMS
# ============================================================================
# Enum definitions for device-related types.

from protocol_codegen.core.enum_def import EnumDef

# DeviceType: Type of device in the DAW
DeviceType = EnumDef(
    name="DeviceType",
    description="Type of device in the DAW",
    values={
        "UNKNOWN": 0,
        "AUDIO_EFFECT": 1,
        "INSTRUMENT": 2,
        "NOTE_EFFECT": 3,
    },
    string_mapping={
        "audio-effect": "AUDIO_EFFECT",
        "instrument": "INSTRUMENT",
        "note-effect": "NOTE_EFFECT",
    },
)

# ParameterType: UI hint for parameter rendering
# Determined by discreteCount and value names
# - KNOB: continuous parameter or unrecognized discrete
# - BUTTON: discreteCount == 2 AND values are On/Off
# - LIST: discreteCount > 2
ParameterType = EnumDef(
    name="ParameterType",
    description="UI hint for parameter rendering",
    values={
        "KNOB": 0,
        "BUTTON": 1,
        "LIST": 2,
    }
)
