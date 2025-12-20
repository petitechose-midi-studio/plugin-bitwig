from protocol_codegen.core.field import PrimitiveField, Type

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

# ============================================================================
# PARAMETER AUTOMATION FIELDS
# ============================================================================
# Aligned with Bitwig API Parameter.java naming

# hasAutomation() - true if parameter has automation data
parameter_has_automation = PrimitiveField('hasAutomation', type_name=Type.BOOL)

# modulatedValue() - value after automation/modulation applied (0.0-1.0)
parameter_modulated_value = PrimitiveField('modulatedValue', type_name=Type.FLOAT32)

# isModulated - true if parameter has modulation source (automation, LFO, envelope, etc.)
# Used to determine if ribbon should be displayed on knob
parameter_is_modulated = PrimitiveField('isModulated', type_name=Type.BOOL)

# ============================================================================
# BATCHED AUTOMATION FIELDS
# ============================================================================
# For efficient batch updates of all 8 remote control modulated values

# Sequence number for staleness detection (0-255, wraps)
# Used to detect USB buffer accumulation and discard stale batches
batch_sequence_number = PrimitiveField('sequenceNumber', type_name=Type.UINT8)

# Array of 8 modulated values (one per remote control slot)
# Uses NORM8 (1 byte per value) for minimal bandwidth
# Total: 8 bytes instead of 40 bytes per batch (80% reduction)
# Precision: ~0.8% (1/127), sufficient for visual display of modulation ribbon
parameter_modulated_values_batch = PrimitiveField('modulatedValues', type_name=Type.NORM8, array=8)
