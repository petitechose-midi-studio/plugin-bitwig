#pragma once

#include <cstdint>

#include <oc/ui/lvgl/theme/BaseTheme.hpp>

namespace bitwig::theme {

// Import base theme for convenience
using namespace oc::ui::lvgl;

namespace Color {

constexpr uint32_t BACKGROUND_BASE = 0x000000;

constexpr uint32_t MACRO_1 = BaseTheme::Color::MACRO_1_RED;
constexpr uint32_t MACRO_2 = BaseTheme::Color::MACRO_2_ORANGE;
constexpr uint32_t MACRO_3 = BaseTheme::Color::MACRO_3_YELLOW;
constexpr uint32_t MACRO_4 = BaseTheme::Color::MACRO_4_GREEN;
constexpr uint32_t MACRO_5 = BaseTheme::Color::MACRO_5_CYAN;
constexpr uint32_t MACRO_6 = BaseTheme::Color::MACRO_6_BLUE;
constexpr uint32_t MACRO_7 = BaseTheme::Color::MACRO_7_PURPLE;
constexpr uint32_t MACRO_8 = BaseTheme::Color::MACRO_8_PINK;

constexpr uint32_t BACKGROUND_FILL = BaseTheme::Color::BACKGROUND;
constexpr uint32_t DATA_INACTIVE = BaseTheme::Color::INACTIVE;
constexpr uint32_t DATA_ACTIVE = BaseTheme::Color::ACTIVE;
constexpr uint32_t TEXT_LIGHT = BaseTheme::Color::TEXT_SECONDARY;
constexpr uint32_t TEXT_DARK = BaseTheme::Color::TEXT_PRIMARY_INVERTED;

// Re-export base theme colors for convenience
constexpr uint32_t TEXT_PRIMARY = BaseTheme::Color::TEXT_PRIMARY;
constexpr uint32_t INACTIVE_LIGHTER = BaseTheme::Color::INACTIVE_LIGHTER;
constexpr uint32_t KNOB_BACKGROUND = BaseTheme::Color::KNOB_BACKGROUND;

constexpr uint32_t DEVICE_KNOB_INACTIVE = BaseTheme::Color::KNOB_BACKGROUND;
constexpr uint32_t KNOB_VALUE_RIBBON = BaseTheme::Color::KNOB_TRACK;
constexpr uint32_t KNOB_VALUE_INDICATOR = BaseTheme::Color::KNOB_VALUE;
constexpr uint32_t MACRO_TEXT = TEXT_LIGHT;

constexpr uint32_t STATUS_WARNING = 0x00AA00;
constexpr uint32_t ARC_SUCCESS = 0x00FF88;

constexpr uint32_t DEVICE_SLOT = 0x4A7BA7;      // Dark blue for slots
constexpr uint32_t DEVICE_LAYER = 0xB8A52A;     // Dark yellow/gold for layers
constexpr uint32_t DEVICE_DRUM_PAD = 0xD97634;  // Orange for drum pads

constexpr uint32_t DEVICE_STATE_ENABLED =
    0x5FDB80;  // Green-cyan (more saturated, less blue than MACRO_5)
constexpr uint32_t DEVICE_STATE_DISABLED =
    BaseTheme::Color::STATUS_INACTIVE;  // Gray (disabled device)

// Device type colors (Bitwig palette)
constexpr uint32_t DEVICE_TYPE_AUDIO = 0xDD5B33;       // Orange-red for audio effects
constexpr uint32_t DEVICE_TYPE_INSTRUMENT = 0xF9C91D;  // Yellow for instruments
constexpr uint32_t DEVICE_TYPE_NOTE = 0x32B1DC;        // Blue for note effects

constexpr uint32_t TRACK_MUTE = 0xFF8800;  // Orange for mute indicator
constexpr uint32_t TRACK_SOLO = 0xFFDD00;  // Yellow for solo indicator

// Automation colors
constexpr uint32_t AUTOMATION_INDICATOR = 0xFF5555;  // Bright red - parameter has automation
constexpr uint32_t AUTOMATION_OVERRIDE = 0x6FE569;   // Green - automation override active (Bitwig green)
constexpr uint32_t AUTOMATION_VALUE = 0x992222;      // Dark red - knob value arc when following
constexpr uint32_t AUTOMATION_TRACK = 0xFF4444;      // Bright red - knob track arc when following

}  // namespace Color

// =============================================================================
// Layout constants - Screen: 320x240
// =============================================================================
namespace Layout {

// Bar heights
constexpr int16_t TOP_BAR_HEIGHT = 20;
constexpr int16_t TRANSPORT_BAR_HEIGHT = 20;
constexpr int16_t HINT_BAR_HEIGHT = 20;

// Parameter grid (4x2 layout in body)
constexpr int16_t PARAMETER_GRID_COLS = 4;
constexpr int16_t PARAMETER_GRID_ROWS = 2;

// Widget dimensions
constexpr int16_t WIDGET_WIDTH = 80;
constexpr int16_t WIDGET_HEIGHT = 100;

// Component sizes
constexpr int16_t LEVEL_BAR_WIDTH = 60;
constexpr int16_t COLOR_BAR_WIDTH = 4;
constexpr int16_t INDICATOR_SIZE = 12;
constexpr int16_t AUTOMATION_INDICATOR_SIZE = 8;  // Automation LED size (smaller)

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

// Overlay/Selector header padding
constexpr int16_t OVERLAY_PAD_H = 16;       // Horizontal padding for overlay headers
constexpr int16_t OVERLAY_PAD_TOP = 8;      // Top padding for overlay headers
constexpr int16_t OVERLAY_PAD_BOTTOM = 4;   // Bottom padding for overlay headers
constexpr int16_t OVERLAY_HEADER_HEIGHT = 20;  // Fixed height for overlay headers

// HintBar edge padding
constexpr int16_t HINT_BAR_EDGE_PAD = 24;   // Padding from screen edges for hint cells

}  // namespace Layout

// =============================================================================
// Opacity constants - Semantic opacity levels
// =============================================================================
namespace Opacity {

constexpr uint8_t FULL = 255;        // LV_OPA_COVER - fully visible
constexpr uint8_t OVERLAY_BG = 230;  // ~90% - modal overlay backgrounds
constexpr uint8_t SUBTLE = 178;      // ~70% - secondary icons, hints
constexpr uint8_t DIMMED = 127;      // ~50% - disabled elements
constexpr uint8_t FADED = 102;       // ~40% - inactive but visible
constexpr uint8_t SCROLLBAR = 77;    // ~30% - scrollbar track
constexpr uint8_t HINT = 51;         // ~20% - very subtle background hints
constexpr uint8_t HIDDEN = 0;        // LV_OPA_TRANSP - invisible

}  // namespace Opacity

// =============================================================================
// Animation timing constants
// =============================================================================
namespace Animation {

constexpr uint32_t FADE_MS = 300;  // Standard fade duration

}  // namespace Animation

}  // namespace bitwig::theme
