"""
Bitwig Plugin SysEx Configuration (Pure Python, Type-Safe)

Plugin-specific overrides for SysEx protocol constants.
"""

from protocol_codegen.methods.sysex import SysExConfig, SysExFraming, SysExLimits


# Bitwig-specific configuration
PROTOCOL_CONFIG = SysExConfig(
    framing=SysExFraming(
        manufacturer_id=0x7F,  # Bitwig uses 0x7F (experimental/development)
        device_id=0x01,        # Bitwig MIDI Studio device
    ),
    limits=SysExLimits(
        max_message_size=16000,  # Match System::Protocol::MAX_MESSAGE_SIZE
    ),
)
