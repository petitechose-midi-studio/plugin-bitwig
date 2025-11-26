#include "FontLoader.hpp"

#include <Arduino.h>

// Binary font data
#include "data/bitwig_icons_14.c.inc"
#include "data/interdisplay_medium_14_4bpp.c.inc"
#include "data/interdisplay_light_14_light_4bpp.c.inc"

BitwigFonts bitwig_fonts;

void load_bitwig_fonts() {
    // Icon font
    bitwig_fonts.icons = lv_binfont_create_from_buffer(
        (void*)bitwig_icons_14_bin,
        bitwig_icons_14_bin_len
    );

    // Text fonts
    bitwig_fonts.device_label = lv_binfont_create_from_buffer(
        (void*)interdisplay_medium_14_4bpp_bin,
        interdisplay_medium_14_4bpp_bin_len
    );

    bitwig_fonts.page_label = lv_binfont_create_from_buffer(
        (void*)interdisplay_light_14_light_4bpp_bin,
        interdisplay_light_14_light_4bpp_bin_len
    );

    // LVGL built-in symbols
    bitwig_fonts.lvgl_symbols = &lv_font_montserrat_12;
}

void free_bitwig_fonts() {
    if (bitwig_fonts.icons) {
        lv_binfont_destroy(bitwig_fonts.icons);
        bitwig_fonts.icons = nullptr;
    }
    if (bitwig_fonts.device_label) {
        lv_binfont_destroy(bitwig_fonts.device_label);
        bitwig_fonts.device_label = nullptr;
    }
    if (bitwig_fonts.page_label) {
        lv_binfont_destroy(bitwig_fonts.page_label);
        bitwig_fonts.page_label = nullptr;
    }
}
