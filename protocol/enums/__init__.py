# ============================================================================
# ENUM DEFINITIONS
# ============================================================================
# Centralized enum definitions for protocol messages.
# These enums are the single source of truth for C++ and Java code generation.
#
# Each enum is defined using EnumDef from protocol_codegen.core.enum_def.
# The codegen generates:
# - C++: enum class in Protocol namespace (e.g., Protocol::TrackType)
# - Java: enum with fromValue() and optional fromBitwigString() methods
#
# See individual modules for specific enum definitions:
# - track.py: TrackType
# - device.py: DeviceType, ParameterType
# - navigation.py: ChildType, ViewType

from enums.track import TrackType
from enums.device import DeviceType, ParameterType
from enums.navigation import ChildType, ViewType

__all__ = [
    'TrackType',
    'DeviceType',
    'ParameterType',
    'ChildType',
    'ViewType',
]
