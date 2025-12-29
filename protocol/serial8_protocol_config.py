"""
Bitwig Plugin Serial8 Configuration

Plugin-specific configuration for Serial8 (8-bit binary) protocol.
Used with oc-bridge for USB Serial communication.
"""

from protocol_codegen.methods.serial8 import Serial8Config, Serial8Limits, Serial8Structure


# Bitwig-specific Serial8 configuration
PROTOCOL_CONFIG = Serial8Config(
    structure=Serial8Structure(
        message_type_offset=0,  # First byte = MessageID
        payload_offset=1,       # Payload starts at byte 1
    ),
    limits=Serial8Limits(
        string_max_length=32,    # Match SysEx config
        array_max_items=32,      # Match SysEx config
        max_payload_size=10000,  # Match SysEx config
        max_message_size=16000,  # Match SysEx config
    ),
)
