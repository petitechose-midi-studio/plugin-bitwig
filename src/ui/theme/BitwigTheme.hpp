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

// Device type colors (Bitwig palette)
constexpr uint32_t DEVICE_TYPE_AUDIO = 0xDD5B33;      // Orange-red for audio effects
constexpr uint32_t DEVICE_TYPE_INSTRUMENT = 0xF9C91D; // Yellow for instruments
constexpr uint32_t DEVICE_TYPE_NOTE = 0x32B1DC;       // Blue for note effects

constexpr uint32_t TRACK_MUTE = 0xFF8800;  // Orange for mute indicator
constexpr uint32_t TRACK_SOLO = 0xFFDD00;  // Yellow for solo indicator
}  // namespace Color

// =============================================================================
// Layout constants - Screen: 320x240
// =============================================================================
namespace Layout {

// Bar heights
constexpr int16_t TOP_BAR_HEIGHT = 20;
constexpr int16_t TRANSPORT_BAR_HEIGHT = 20;
constexpr int16_t HINT_BAR_HEIGHT = 20;

// Widget dimensions (4x2 grid in body)
constexpr int16_t WIDGET_WIDTH = 80;
constexpr int16_t WIDGET_HEIGHT = 100;

// Component sizes
constexpr int16_t LEVEL_BAR_WIDTH = 60;
constexpr int16_t COLOR_BAR_WIDTH = 4;
constexpr int16_t INDICATOR_SIZE = 12;

// Padding scale (4px base unit)
constexpr int16_t PAD_XS = 2;
constexpr int16_t PAD_SM = 4;
constexpr int16_t PAD_MD = 6;
constexpr int16_t PAD_LG = 10;
constexpr int16_t PAD_XL = 12;

// Gaps (for flex/grid)
constexpr int16_t GAP_SM = 4;
constexpr int16_t GAP_MD = 6;
constexpr int16_t GAP_LG = 10;

}  // namespace Layout

// =============================================================================
// Opacity constants - Semantic opacity levels
// =============================================================================
namespace Opacity {

constexpr uint8_t FULL = 255;        // LV_OPA_COVER - fully visible
constexpr uint8_t SUBTLE = 178;      // ~70% - secondary icons, hints
constexpr uint8_t DIMMED = 127;      // ~50% - disabled elements
constexpr uint8_t FADED = 102;       // ~40% - inactive but visible
constexpr uint8_t HINT = 51;         // ~20% - very subtle background hints
constexpr uint8_t HIDDEN = 0;        // LV_OPA_TRANSP - invisible

}  // namespace Opacity

}  // namespace Theme
