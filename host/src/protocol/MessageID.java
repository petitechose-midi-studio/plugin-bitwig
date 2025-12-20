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
 * Total messages: 79
 */
public enum MessageID {

    // ========================================
    // Protocol Messages
    // ========================================

    DEVICE_CHANGE(0x00),  // Complete device state (name, enabled, page, 8 remote controls)
    DEVICE_CHANGE_HEADER(0x01),  // Device change header - lightweight message with device identity only (sent first)
    DEVICE_CHILDREN(0x02),  // List of children for a device of specific type
    DEVICE_LIST(0x03),  // [DEPRECATED] List of devices in current chain with context
    DEVICE_LIST_WINDOW(0x04),  // Windowed device list response (16 items max)
    DEVICE_PAGE_CHANGE(0x05),  // Page change with new remote controls set
    DEVICE_PAGE_NAMES(0x06),  // [DEPRECATED] List of all available page names
    DEVICE_PAGE_NAMES_WINDOW(0x07),  // Windowed page names response (16 items max)
    DEVICE_PAGE_SELECT_BY_INDEX(0x08),  // Select device page by index (modulo pageCount)
    DEVICE_REMOTE_CONTROLS_MODULATED_VALUES_BATCH(0x09),  // Batch update of all 8 remote control modulated values (sent at fixed rate ~30Hz)
    DEVICE_REMOTE_CONTROL_DISCRETE_VALUES(0x0A),  // Full list of discrete values for List parameters (lazy-loaded on demand)
    DEVICE_REMOTE_CONTROL_HAS_AUTOMATION_CHANGE(0x0B),  // hasAutomation() state changed for remote control
    DEVICE_REMOTE_CONTROL_IS_MODULATED_CHANGE(0x0C),  // isModulated state changed for remote control (modulation source added/removed)
    DEVICE_REMOTE_CONTROL_MODULATED_VALUE_CHANGE(0x0D),  // modulatedValue() changed for remote control (automation/modulation applied)
    DEVICE_REMOTE_CONTROL_NAME_CHANGE(0x0E),  // Single remote control name change
    DEVICE_REMOTE_CONTROL_ORIGIN_CHANGE(0x0F),  // origin changed for remote control (bipolar center point)
    DEVICE_REMOTE_CONTROL_TOUCH(0x10),  // Touch automation start/stop for remote control parameter
    DEVICE_REMOTE_CONTROL_UPDATE(0x11),  // Complete remote control update - sent individually per parameter (8x after header)
    DEVICE_REMOTE_CONTROL_VALUE_CHANGE(0x12),  // Single remote control value update with echo flag for optimistic updates
    DEVICE_SELECT_BY_INDEX(0x13),  // Select device by index in current chain
    DEVICE_STATE_CHANGE(0x14),  // Toggle device enabled/bypassed by index
    ENTER_DEVICE_CHILD(0x15),  // Navigate into a child (slot/layer/drum pad)
    ENTER_TRACK_GROUP(0x16),  // Navigate into a track group to see its children
    EXIT_TO_PARENT(0x17),  // Navigate back to parent device chain
    EXIT_TRACK_GROUP(0x18),  // Navigate back to parent track context
    HOST_DEACTIVATED(0x19),  // Host plugin deactivating
    HOST_INITIALIZED(0x1A),  // Host plugin initialized and active
    LAST_CLICKED_TOUCH(0x1B),  // Touch automation for last clicked parameter
    LAST_CLICKED_UPDATE(0x1C),  // Last clicked parameter update - sent when user clicks a new parameter
    LAST_CLICKED_VALUE_CHANGE(0x1D),  // Last clicked parameter value change (bidirectional with echo flag)
    REQUEST_DEVICE_CHILDREN(0x1E),  // Request children (slots/layers/drums) for device and type
    REQUEST_DEVICE_LIST(0x1F),  // [DEPRECATED] Request list of devices in current device chain
    REQUEST_DEVICE_LIST_WINDOW(0x20),  // Request device list starting at index (windowed, 16 items)
    REQUEST_DEVICE_PAGE_NAMES(0x21),  // [DEPRECATED] Request list of all available page names
    REQUEST_DEVICE_PAGE_NAMES_WINDOW(0x22),  // Request page names starting at index (windowed, 16 items)
    REQUEST_HOST_STATUS(0x23),  // Request current host status (triggers HOST_INITIALIZED response)
    REQUEST_SEND_DESTINATIONS(0x24),  // Request list of send destination names (Controller → Host)
    REQUEST_TRACK_LIST(0x25),  // [DEPRECATED] Request list of tracks in current context
    REQUEST_TRACK_LIST_WINDOW(0x26),  // Request track list starting at index (windowed, 16 items)
    REQUEST_TRACK_SEND_LIST(0x27),  // Request list of sends for current track
    RESET_AUTOMATION_OVERRIDES(0x28),  // Reset all automation overrides globally (resetAutomationOverrides())
    SELECT_MIX_SEND(0x29),  // Select which send to observe for MixView (Controller → Host)
    SEND_DESTINATIONS_LIST(0x2A),  // List of send destination names (effect track names)
    TRACK_ACTIVATE(0x2B),  // Toggle track activated/deactivated state
    TRACK_ARM_CHANGE(0x2C),  // Track record arm state change (bidirectional)
    TRACK_CHANGE(0x2D),  // Track context change notification with full channel state
    TRACK_LIST(0x2E),  // [DEPRECATED] List of tracks in current context with navigation info
    TRACK_LIST_WINDOW(0x2F),  // Windowed track list response (16 items max)
    TRACK_MUTE(0x30),  // Toggle track mute state
    TRACK_MUTED_BY_SOLO_CHANGE(0x31),  // Track muted by solo state changed (Host → Controller only)
    TRACK_PAN_CHANGE(0x32),  // Track pan change (bidirectional with echo)
    TRACK_PAN_HAS_AUTOMATION_CHANGE(0x33),  // Track pan hasAutomation() state changed
    TRACK_PAN_MODULATED_VALUE_CHANGE(0x34),  // Track pan modulatedValue() changed (automation/modulation applied)
    TRACK_PAN_TOUCH(0x35),  // Touch automation start/stop for track pan
    TRACK_SELECT_BY_INDEX(0x36),  // Select track by index in current context
    TRACK_SEND_ENABLED_CHANGE(0x37),  // Track send enabled state change (bidirectional)
    TRACK_SEND_HAS_AUTOMATION_CHANGE(0x38),  // Track send hasAutomation() state changed
    TRACK_SEND_LIST(0x39),  // List of sends for current track
    TRACK_SEND_MODE_CHANGE(0x3A),  // Track send mode change (bidirectional)
    TRACK_SEND_MODULATED_VALUE_CHANGE(0x3B),  // Track send modulatedValue() changed (automation/modulation applied)
    TRACK_SEND_PRE_FADER_CHANGE(0x3C),  // Track send pre-fader state changed (Host → Controller only)
    TRACK_SEND_TOUCH(0x3D),  // Touch automation start/stop for track send
    TRACK_SEND_VALUE_CHANGE(0x3E),  // Track send value change (bidirectional with echo)
    TRACK_SOLO(0x3F),  // Toggle track solo state
    TRACK_VOLUME_CHANGE(0x40),  // Track volume change (bidirectional with echo)
    TRACK_VOLUME_HAS_AUTOMATION_CHANGE(0x41),  // Track volume hasAutomation() state changed
    TRACK_VOLUME_MODULATED_VALUE_CHANGE(0x42),  // Track volume modulatedValue() changed (automation/modulation applied)
    TRACK_VOLUME_TOUCH(0x43),  // Touch automation start/stop for track volume
    TRANSPORT_ARRANGER_AUTOMATION_WRITE_ENABLED_CHANGE(0x44),  // isArrangerAutomationWriteEnabled() state (bidirectional)
    TRANSPORT_ARRANGER_OVERDUB_ENABLED_CHANGE(0x45),  // isArrangerOverdubEnabled() state (bidirectional)
    TRANSPORT_AUTOMATION_OVERRIDE_ACTIVE_CHANGE(0x46),  // isAutomationOverrideActive() state changed (Host → Controller only)
    TRANSPORT_AUTOMATION_WRITE_MODE_CHANGE(0x47),  // automationWriteMode() latch/touch/write (bidirectional)
    TRANSPORT_CLIP_LAUNCHER_AUTOMATION_WRITE_ENABLED_CHANGE(0x48),  // isClipLauncherAutomationWriteEnabled() state (bidirectional)
    TRANSPORT_CLIP_LAUNCHER_OVERDUB_ENABLED_CHANGE(0x49),  // isClipLauncherOverdubEnabled() state (bidirectional)
    TRANSPORT_PLAY(0x4A),  // Transport play/pause state
    TRANSPORT_RECORD(0x4B),  // Transport Record Request
    TRANSPORT_STOP(0x4C),  // Transport Stop Request
    TRANSPORT_TEMPO(0x4D),  // Tempo in BPM (bidirectional)
    VIEW_STATE_CHANGE(0x4E);  // Controller view state changed (view type or selector visibility)


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
