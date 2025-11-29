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
    // INITIALIZATION
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Delay at startup sync
     */
    public static final int INIT_MS = 200;

    // ═══════════════════════════════════════════════════════════════════
    // TRACK NAVIGATION
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Delay after entering a track group (select + selectFirstChild).
     * Source: DrivenByMoss TrackImpl.java:166
     */
    public static final int TRACK_ENTER_GROUP_MS = 100;

    /**
     * Delay after exiting a track group (selectParent).
     */
    public static final int TRACK_EXIT_GROUP_MS = 100;

    /**
     * Delay after track selection before sending track list.
     * Allows cursorTrack.position() to update after selectChannel().
     */
    public static final int TRACK_SELECT_DELAY_MS = 100;

    // ═══════════════════════════════════════════════════════════════════
    // DEVICE NAVIGATION
    // ═══════════════════════════════════════════════════════════════════

    /**
     * Delay after entering device child (slot/layer/drum).
     * Also used for device change macro send.
     */
    public static final int DEVICE_ENTER_CHILD_MS = 100;

    /**
     * Delay after device change before reading all properties.
     * Longer than DEVICE_ENTER_CHILD_MS because hasSlots/hasLayers/hasDrumPads
     * take longer to populate via Bitwig's observer system.
     */
    public static final int DEVICE_CHANGE_HEADER_MS = 100;

    /**
     * Delay after selectParent on nested device.
     * Source: DrivenByMoss DeviceParamsMode.java:226
     * Kept higher because nested navigation is more complex.
     */
    public static final int DEVICE_EXIT_NESTED_MS = 100;

    /**
     * Delay after changing remote controls page.
     */
    public static final int PAGE_CHANGE_MS = 100;

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
