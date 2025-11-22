from protocol_codegen.core.field import PrimitiveField, Type

# ============================================================================
# TRANSPORT STATE FIELDS (Host → Controller feedback)
# ============================================================================

transport_play = PrimitiveField('isPlaying', type_name=Type.BOOL)
transport_record = PrimitiveField('isRecording', type_name=Type.BOOL)
transport_stop = PrimitiveField('isStopping', type_name=Type.BOOL)
tempo_value = PrimitiveField('tempo', type_name=Type.FLOAT32)
