from protocol_codegen.core.field import PrimitiveField, Type

# ============================================================================
# TRANSPORT STATE FIELDS (Host → Controller feedback)
# ============================================================================

transport_play = PrimitiveField('isPlaying', type_name=Type.BOOL)
transport_record = PrimitiveField('isRecording', type_name=Type.BOOL)
transport_stop = PrimitiveField('isStopping', type_name=Type.BOOL)
tempo_value = PrimitiveField('tempo', type_name=Type.FLOAT32)

# ============================================================================
# TRANSPORT AUTOMATION FIELDS (Host → Controller feedback)
# ============================================================================
# Aligned with Bitwig API Transport.java naming

# isAutomationOverrideActive() - true when any parameter has been manually overridden
is_automation_override_active = PrimitiveField('isAutomationOverrideActive', type_name=Type.BOOL)

# isArrangerAutomationWriteEnabled() - arranger automation write armed
is_arranger_automation_write_enabled = PrimitiveField('isArrangerAutomationWriteEnabled', type_name=Type.BOOL)

# isClipLauncherAutomationWriteEnabled() - clip launcher automation write armed
is_clip_launcher_automation_write_enabled = PrimitiveField('isClipLauncherAutomationWriteEnabled', type_name=Type.BOOL)

# automationWriteMode() - "latch", "touch", "write"
automation_write_mode = PrimitiveField('automationWriteMode', type_name=Type.STRING)

# ============================================================================
# TRANSPORT OVERDUB FIELDS (Bidirectional)
# ============================================================================
# Aligned with Bitwig API Transport.java naming

# isArrangerOverdubEnabled() - arranger overdub enabled
is_arranger_overdub_enabled = PrimitiveField('isArrangerOverdubEnabled', type_name=Type.BOOL)

# isClipLauncherOverdubEnabled() - clip launcher overdub enabled
is_clip_launcher_overdub_enabled = PrimitiveField('isClipLauncherOverdubEnabled', type_name=Type.BOOL)
