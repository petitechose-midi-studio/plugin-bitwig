"""
Plugin Lifecycle Messages

Messages for plugin initialization and deactivation lifecycle.
These messages allow the host to signal its state to the controller.

MESSAGES:
- REQUEST_HOST_STATUS: Controller requests current host status (Controller → Host)
- HOST_INITIALIZED: Host plugin is initialized and ready (Host → Controller)
- HOST_DEACTIVATED: Host plugin is deactivating/closing (Host → Controller)
"""

from field.plugin import *
from protocol_codegen.core.enums import Direction, Intent
from protocol_codegen.core.message import Message


# ============================================================================
# Plugin Lifecycle Requests (Controller → Host)
# ============================================================================

REQUEST_HOST_STATUS = Message(
    direction=Direction.TO_HOST,
    intent=Intent.QUERY,
    description='Request current host status (triggers HOST_INITIALIZED response)',
    fields=[]  # No payload - simple ping/request
)


# ============================================================================
# Plugin Lifecycle (Host → Controller)
# ============================================================================

HOST_INITIALIZED = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Host plugin initialized and active',
    fields=[host_active]  # isHostActive = true
)

HOST_DEACTIVATED = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Host plugin deactivating',
    fields=[host_active]  # isHostActive = false
)
