#pragma once

#include <lvgl.h>

/**
 * @brief Bitwig-specific font registry
 * Fonts are registered via core's register_font() in loadResources()
 *
 * All fonts are Inter Display 14px:
 * - track_label: Bold (track name in top bar)
 * - device_label: Semibold (device name in top bar)
 * - page_label: Regular (page name in top bar)
 * - param_label: Regular (parameter names)
 */
struct BitwigFonts {
    // Icon fonts (Bitwig-specific)
    lv_font_t* icons_12 = nullptr;
    lv_font_t* icons_14 = nullptr;
    lv_font_t* icons_18 = nullptr;

    // Text fonts (Inter Display 14px from core)
    lv_font_t* track_label = nullptr;   // Bold
    lv_font_t* device_label = nullptr;  // Semibold
    lv_font_t* page_label = nullptr;    // Regular
    lv_font_t* param_label = nullptr;   // Regular (same as page_label)

    const lv_font_t* lvgl_symbols = nullptr;
};

extern BitwigFonts bitwig_fonts;
