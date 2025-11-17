from protocol_codegen.core.field import PrimitiveField, Type

# ============================================================================
# PLUGIN STATE FIELDS (Host → Controller feedback)
# ============================================================================

# Host active state (true=host initialized and active, false=host deactivating)
host_active = PrimitiveField('isHostActive', type_name=Type.BOOL)
