from protocol import PrimitiveField, Type

# ============================================================================
# TRANSPORT STATE FIELDS (Host → Controller feedback)
# ============================================================================

# Transport play/pause state (true=playing, false=paused)
transport_play = PrimitiveField('isPlaying', type_name=Type.BOOL)
transport_record = PrimitiveField('isRecording', type_name=Type.BOOL)
transport_stop = PrimitiveField('isStopping', type_name=Type.BOOL)
