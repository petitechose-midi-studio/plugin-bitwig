package protocol;

/**
 * MessageID - SysEx Message ID Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: sysex_messages.py + message_id_allocator
 *
 * This enum defines all valid SysEx message identifiers.
 * IDs are auto-allocated sequentially starting from 0x00.
 *
 * Total messages: 66
 */
public enum MessageID {

    // ========================================
    // Protocol Messages
    // ========================================

    DEVICE_CHANGE(0x00),  // Complete device state (name, enabled, page, 8 remote controls)
    DEVICE_CHANGE_HEADER(0x01),  // Device change header - lightweight message with device identity only (sent first)
    DEVICE_CHILDREN(0x02),  // List of children for a device of specific type
    DEVICE_LIST(0x03),  // List of devices in current chain with context
    DEVICE_PAGE_CHANGE(0x04),  // Page change with new remote controls set
    DEVICE_PAGE_NAMES(0x05),  // List of all available page names
    DEVICE_PAGE_SELECT_BY_INDEX(0x06),  // Select device page by index (modulo pageCount)
    DEVICE_REMOTE_CONTROL_DISCRETE_VALUES(0x07),  // Full list of discrete values for List parameters (lazy-loaded on demand)
    DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE(0x08),  // hasAutomation() state changed for remote control
    DEVICE_REMOTE_CONTROL_MODULATED_VALUE_CHANGE(0x09),  // modulatedValue() changed for remote control (automation/modulation applied)
    DEVICE_REMOTE_CONTROL_NAME_CHANGE(0x0A),  // Single remote control name change
    DEVICE_REMOTE_CONTROL_TOUCH(0x0B),  // Touch automation start/stop for remote control parameter
    DEVICE_REMOTE_CONTROL_UPDATE(0x0C),  // Complete remote control update - sent individually per parameter (8x after header)
    DEVICE_REMOTE_CONTROL_VALUE_CHANGE(0x0D),  // Single remote control value update with echo flag for optimistic updates
    DEVICE_SELECT_BY_INDEX(0x0E),  // Select device by index in current chain
    DEVICE_STATE_CHANGE(0x0F),  // Toggle device enabled/bypassed by index
    ENTER_DEVICE_CHILD(0x10),  // Navigate into a child (slot/layer/drum pad)
    ENTER_TRACK_GROUP(0x11),  // Navigate into a track group to see its children
    EXIT_TO_PARENT(0x12),  // Navigate back to parent device chain
    EXIT_TRACK_GROUP(0x13),  // Navigate back to parent track context
    HOST_DEACTIVATED(0x14),  // Host plugin deactivating
    HOST_INITIALIZED(0x15),  // Host plugin initialized and active
    LAST_CLICKED_TOUCH(0x16),  // Touch automation for last clicked parameter
    LAST_CLICKED_UPDATE(0x17),  // Last clicked parameter update - sent when user clicks a new parameter
    LAST_CLICKED_VALUE_CHANGE(0x18),  // Last clicked parameter value change (bidirectional with echo flag)
    REQUEST_DEVICE_CHILDREN(0x19),  // Request children (slots/layers/drums) for device and type
    REQUEST_DEVICE_LIST(0x1A),  // Request list of devices in current device chain
    REQUEST_DEVICE_PAGE_NAMES(0x1B),  // Request list of all available page names
    REQUEST_HOST_STATUS(0x1C),  // Request current host status (triggers HOST_INITIALIZED response)
    REQUEST_TRACK_LIST(0x1D),  // Request list of tracks in current context
    REQUEST_TRACK_SEND_LIST(0x1E),  // Request list of sends for current track
    RESET_AUTOMATION_OVERRIDES(0x1F),  // Reset all automation overrides globally (resetAutomationOverrides())
    TRACK_ACTIVATE(0x20),  // Toggle track activated/deactivated state
    TRACK_ARM_CHANGE(0x21),  // Track record arm state change (bidirectional)
    TRACK_CHANGE(0x22),  // Track context change notification with full channel state
    TRACK_LIST(0x23),  // List of tracks in current context with navigation info
    TRACK_MUTE(0x24),  // Toggle track mute state
    TRACK_MUTED_BY_SOLO_CHANGE(0x25),  // Track muted by solo state changed (Host → Controller only)
    TRACK_PAN_CHANGE(0x26),  // Track pan change (bidirectional with echo)
    TRACK_PAN_HAS_AUTOMATION_CHANGE(0x27),  // Track pan hasAutomation() state changed
    TRACK_PAN_MODULATED_VALUE_CHANGE(0x28),  // Track pan modulatedValue() changed (automation/modulation applied)
    TRACK_PAN_TOUCH(0x29),  // Touch automation start/stop for track pan
    TRACK_SELECT_BY_INDEX(0x2A),  // Select track by index in current context
    TRACK_SEND_ENABLED_CHANGE(0x2B),  // Track send enabled state change (bidirectional)
    TRACK_SEND_HAS_AUTOMATION_CHANGE(0x2C),  // Track send hasAutomation() state changed
    TRACK_SEND_LIST(0x2D),  // List of sends for current track
    TRACK_SEND_MODE_CHANGE(0x2E),  // Track send mode change (bidirectional)
    TRACK_SEND_MODULATED_VALUE_CHANGE(0x2F),  // Track send modulatedValue() changed (automation/modulation applied)
    TRACK_SEND_PRE_FADER_CHANGE(0x30),  // Track send pre-fader state changed (Host → Controller only)
    TRACK_SEND_TOUCH(0x31),  // Touch automation start/stop for track send
    TRACK_SEND_VALUE_CHANGE(0x32),  // Track send value change (bidirectional with echo)
    TRACK_SOLO(0x33),  // Toggle track solo state
    TRACK_VOLUME_CHANGE(0x34),  // Track volume change (bidirectional with echo)
    TRACK_VOLUME_HAS_AUTOMATION_CHANGE(0x35),  // Track volume hasAutomation() state changed
    TRACK_VOLUME_MODULATED_VALUE_CHANGE(0x36),  // Track volume modulatedValue() changed (automation/modulation applied)
    TRACK_VOLUME_TOUCH(0x37),  // Touch automation start/stop for track volume
    TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE(0x38),  // isArrangerAutomationWriteEnabled() state (bidirectional)
    TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE(0x39),  // isArrangerOverdubEnabled() state (bidirectional)
    TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE(0x3A),  // isAutomationOverrideActive() state changed (Host → Controller only)
    TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE(0x3B),  // automationWriteMode() latch/touch/write (bidirectional)
    TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE(0x3C),  // isClipLauncherAutomationWriteEnabled() state (bidirectional)
    TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE(0x3D),  // isClipLauncherOverdubEnabled() state (bidirectional)
    TRANSPORT_PLAY(0x3E),  // Transport play/pause state
    TRANSPORT_RECORD(0x3F),  // Transport Record Request
    TRANSPORT_STOP(0x40),  // Transport Stop Request
    TRANSPORT_TEMPO(0x41);  // Tempo in BPM (bidirectional)


    private final byte value;

    MessageID(int value) {
        this.value = (byte) value;
    }

    public byte getValue() {
        return value;
    }

    /**
     * Get MessageID from byte value
     * @param value The byte value
     * @return MessageID enum constant, or null if not found
     */
    public static MessageID fromValue(byte value) {
        for (MessageID id : values()) {
            if (id.value == value) {
                return id;
            }
        }
        return null;
    }

    /**
     * Total number of defined messages
     */
    public static int getMessageCount() {
        return values().length;
    }

}
