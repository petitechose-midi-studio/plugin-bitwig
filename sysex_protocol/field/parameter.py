from protocol import PrimitiveField, Type

# ============================================================================
# GENERIC PARAMETER FIELDS
# ============================================================================
# These fields can be used by:
# - Device macros (with index 0-7)
# - Last clicked parameter (no index)
# - Mixer parameters (future)
# - Transport parameters (future)
#
# Parameter types:
# - 0: Knob (continuous or unrecognized discrete)
# - 1: Button (discreteCount == 2 AND values are On/Off)
# - 2: List (discreteCount > 2)

# Core parameter fields
parameter_value = PrimitiveField('parameterValue', type_name=Type.FLOAT32)
parameter_name = PrimitiveField('parameterName', type_name=Type.STRING)
parameter_display_value = PrimitiveField('displayValue', type_name=Type.STRING)
parameter_origin = PrimitiveField('parameterOrigin', type_name=Type.FLOAT32)
parameter_exists = PrimitiveField('parameterExists', type_name=Type.BOOL)
parameter_touched = PrimitiveField('isTouched', type_name=Type.BOOL)
parameter_is_echo = PrimitiveField('isEcho', type_name=Type.BOOL)

# Parameter type detection
parameter_type = PrimitiveField('parameterType', type_name=Type.UINT8)

# Discrete values (for List/Button parameters)
parameter_discrete_count = PrimitiveField('discreteValueCount', type_name=Type.INT16)
parameter_discrete_value_names = PrimitiveField('discreteValueNames', type_name=Type.STRING, array=32, dynamic=True)
parameter_current_value_index = PrimitiveField('currentValueIndex', type_name=Type.UINT8)
