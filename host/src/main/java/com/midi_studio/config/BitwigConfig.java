package com.midi_studio.config;

/**
 * BitwigConfig - Centralized configuration for Bitwig extension
 *
 * Contains timing delays and other configurable parameters to ensure
 * stable communication with Bitwig API.
 */
public class BitwigConfig {
    /**
     * Delay in milliseconds to wait after cursor navigation before sending device list.
     *
     * This delay is necessary because Bitwig needs time to update its internal
     * cursor state after navigation operations (selectParent, selectFirstInSlot, etc.)
     * before we can reliably query the new device list.
     *
     * Adjust this value based on stability:
     * - Lower values (5-10ms): Maximum UI response, may miss updates on slower systems
     * - Higher values (20-50ms): More reliable, slightly slower UI
     *
     * Default: 10ms (optimized for maximum speed)
     * Previous values: 20ms, 50ms
     */
    public static final int CURSOR_UPDATE_DELAY_MS = 10;

    /**
     * Delay in milliseconds before writing automation after touch press.
     * Prevents premature automation writes during initial touch detection.
     */
    public static final int TOUCH_PRESS_DELAY_MS = 10;

    /**
     * Grace period in milliseconds after touch release.
     * Blocks parameter value updates to prevent automation playback re-trigger.
     */
    public static final int TOUCH_RELEASE_GRACE_MS = 10;

    private BitwigConfig() {
        // Prevent instantiation
    }
}
