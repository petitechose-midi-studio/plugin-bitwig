#pragma once

#include <cstdint>

#include "theme/BaseTheme.hpp"

namespace Theme {

namespace Color {
using namespace BaseTheme::Color;

constexpr uint32_t MACRO_1 = MACRO_1_RED;
constexpr uint32_t MACRO_2 = MACRO_2_ORANGE;
constexpr uint32_t MACRO_3 = MACRO_3_YELLOW;
constexpr uint32_t MACRO_4 = MACRO_4_GREEN;
constexpr uint32_t MACRO_5 = MACRO_5_CYAN;
constexpr uint32_t MACRO_6 = MACRO_6_BLUE;
constexpr uint32_t MACRO_7 = MACRO_7_PURPLE;
constexpr uint32_t MACRO_8 = MACRO_8_PINK;

constexpr uint32_t BACKGROUND_FILL = BACKGROUND;
constexpr uint32_t DATA_INACTIVE = INACTIVE;
constexpr uint32_t DATA_ACTIVE = ACTIVE;
constexpr uint32_t TEXT_LIGHT = TEXT_SECONDARY;
constexpr uint32_t TEXT_DARK = TEXT_PRIMARY_INVERTED;

constexpr uint32_t DEVICE_KNOB_INACTIVE = KNOB_BACKGROUND;
constexpr uint32_t KNOB_VALUE_RIBBON = KNOB_TRACK;
constexpr uint32_t KNOB_VALUE_INDICATOR = KNOB_VALUE;
constexpr uint32_t MACRO_TEXT = TEXT_LIGHT;

constexpr uint32_t STATUS_WARNING = 0x00AA00;
constexpr uint32_t ARC_SUCCESS = 0x00FF88;

constexpr uint32_t DEVICE_SLOT = 0x4A7BA7;      // Dark blue for slots
constexpr uint32_t DEVICE_LAYER = 0xB8A52A;     // Dark yellow/gold for layers
constexpr uint32_t DEVICE_DRUM_PAD = 0xD97634;  // Orange for drum pads

constexpr uint32_t DEVICE_STATE_ENABLED = 0x5FDB80;   // Green-cyan (more saturated, less blue than MACRO_5)
constexpr uint32_t DEVICE_STATE_DISABLED = STATUS_INACTIVE; // Gray (disabled device)

constexpr uint32_t TRACK_MUTE = 0xFF8800;  // Orange for mute indicator
constexpr uint32_t TRACK_SOLO = 0xFFDD00;  // Yellow for solo indicator
}  // namespace Color
}  // namespace Theme
