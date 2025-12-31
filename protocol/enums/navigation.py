# ============================================================================
# NAVIGATION ENUMS
# ============================================================================
# Enum definitions for navigation-related types.

from protocol_codegen.core.enum_def import EnumDef

# ChildType: Type of device child (slot, layer, drum pad)
# These are bitflags that can be combined (e.g., SLOTS | LAYERS)
# Used in children_types array to indicate which child types are available
ChildType = EnumDef(
    name="ChildType",
    description="Type of device child (bitflags, combinable)",
    values={
        "NONE": 0,
        "SLOTS": 1,
        "LAYERS": 2,
        "DRUMS": 4,
    },
    is_bitflags=True  # Generate as constants, not enum class
)

# ViewType: Active view on the controller
# Determines which UI is displayed and which messages the host should send
ViewType = EnumDef(
    name="ViewType",
    description="Active view on the controller",
    values={
        "REMOTE_CONTROLS": 0,
        "MIX": 1,
        "CLIP": 2,
    }
)
