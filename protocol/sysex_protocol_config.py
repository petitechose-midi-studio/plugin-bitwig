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
        string_max_length=32,    # Reasonable limit for device/track names
        array_max_items=32,      # Max items in lists (devices, tracks, pages)
        max_payload_size=10000,  # Max payload (largest struct ~9KB)
        max_message_size=16000,  # Match System::Protocol::MAX_MESSAGE_SIZE
    ),
)
