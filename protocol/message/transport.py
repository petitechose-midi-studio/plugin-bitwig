"""
Transport Control Messages

Bidirectional messages for transport state synchronization between
Bitwig and the hardware controller.

MESSAGES:
- TRANSPORT_PLAY: Play/pause state (bidirectional)
- TRANSPORT_RECORD: Record arm state (bidirectional)
"""

from field.transport import *
from protocol_codegen.core.message import Message


# ============================================================================
# Transport State (Bidirectional)
# ============================================================================

TRANSPORT_PLAY = Message(
    description='Transport play/pause state',
    fields=[transport_play]
)

TRANSPORT_RECORD = Message(
    description='Transport Record Request',
    fields=[transport_record]
)
TRANSPORT_STOP = Message(
    description='Transport Stop Request',
    fields=[transport_stop]
)
