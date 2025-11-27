package config;

/**
 * BitwigConfig - Centralized configuration for Bitwig extension
 *
 * ALL timing constants are here. No observer-based logic.
 * Uses fixed delays like DrivenByMoss (industry standard).
 *
 * Sources:
 * - DrivenByMoss (LGPLv3) by Jürgen Moßgraber
 * - bitwig-extensions-main (MIT) by Bitwig
 */
public final class BitwigConfig {

    private BitwigConfig() {} // Prevent instantiation

    // ═══════════════════════════════════════════════════════════════════
    // TRACK NAVIGATION
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Delay after entering a track group (select + selectFirstChild).
     * Source: DrivenByMoss TrackImpl.java:166
     */
    public static final int TRACK_ENTER_GROUP_MS = 70;

    /**
     * Delay after exiting a track group (selectParent).
     */
    public static final int TRACK_EXIT_GROUP_MS = 70;

    // ═══════════════════════════════════════════════════════════════════
    // DEVICE NAVIGATION
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Delay after entering device child (slot/layer/drum).
     * Also used for device change macro send.
     */
    public static final int DEVICE_ENTER_CHILD_MS = 30;

    /**
     * Delay after selectParent on nested device.
     * Source: DrivenByMoss DeviceParamsMode.java:226
     * Kept higher because nested navigation is more complex.
     */
    public static final int DEVICE_EXIT_NESTED_MS = 70;

    /**
     * Delay after changing remote controls page.
     */
    public static final int PAGE_CHANGE_MS = 30;

    // ═══════════════════════════════════════════════════════════════════
    // TOUCH / AUTOMATION
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Delay after touch press before allowing parameter changes.
     * Prevents accidental changes during initial touch detection.
     */
    public static final int TOUCH_PRESS_DELAY_MS = 20;

    /**
     * Grace period after touch release.
     * Blocks updates to prevent automation playback re-trigger.
     */
    public static final int TOUCH_RELEASE_GRACE_MS = TOUCH_PRESS_DELAY_MS;

    /**
     * Window for detecting echo callbacks from controller changes.
     * Callbacks within this window are considered echoes (not sent to controller).
     */
    public static final int ECHO_TIMEOUT_MS = 100;
}
