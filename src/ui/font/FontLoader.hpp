#pragma once

#include <lvgl.h>

/**
 * @brief Bitwig-specific font registry
 * Contains fonts used only by Bitwig plugin components
 */
struct BitwigFonts {
    lv_font_t* icons = nullptr;                   // Bitwig icon font (14px)
    lv_font_t* device_label = nullptr;            // DeviceStateBar, SplashView
    lv_font_t* page_label = nullptr;              // DeviceStateBar
    const lv_font_t* lvgl_symbols = nullptr;      // LVGL built-in symbols
};

extern BitwigFonts bitwig_fonts;

/**
 * @brief Load all Bitwig-specific fonts from binary data
 * Must be called after LVGL initialization
 */
void load_bitwig_fonts();

/**
 * @brief Free all Bitwig-specific fonts
 * Call before shutdown
 */
void free_bitwig_fonts();
