#pragma once

/**
 * @brief Bitwig-specific font registry
 *
 * Icon fonts specific to Bitwig mode.
 * Text fonts are linked from CoreFonts after loading.
 */

#include <lvgl.h>
#include <oc/ui/lvgl/FontLoader.hpp>

struct BitwigFonts {
    // Icon fonts (Bitwig-specific, loaded from binaries)
    lv_font_t* icons_12 = nullptr;
    lv_font_t* icons_14 = nullptr;
    lv_font_t* icons_16 = nullptr;

    // Text fonts (linked from CoreFonts after loading)
    lv_font_t* track_label = nullptr;   // Semibold (list_item_label)
    lv_font_t* device_label = nullptr;  // Semibold (list_item_label)
    lv_font_t* page_label = nullptr;    // Regular
    lv_font_t* param_label = nullptr;   // Regular

    const lv_font_t* lvgl_symbols = nullptr;
};

extern BitwigFonts bitwig_fonts;

/// Font entry descriptors (stored in flash)
extern const oc::ui::lvgl::font::Entry BITWIG_FONT_ENTRIES[];

/// Number of bitwig font entries
extern const size_t BITWIG_FONT_COUNT;

/**
 * @brief Link text fonts to CoreFonts
 *
 * Call after core fonts are loaded to set up text font aliases.
 */
void linkBitwigFontAliases();
