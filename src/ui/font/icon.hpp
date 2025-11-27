// Auto-generated | 20 icons | 2025-11-27
#pragma once
#include <lvgl.h>
#include "FontLoader.hpp"

namespace Icon {
    enum Size { S12 = 12, S14 = 14, S18 = 18 };

    constexpr const char* ARROW_LEFT = "\xEE\x80\x80";
    constexpr const char* AUTOMATION_ABORT = "\xEE\x80\x81";
    constexpr const char* DEVICE_OFF = "\xEE\x80\x82";
    constexpr const char* DEVICE_ON = "\xEE\x80\x83";
    constexpr const char* DIRECTORY = "\xEE\x80\x84";
    constexpr const char* DRUM_PAD_MATRIX = "\xEE\x80\x85";
    constexpr const char* LAYER = "\xEE\x80\x86";
    constexpr const char* MASTER_TRACK = "\xEE\x80\x87";
    constexpr const char* MUTE = "\xEE\x80\x88";
    constexpr const char* PAGE_DEVICE = "\xEE\x80\x89";
    constexpr const char* PAGE_PRESET = "\xEE\x80\x8A";
    constexpr const char* SLIDER = "\xEE\x80\x8B";
    constexpr const char* SOLO = "\xEE\x80\x8C";
    constexpr const char* TRACK_AUDIO = "\xEE\x80\x8D";
    constexpr const char* TRACK_HYBRID = "\xEE\x80\x8E";
    constexpr const char* TRACK_INSTRUMENT = "\xEE\x80\x8F";
    constexpr const char* TRACK_LIST = "\xEE\x80\x90";
    constexpr const char* TRANSPORT_PLAY = "\xEE\x80\x91";
    constexpr const char* TRANSPORT_RECORD = "\xEE\x80\x92";
    constexpr const char* TRANSPORT_STOP = "\xEE\x80\x93";

    inline void set(lv_obj_t* label, const char* icon, Size size = S14) {
        lv_font_t* font = (size == S12) ? bitwig_fonts.icons_12
                        : (size == S14) ? bitwig_fonts.icons_14
                        : bitwig_fonts.icons_18;
        lv_obj_set_style_text_font(label, font, 0);
        lv_label_set_text(label, icon);
    }
}  // namespace Icon