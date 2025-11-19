"""
Bitwig SysEx Messages - Entry Point

This module serves as the entry point for message definitions.
All messages are organized by category in the message/ subdirectory
and are automatically discovered and loaded.

ARCHITECTURE
------------
- Message categories: message/transport.py, message/device.py, etc.
- Auto-discovery: All .py files in message/ are automatically imported
- Protocol codegen: Uses protocol-codegen package for type-safe generation

ADDING NEW MESSAGES
-------------------
1. Create or edit a category file (e.g., message/transport.py)
2. Define messages using Message() with PrimitiveField/CompositeField
3. Add to ALL_MESSAGES list
4. Run: ./script/bash/protocol/generate_protocol.sh

CATEGORIES
----------
Currently implemented:
- transport: Transport control (play, record, tempo)
- device: Device selection and parameter control
- track: Track navigation and control
- system: System messages (handshake, status)

MESSAGE STRUCTURE
-----------------
Each message has:
- description: Human-readable description
- fields: List of PrimitiveField() or CompositeField() objects

Example:
    TransportPlay = Message(
        description="Transport play/pause state",
        fields=[PrimitiveField('isPlaying', type_name='uint8')]
    )
    TransportPlay.name = 'TRANSPORT_PLAY'

MESSAGE ID ALLOCATION
---------------------
Message IDs are auto-allocated sequentially starting from 0x00.
No manual ID management required.
"""

# Import ALL_MESSAGES from local message/ directory (auto-discovery already executed)
from .message import ALL_MESSAGES  # noqa: F401 - Exported for protocol generator

# That's it! ALL_MESSAGES is populated with messages from all categories.
# The protocol generator imports this file and uses ALL_MESSAGES for code generation.

# Export ALL_MESSAGES for external use
__all__ = ['ALL_MESSAGES']
