"""
Transport Control Messages

Bidirectional messages for transport state synchronization between
Bitwig and the hardware controller.

MESSAGES:
- TRANSPORT_PLAY: Play/pause state (bidirectional)
- TRANSPORT_RECORD: Record arm state (bidirectional)
- TRANSPORT_STOP: Stop transport (bidirectional)
- TRANSPORT_TEMPO: Tempo in BPM (bidirectional)

AUTOMATION MESSAGES (Granular - Bidirectional for Settable values):
- TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE: Global override state (Host → Controller only)
- TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE: Arranger write armed (bidirectional)
- TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE: Clip launcher write armed (bidirectional)
- TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE: Write mode latch/touch/write (bidirectional)
- RESET_AUTOMATION_OVERRIDES: Controller → Host request to reset all overrides

OVERDUB MESSAGES (Granular - Bidirectional):
- TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE: Arranger overdub enabled (bidirectional)
- TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE: Clip launcher overdub enabled (bidirectional)
"""

from field.transport import *
from protocol_codegen.core.message import Message


# ============================================================================
# Transport State (Bidirectional)
# ============================================================================

TRANSPORT_PLAY = Message(
    description='Transport play/pause state',
    fields=[transport_play]
)

TRANSPORT_RECORD = Message(
    description='Transport Record Request',
    fields=[transport_record]
)

TRANSPORT_STOP = Message(
    description='Transport Stop Request',
    fields=[transport_stop]
)

TRANSPORT_TEMPO = Message(
    description='Tempo in BPM (bidirectional)',
    fields=[tempo_value]
)

# ============================================================================
# Transport Automation State (Granular)
# ============================================================================
# Aligned with Bitwig API Transport.java naming
# Settable values are bidirectional (Controller can toggle)

TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE = Message(
    description='isAutomationOverrideActive() state changed (Host → Controller only)',
    fields=[is_automation_override_active]
)

TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE = Message(
    description='isArrangerAutomationWriteEnabled() state (bidirectional)',
    fields=[is_arranger_automation_write_enabled]
)

TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE = Message(
    description='isClipLauncherAutomationWriteEnabled() state (bidirectional)',
    fields=[is_clip_launcher_automation_write_enabled]
)

TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE = Message(
    description='automationWriteMode() latch/touch/write (bidirectional)',
    fields=[automation_write_mode]
)

# ============================================================================
# Transport Automation Control (Controller → Host)
# ============================================================================

RESET_AUTOMATION_OVERRIDES = Message(
    description='Reset all automation overrides globally (resetAutomationOverrides())',
    fields=[]
)

# ============================================================================
# Transport Overdub State (Granular - Bidirectional)
# ============================================================================
# Aligned with Bitwig API Transport.java naming

TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE = Message(
    description='isArrangerOverdubEnabled() state (bidirectional)',
    fields=[is_arranger_overdub_enabled]
)

TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE = Message(
    description='isClipLauncherOverdubEnabled() state (bidirectional)',
    fields=[is_clip_launcher_overdub_enabled]
)
