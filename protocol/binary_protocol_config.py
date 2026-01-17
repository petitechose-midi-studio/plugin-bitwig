"""
Bitwig Plugin Binary Protocol Configuration

Plugin-specific configuration for Binary (8-bit) protocol.
Used with oc-bridge for USB Serial communication.
"""

from protocol_codegen.generators.orchestrators.binary import (
    BinaryConfig,
    BinaryLimits,
    BinaryStructure,
)


# Bitwig-specific Binary configuration
PROTOCOL_CONFIG = BinaryConfig(
    structure=BinaryStructure(
        message_type_offset=0,  # First byte = MessageID
        payload_offset=1,  # Payload starts at byte 1
    ),
    limits=BinaryLimits(
        string_max_length=32,  # Match SysEx config
        array_max_items=32,  # Match SysEx config
        max_payload_size=10000,  # Match SysEx config
        max_message_size=16000,  # Match SysEx config
        include_message_name=True,  # Include MESSAGE_NAME prefix for bridge logging
    ),
)
