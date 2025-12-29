"""
Transport Control Messages

Separated into explicit directions:
- COMMANDS (Controller → Host): User actions to change transport state
- NOTIFICATIONS (Host → Controller): State feedback from Bitwig

TRANSPORT MESSAGES:
- TRANSPORT_PLAY: Set play state (Controller → Host)
- TRANSPORT_PLAYING_STATE: Play state changed (Host → Controller)
- TRANSPORT_STOP: Stop transport (Controller → Host)
- TRANSPORT_RECORD: Set record state (Controller → Host)
- TRANSPORT_RECORDING_STATE: Record state changed (Host → Controller)
- TRANSPORT_TEMPO: Adjust tempo (Controller → Host)
- TRANSPORT_TEMPO_STATE: Tempo value notification (Host → Controller)

AUTOMATION MESSAGES:
- TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE: Override state (Host → Controller)
- TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED: Set arranger write (Controller → Host)
- TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE: Arranger write state (Host → Controller)
- etc.

OVERDUB MESSAGES:
- TRANSPORT_ARRANGER_OVERDUB_ENABLED: Set arranger overdub (Controller → Host)
- TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE: Arranger overdub state (Host → Controller)
- etc.
"""

from field.transport import *
from protocol_codegen.core.enums import Direction, Intent
from protocol_codegen.core.message import Message


# ============================================================================
# COMMANDS (Controller → Host)
# ============================================================================

TRANSPORT_PLAY = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set transport play state',
    fields=[transport_play]
)

TRANSPORT_STOP = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Stop transport',
    fields=[]
)

TRANSPORT_RECORD = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set transport record state',
    fields=[transport_record]
)

TRANSPORT_TEMPO = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Adjust tempo (relative or absolute)',
    fields=[tempo_value]
)

RESET_AUTOMATION_OVERRIDES = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Reset all automation overrides globally (resetAutomationOverrides())',
    fields=[]
)

# Automation write enable commands
TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set arranger automation write enabled state',
    fields=[is_arranger_automation_write_enabled]
)

TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set clip launcher automation write enabled state',
    fields=[is_clip_launcher_automation_write_enabled]
)

TRANSPORT_AUTOMATION_WRITE_MODE = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set automation write mode (latch/touch/write)',
    fields=[automation_write_mode]
)

# Overdub enable commands
TRANSPORT_ARRANGER_OVERDUB_ENABLED = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set arranger overdub enabled state',
    fields=[is_arranger_overdub_enabled]
)

TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED = Message(
    direction=Direction.TO_HOST,
    intent=Intent.COMMAND,
    description='Set clip launcher overdub enabled state',
    fields=[is_clip_launcher_overdub_enabled]
)


# ============================================================================
# NOTIFICATIONS (Host → Controller)
# ============================================================================

TRANSPORT_PLAYING_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Transport playing state changed',
    fields=[transport_play]
)

TRANSPORT_RECORDING_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Transport recording state changed',
    fields=[transport_record]
)

TRANSPORT_TEMPO_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='Tempo value notification',
    fields=[tempo_value]
)

# Automation state notifications
TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='isAutomationOverrideActive() state changed',
    fields=[is_automation_override_active]
)

TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='isArrangerAutomationWriteEnabled() state changed',
    fields=[is_arranger_automation_write_enabled]
)

TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='isClipLauncherAutomationWriteEnabled() state changed',
    fields=[is_clip_launcher_automation_write_enabled]
)

TRANSPORT_AUTOMATION_WRITE_MODE_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='automationWriteMode() state changed',
    fields=[automation_write_mode]
)

# Overdub state notifications
TRANSPORT_ARRANGER_OVERDUB_ENABLED_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='isArrangerOverdubEnabled() state changed',
    fields=[is_arranger_overdub_enabled]
)

TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_STATE = Message(
    direction=Direction.TO_CONTROLLER,
    intent=Intent.NOTIFY,
    description='isClipLauncherOverdubEnabled() state changed',
    fields=[is_clip_launcher_overdub_enabled]
)
