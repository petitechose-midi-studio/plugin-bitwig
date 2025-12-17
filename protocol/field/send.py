from protocol_codegen.core.field import PrimitiveField, CompositeField, Type
from field.color import color_rgb
from field.parameter import parameter_touched, parameter_is_echo, parameter_has_automation, parameter_modulated_value

# ============================================================================
# SEND FIELDS
# ============================================================================
# Aligned with Bitwig API Send.java (extends Parameter)
# Sends are per-track, sending audio to effect returns

# Send index (position in send bank, 0-based)
send_index = PrimitiveField('sendIndex', type_name=Type.UINT8)

# Send count (total sends available)
send_count = PrimitiveField('sendCount', type_name=Type.UINT8)

# Core parameter fields (inherited from Parameter)
send_value = PrimitiveField('sendValue', type_name=Type.FLOAT32)  # Normalized 0.0-1.0
send_display_value = PrimitiveField('sendDisplayValue', type_name=Type.STRING)  # "0 dB", "-inf"
send_name = PrimitiveField('sendName', type_name=Type.STRING)  # Destination track name

# Send-specific fields
send_channel_color = color_rgb  # Destination track color (uint32 RGB)
send_is_enabled = PrimitiveField('sendIsEnabled', type_name=Type.BOOL)  # isEnabled()
send_mode = PrimitiveField('sendMode', type_name=Type.STRING)  # "AUTO", "PRE", "POST"
send_is_pre_fader = PrimitiveField('sendIsPreFader', type_name=Type.BOOL)  # isPreFader() (read-only)

# ============================================================================
# COMPOSITE STRUCT FOR SEND LIST
# ============================================================================

# SendInfo: Information about a send in the bank
send_info = [
    send_index,           # Position in bank (uint8)
    send_name,            # Destination track name (string)
    send_channel_color,   # Destination track color (uint32)
    send_value,           # Send level (float32, 0.0-1.0)
    send_display_value,   # Display value (string)
    send_is_enabled,      # Send enabled (bool)
    send_mode,            # Send mode (string: AUTO/PRE/POST)
    send_is_pre_fader     # Is pre-fader (bool, read-only)
]

# Array of sends (max 8 sends per track, typical for most DAWs)
send_list = CompositeField('sends', fields=send_info, array=8)
