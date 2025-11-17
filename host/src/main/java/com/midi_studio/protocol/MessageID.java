package com.midi_studio.protocol;

/**
 * MessageID - SysEx Message ID Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: sysex_messages.py + message_id_allocator
 *
 * This enum defines all valid SysEx message identifiers.
 * IDs are auto-allocated sequentially starting from 0x00.
 *
 * Total messages: 37
 */
public enum MessageID {

    // ========================================
    // Protocol Messages
    // ========================================

    DEVICE_CHANGE(0x00),  // Complete device state (name, enabled, page, 8 parameters)
    DEVICE_CHANGE_HEADER(0x01),  // Device change header - lightweight message with device identity only (sent first)
    DEVICE_CHILDREN(0x02),  // List of children for a device of specific type
    DEVICE_LIST(0x03),  // List of devices in current chain with context
    DEVICE_MACRO_DISCRETE_VALUES(0x04),  // Full list of discrete values for List parameters (lazy-loaded on demand)
    DEVICE_MACRO_NAME_CHANGE(0x05),  // Single parameter name change
    DEVICE_MACRO_TOUCH(0x06),  // Touch automation start/stop for macro parameter
    DEVICE_MACRO_UPDATE(0x07),  // Complete macro parameter update - sent individually per parameter (8x after header)
    DEVICE_MACRO_VALUE_CHANGE(0x08),  // Single parameter value update with echo flag for optimistic updates
    DEVICE_PAGE_CHANGE(0x09),  // Page change with new parameter set
    DEVICE_PAGE_NAMES(0x0A),  // List of all available page names
    DEVICE_PAGE_SELECT_BY_INDEX(0x0B),  // Select device page by index (modulo pageCount)
    DEVICE_SELECT_BY_INDEX(0x0C),  // Select device by index in current chain
    DEVICE_STATE_CHANGE(0x0D),  // Toggle device enabled/bypassed by index
    ENTER_DEVICE_CHILD(0x0E),  // Navigate into a child (slot/layer/drum pad)
    ENTER_TRACK_GROUP(0x0F),  // Navigate into a track group to see its children
    EXIT_TO_PARENT(0x10),  // Navigate back to parent device chain
    EXIT_TRACK_GROUP(0x11),  // Navigate back to parent track context
    HOST_DEACTIVATED(0x12),  // Host plugin deactivating
    HOST_INITIALIZED(0x13),  // Host plugin initialized and active
    LAST_CLICKED_TOUCH(0x14),  // Touch automation for last clicked parameter
    LAST_CLICKED_UPDATE(0x15),  // Last clicked parameter update - sent when user clicks a new parameter
    LAST_CLICKED_VALUE_CHANGE(0x16),  // Last clicked parameter value change (bidirectional with echo flag)
    REQUEST_DEVICE_CHILDREN(0x17),  // Request children (slots/layers/drums) for device and type
    REQUEST_DEVICE_LIST(0x18),  // Request list of devices in current device chain
    REQUEST_DEVICE_PAGE_NAMES(0x19),  // Request list of all available page names
    REQUEST_HOST_STATUS(0x1A),  // Request current host status (triggers HOST_INITIALIZED response)
    REQUEST_TRACK_LIST(0x1B),  // Request list of tracks in current context
    TRACK_ACTIVATE(0x1C),  // Toggle track activated/deactivated state
    TRACK_CHANGE(0x1D),  // Track context change notification
    TRACK_LIST(0x1E),  // List of tracks in current context with navigation info
    TRACK_MUTE(0x1F),  // Toggle track mute state
    TRACK_SELECT_BY_INDEX(0x20),  // Select track by index in current context
    TRACK_SOLO(0x21),  // Toggle track solo state
    TRANSPORT_PLAY(0x22),  // Transport play/pause state
    TRANSPORT_RECORD(0x23),  // Transport Record Request
    TRANSPORT_STOP(0x24);  // Transport Stop Request


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
