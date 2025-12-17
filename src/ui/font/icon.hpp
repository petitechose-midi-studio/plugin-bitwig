// Auto-generated | 28 icons | 2025-12-17
#pragma once
#include "BitwigFonts.hpp"

#include <lvgl.h>

namespace Icon {
enum class Size : uint8_t { S = 12, M = 14, L = 16 };

    constexpr const char* BROWSER_AUDIO_DEVICES = "\xEE\x80\x80";
    constexpr const char* BROWSER_DIRECTORY = "\xEE\x80\x81";
    constexpr const char* BROWSER_LAYER = "\xEE\x80\x82";
    constexpr const char* CHANNEL_LIST = "\xEE\x80\x83";
    constexpr const char* CHANNEL_MUTE = "\xEE\x80\x84";
    constexpr const char* CHANNEL_SOLO = "\xEE\x80\x85";
    constexpr const char* DEVICE_AUDIO = "\xEE\x80\x86";
    constexpr const char* DEVICE_DRUM_PAD = "\xEE\x80\x87";
    constexpr const char* DEVICE_INSTRUMENT = "\xEE\x80\x88";
    constexpr const char* DEVICE_NOTE = "\xEE\x80\x89";
    constexpr const char* DEVICE_OFF = "\xEE\x80\x8A";
    constexpr const char* DEVICE_ON = "\xEE\x80\x8B";
    constexpr const char* DEVICE_VIEW = "\xEE\x80\x8C";
    constexpr const char* TRACK_AUDIO = "\xEE\x80\x8D";
    constexpr const char* TRACK_HYBRID = "\xEE\x80\x8E";
    constexpr const char* TRACK_INSTRUMENT = "\xEE\x80\x8F";
    constexpr const char* TRACK_LIST = "\xEE\x80\x90";
    constexpr const char* TRACK_MASTER = "\xEE\x80\x91";
    constexpr const char* TRACK_RETURN = "\xEE\x80\x92";
    constexpr const char* TRANSPORT_AUTOMATION = "\xEE\x80\x93";
    constexpr const char* TRANSPORT_AUTOMATION_ABORT = "\xEE\x80\x94";
    constexpr const char* TRANSPORT_OVERDUB = "\xEE\x80\x95";
    constexpr const char* TRANSPORT_OVERDUB_LAUNCHER = "\xEE\x80\x96";
    constexpr const char* TRANSPORT_PLAY = "\xEE\x80\x97";
    constexpr const char* TRANSPORT_RECORD = "\xEE\x80\x98";
    constexpr const char* TRANSPORT_STOP = "\xEE\x80\x99";
    constexpr const char* UI_ARROW_LEFT = "\xEE\x80\x9A";
    constexpr const char* UI_SLIDER = "\xEE\x80\x9B";

inline void set(lv_obj_t* label, const char* icon, Size size = Size::M) {
    lv_font_t* font = (size == Size::S) ? bitwig_fonts.icons_12
                        : (size == Size::M) ? bitwig_fonts.icons_14
                        : bitwig_fonts.icons_16;
    lv_obj_set_style_text_font(label, font, 0);
    lv_label_set_text(label, icon);
}
}  // namespace Icon