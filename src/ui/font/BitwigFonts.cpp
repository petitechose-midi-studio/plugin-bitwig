#include "BitwigFonts.hpp"

// Core's font system (provides registerFont and fonts registry)
#include <ui/font/FontLoader.hpp>

#include "data/bitwig_icons_12.c.inc"
#include "data/bitwig_icons_14.c.inc"
#include "data/bitwig_icons_16.c.inc"

// Global font registry instance for Bitwig
BitwigFonts bitwig_fonts;

void bitwigFontsRegister() {
    // Register icon fonts with core's system (queued for loading)
    ::registerFont(&bitwig_fonts.icons_12, bitwig_icons_12_bin, bitwig_icons_12_bin_len);
    ::registerFont(&bitwig_fonts.icons_14, bitwig_icons_14_bin, bitwig_icons_14_bin_len);
    ::registerFont(&bitwig_fonts.icons_16, bitwig_icons_16_bin, bitwig_icons_16_bin_len);
}

void bitwigFontsLink() {
    // Link text fonts to core's fonts after they're loaded
    bitwig_fonts.track_label = ::fonts.list_item_label;   // Semibold 14 (from core)
    bitwig_fonts.device_label = ::fonts.list_item_label;  // Semibold 14 (from core)
    bitwig_fonts.page_label = ::fonts.inter_14_regular;
    bitwig_fonts.param_label = ::fonts.inter_14_regular;
    bitwig_fonts.lvgl_symbols = &lv_font_montserrat_12;  // Built-in LVGL symbols
}
