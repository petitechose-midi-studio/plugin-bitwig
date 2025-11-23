package config;

/**
 * BitwigConfig - Centralized configuration for Bitwig extension
 *
 * Contains timing delays and other configurable parameters to ensure
 * stable communication with Bitwig API.
 */
public class BitwigConfig {
    /**
     * Delay for single element operations.
     * Lightweight operations on individual items.
     * Used for: selectDevice, selectParent, selectFirstChild, selectFirstInSlot
     */
    public static final int SINGLE_ELEMENT_DELAY_MS = 6;

    /**
     * Delay for list/bulk operations.
     * Heavier operations that query multiple items and build lists.
     * Used for: sendDeviceList, sendTrackList, sendDeviceChildren
     */
    public static final int LIST_OPERATION_DELAY_MS = 7;

    /**
     * Delay for complex nested operations.
     * Operations requiring multiple sequential API calls.
     * Used for: selectInEditor + selectFirstInChannel (layers)
     */
    public static final int COMPLEX_OPERATION_DELAY_MS = 10;

    /**
     * Stagger factor for bulk message sending
     * Used to spread out messages in loops to avoid bandwidth saturation.
     */
    public static final int BULK_MESSAGE_STAGGER_MS = 2;

    /**
     * Delay in milliseconds before writing automation after touch press.
     * Prevents premature automation writes during initial touch detection.
     */
    public static final int TOUCH_PRESS_DELAY_MS = 10;

    /**
     * Grace period in milliseconds after touch release.
     * Blocks parameter value updates to prevent automation playback re-trigger.
     */
    public static final int TOUCH_RELEASE_GRACE_MS = TOUCH_PRESS_DELAY_MS;

    /**
     * Delay for sending initial state to controller at startup.
     * Allows Bitwig API values (tempo, play state, etc.) to stabilize before
     * reading.
     * Used for: sendInitialState in Host handlers
     */
    public static final int INITIAL_STATE_SEND_DELAY_MS = 50;

    private BitwigConfig() {
        // Prevent instantiation
    }
}
