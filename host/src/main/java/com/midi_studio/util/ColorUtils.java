package com.midi_studio.util;

import com.bitwig.extension.api.Color;

/**
 * Utility class for converting Bitwig Color objects to protocol-compatible formats
 *
 * Provides conversion methods to transform Bitwig's Color API (float RGB 0.0-1.0)
 * into formats suitable for embedded controllers (uint32 hex).
 */
public class ColorUtils {

    /**
     * Convert Bitwig Color to uint32 hex format (0xRRGGBB)
     *
     * This format is directly compatible with LVGL's lv_color_hex() function.
     *
     * @param color Bitwig Color object
     * @return 32-bit RGB value in format 0x00RRGGBB (alpha ignored)
     *
     * Example:
     *   Color red = Color.fromRGB255(255, 0, 0)
     *   toUint32Hex(red) → 0x00FF0000
     */
    public static long toUint32Hex(Color color) {
        if (color == null) {
            return 0x000000;  // Black for null colors
        }

        int r = color.getRed255();    // 0-255
        int g = color.getGreen255();  // 0-255
        int b = color.getBlue255();   // 0-255

        // Combine into 0x00RRGGBB format (24-bit RGB in 32-bit container)
        // Use long to avoid sign issues with Java's signed int
        return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
    }

    /**
     * Convert Bitwig Color to uint32 hex format with bounds checking
     *
     * Same as toUint32Hex() but validates color components are in valid range.
     *
     * @param color Bitwig Color object
     * @return 32-bit RGB value in format 0x00RRGGBB
     * @throws IllegalArgumentException if color components are out of range
     */
    public static long toUint32HexChecked(Color color) {
        if (color == null) {
            throw new IllegalArgumentException("Color cannot be null");
        }

        int r = color.getRed255();
        int g = color.getGreen255();
        int b = color.getBlue255();

        // Validate range (should always be valid from Bitwig API, but defensive)
        if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
            throw new IllegalArgumentException(
                String.format("Color components out of range: R=%d G=%d B=%d", r, g, b)
            );
        }

        return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
    }

    /**
     * Convert Bitwig Color to hex string (for debugging)
     *
     * @param color Bitwig Color object
     * @return Hex string in format "RRGGBB" (no '#' prefix)
     */
    public static String toHexString(Color color) {
        if (color == null) {
            return "000000";
        }

        long hex = toUint32Hex(color);
        return String.format("%06X", hex);
    }

    /**
     * Get a dimmed version of a color (for disabled states)
     *
     * @param color Original color
     * @param factor Dimming factor (0.0 = black, 1.0 = original)
     * @return Dimmed color as uint32 hex
     */
    public static long toDimmedUint32Hex(Color color, double factor) {
        if (color == null) {
            return 0x000000;
        }

        factor = Math.max(0.0, Math.min(1.0, factor));  // Clamp to [0, 1]

        int r = (int)(color.getRed255() * factor);
        int g = (int)(color.getGreen255() * factor);
        int b = (int)(color.getBlue255() * factor);

        return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
    }

    // Private constructor to prevent instantiation
    private ColorUtils() {
        throw new AssertionError("Utility class should not be instantiated");
    }
}
