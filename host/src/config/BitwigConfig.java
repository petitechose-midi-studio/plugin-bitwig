package config;

/**
 * BitwigConfig - Centralized configuration for Bitwig extension
 *
 * ALL timing constants are here. No observer-based logic.
 * Uses fixed delays like DrivenByMoss (industry standard).
 *
 * ═══════════════════════════════════════════════════════════════════════════
 * IMPORTANT: Request → Delay → Read → Send Pattern
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * When responding to controller requests (RequestTrackList, RequestDeviceList, etc.),
 * the Bitwig API values may not be updated yet after an action (selectChannel, etc.).
 *
 * CORRECT PATTERN for sendXXX() methods responding to requests:
 *
 *   public void sendTrackList() {
 *       host.scheduleTask(() -> {
 *           // 1. AFTER delay: read API values (now updated)
 *           final int selectedIndex = cursorTrack.position().get();
 *           final List<...> tracks = buildTrackList();
 *
 *           // 2. Send immediately
 *           protocol.send(new TrackListMessage(...));
 *       }, TRACK_SELECT_DELAY_MS);
 *   }
 *
 * EXCEPTION: Bitwig observers (addValueObserver) - values are already updated
 * when the callback fires, so no delay needed for observer-triggered sends.
 *
 * Sources:
 * - DrivenByMoss (LGPLv3) by Jürgen Moßgraber
 * - bitwig-extensions-main (MIT) by Bitwig
 */
public final class BitwigConfig {

    private BitwigConfig() {} // Prevent instantiation

    // ═══════════════════════════════════════════════════════════════════
    // STANDARD DELAYS
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Standard delay for Bitwig API to update values after an action.
     * Used for most navigation and state change operations.
     */
    public static final int STANDARD_DELAY_MS = 80;

    /**
     * Delay at startup sync (longer for initial stabilization)
     */
    public static final int INIT_MS = STANDARD_DELAY_MS * 2;

    // ═══════════════════════════════════════════════════════════════════
    // TRACK NAVIGATION
    // ═══════════════════════════════════════════════════════════════════

    /** Delay after entering a track group (2x standard - navigation is complex) */
    public static final int TRACK_ENTER_GROUP_MS = STANDARD_DELAY_MS;

    /** Delay after exiting a track group (2x standard - navigation is complex) */
    public static final int TRACK_EXIT_GROUP_MS = STANDARD_DELAY_MS;

    /** Delay after track selection before sending track list */
    public static final int TRACK_SELECT_DELAY_MS = STANDARD_DELAY_MS;

    // ═══════════════════════════════════════════════════════════════════
    // DEVICE NAVIGATION
    // ═══════════════════════════════════════════════════════════════════

    /** Delay after entering device child (2x standard - navigation is complex) */
    public static final int DEVICE_ENTER_CHILD_MS = STANDARD_DELAY_MS;

    /** Delay after device change before reading properties */
    public static final int DEVICE_CHANGE_HEADER_MS = STANDARD_DELAY_MS;

    /** Delay after exiting nested device (2x standard - navigation is complex) */
    public static final int DEVICE_EXIT_NESTED_MS = STANDARD_DELAY_MS;

    /** Delay after changing remote controls page */
    public static final int PAGE_CHANGE_MS = STANDARD_DELAY_MS;

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
    public static final int ECHO_TIMEOUT_MS = STANDARD_DELAY_MS / 4;

    /**
     * Timeout for mute/solo toggle confirmation.
     * If observer doesn't fire within this window, pending state expires.
     */
    public static final int TOGGLE_CONFIRM_TIMEOUT_MS = STANDARD_DELAY_MS;

    // ═══════════════════════════════════════════════════════════════════
    // BANK SIZES
    // ═══════════════════════════════════════════════════════════════════

    /** Number of remote control parameters per page */
    public static final int MAX_PARAMETERS = 8;

    /** Maximum tracks/devices in a bank */
    public static final int MAX_BANK_SIZE = 32;

    /** Maximum device children (slots + layers + drums) */
    public static final int MAX_CHILDREN = 16;
}
