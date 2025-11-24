package config;

/**
 * BitwigConfig - Centralized configuration for Bitwig extension
 *
 * All timing is now handled by observer-based system (ObserverBasedRequestHandler).
 * Remaining delays are ONLY for touch automation sensitivity.
 */
public class BitwigConfig {
    /**
     * Delay after touch press before allowing parameter changes.
     * Prevents accidental parameter changes during initial touch detection.
     * Used in: DeviceController touch handling
     */
    public static final int TOUCH_PRESS_DELAY_MS = 20;

    /**
     * Grace period after touch release.
     * Blocks parameter updates to prevent automation playback re-trigger.
     * Used in: DeviceController touch handling
     */
    public static final int TOUCH_RELEASE_GRACE_MS = TOUCH_PRESS_DELAY_MS;

    private BitwigConfig() {
        // Prevent instantiation
    }
}
