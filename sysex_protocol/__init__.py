"""
Bitwig Studio SysEx Protocol Definitions

This package contains the Bitwig-specific SysEx protocol definitions:
- builtin_types.yaml: Built-in primitive types
- types.yaml: Bitwig-specific atomic types
- sysex_messages.py: Bitwig message definitions

These definitions use generic tools from resource.code.protocol to create
a type-safe, auto-generated protocol system.
"""

from typing import List
from protocol_codegen.core.message import Message
from .sysex_messages import ALL_MESSAGES

# Re-annotate for type safety
ALL_MESSAGES: List[Message]

__all__ = [
    'ALL_MESSAGES',
]
