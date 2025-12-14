#pragma once

#include <lvgl.h>

/**
 * @brief Bitwig-specific font registry
 * Fonts are registered via core's register_font() in loadResources()
 *
 * All fonts are Inter Display 14px:
 * - track_label: Semibold (track name - linked to core's list_item_label)
 * - device_label: Semibold (device name - linked to core's list_item_label)
 * - page_label: Regular (page name in top bar)
 * - param_label: Regular (parameter names)
 */
struct BitwigFonts {
    // Icon fonts (Bitwig-specific)
    lv_font_t* icons_12 = nullptr;
    lv_font_t* icons_14 = nullptr;
    lv_font_t* icons_16 = nullptr;

    // Text fonts (Inter Display 14px from core)
    lv_font_t* track_label = nullptr;   // Semibold (list_item_label)
    lv_font_t* device_label = nullptr;  // Semibold (list_item_label)
    lv_font_t* page_label = nullptr;    // Regular
    lv_font_t* param_label = nullptr;   // Regular (same as page_label)

    const lv_font_t* lvgl_symbols = nullptr;
};

extern BitwigFonts bitwig_fonts;

// Register Bitwig icon fonts with core's font system
void bitwigFontsRegister();

// Link text fonts to core's loaded fonts (call after fontsLoadEssential/loadPluginFonts)
void bitwigFontsLink();
