// Auto-generated | 24 icons | 2025-12-02
#pragma once
#include "FontLoader.hpp"

#include <lvgl.h>

namespace Icon {
enum class Size : uint8_t { S = 12, M = 14, L = 16 };

constexpr const char* ARROW_LEFT = "\xEE\x80\x80";
constexpr const char* AUDIO_DEVICES = "\xEE\x80\x81";
constexpr const char* CHANNEL_LIST = "\xEE\x80\x82";
constexpr const char* DEVICE_AUDIO = "\xEE\x80\x83";
constexpr const char* DEVICE_INSTRUMENT = "\xEE\x80\x84";
constexpr const char* DEVICE_NOTE = "\xEE\x80\x85";
constexpr const char* DEVICE_OFF = "\xEE\x80\x86";
constexpr const char* DEVICE_ON = "\xEE\x80\x87";
constexpr const char* DEVICE_VIEW = "\xEE\x80\x88";
constexpr const char* DIRECTORY = "\xEE\x80\x89";
constexpr const char* DRUM_PAD_MATRIX = "\xEE\x80\x8A";
constexpr const char* LAYER = "\xEE\x80\x8B";
constexpr const char* MASTER_TRACK = "\xEE\x80\x8C";
constexpr const char* MUTE = "\xEE\x80\x8D";
constexpr const char* RETURN_TRACK = "\xEE\x80\x8E";
constexpr const char* SLIDER = "\xEE\x80\x8F";
constexpr const char* SOLO = "\xEE\x80\x90";
constexpr const char* TRACK_AUDIO = "\xEE\x80\x91";
constexpr const char* TRACK_HYBRID = "\xEE\x80\x92";
constexpr const char* TRACK_INSTRUMENT = "\xEE\x80\x93";
constexpr const char* TRACK_LIST = "\xEE\x80\x94";
constexpr const char* TRANSPORT_PLAY = "\xEE\x80\x95";
constexpr const char* TRANSPORT_RECORD = "\xEE\x80\x96";
constexpr const char* TRANSPORT_STOP = "\xEE\x80\x97";

inline void set(lv_obj_t* label, const char* icon, Size size = Size::M) {
    lv_font_t* font = (size == Size::S)   ? bitwig_fonts.icons_12
                      : (size == Size::M) ? bitwig_fonts.icons_14
                                          : bitwig_fonts.icons_16;
    lv_obj_set_style_text_font(label, font, 0);
    lv_label_set_text(label, icon);
}
}  // namespace Icon