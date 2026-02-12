#include "BitwigFonts.hpp"

#include <config/PlatformCompat.hpp>
#include <ms/ui/font/CoreFonts.hpp>

#include "data/bitwig_icons_12.c.inc"
#include "data/bitwig_icons_14.c.inc"
#include "data/bitwig_icons_16.c.inc"

BitwigFonts bitwig_fonts;

const oc::ui::lvgl::font::Entry BITWIG_FONT_ENTRIES[] = {
    {&bitwig_fonts.icons_12, bitwig_icons_12_bin,
     bitwig_icons_12_bin_len, "BitwigIcons12", false},
    {&bitwig_fonts.icons_14, bitwig_icons_14_bin,
     bitwig_icons_14_bin_len, "BitwigIcons14", false},
    {&bitwig_fonts.icons_16, bitwig_icons_16_bin,
     bitwig_icons_16_bin_len, "BitwigIcons16", false},
};

const size_t BITWIG_FONT_COUNT =
    sizeof(BITWIG_FONT_ENTRIES) / sizeof(BITWIG_FONT_ENTRIES[0]);

void linkBitwigFontAliases() {
    // Link text fonts to core fonts
    bitwig_fonts.track_label = fonts.list_item_label;
    bitwig_fonts.device_label = fonts.list_item_label;
    bitwig_fonts.page_label = fonts.inter_14_regular;
    bitwig_fonts.param_label = fonts.inter_14_regular;
    bitwig_fonts.lvgl_symbols = &lv_font_montserrat_12;
}
