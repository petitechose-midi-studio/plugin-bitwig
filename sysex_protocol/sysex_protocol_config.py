"""
Bitwig Plugin SysEx Configuration (Pure Python, Type-Safe)

Plugin-specific overrides for SysEx protocol constants.
Inherits from builtin defaults and overrides only what's needed.
"""

import sys
from pathlib import Path

# Add protocol to path for imports
protocol_path = Path(__file__).parent.parent.parent.parent / "resource" / "code" / "py"
sys.path.insert(0, str(protocol_path))

from protocol.sysex.sysex_config import SysExConfig
from protocol.sysex.sysex_builtin_config import BUILTIN_SYSEX_CONFIG


# Bitwig-specific configuration
# Inherits all defaults and overrides manufacturer_id
PLUGIN_SYSEX_CONFIG = SysExConfig(
    framing=BUILTIN_SYSEX_CONFIG.framing.model_copy(
        update={'manufacturer_id': 0x7F}  # Bitwig uses 0x7F
    ),
    structure=BUILTIN_SYSEX_CONFIG.structure,
    limits=BUILTIN_SYSEX_CONFIG.limits,
    message_id_ranges=BUILTIN_SYSEX_CONFIG.message_id_ranges,
    roles=BUILTIN_SYSEX_CONFIG.roles,
)
