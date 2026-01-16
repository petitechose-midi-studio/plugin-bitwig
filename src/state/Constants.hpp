#pragma once

#include <cstdint>

// IWYU pragma: begin_exports
#include "protocol/ChildType.hpp"
#include "protocol/DeviceType.hpp"
#include "protocol/ParameterType.hpp"
#include "protocol/TrackType.hpp"
// IWYU pragma: end_exports

#include "ui/font/BitwigIcons.hpp"

namespace bitwig::state {

namespace icons = bitwig::icons;

// =============================================================================
// Capacity Constants
// =============================================================================

constexpr uint8_t PARAMETER_COUNT = 8;
constexpr uint8_t MAX_DEVICES = 64;  // Increased for windowed loading (was 16)
constexpr uint8_t MAX_TRACKS = 64;   // Increased for windowed loading (was 32)
constexpr uint8_t MAX_PAGES = 128;   // Increased for windowed loading (was 8)
constexpr uint8_t MAX_DISCRETE_VALUES = 16;
constexpr uint8_t MAX_CHILDREN = 8;
constexpr uint8_t MAX_CHILD_TYPES = 4;

// =============================================================================
// Windowed List Constants
// =============================================================================

constexpr uint8_t LIST_WINDOW_SIZE = 16;     // Items per window request (devices, tracks, pages)
constexpr uint8_t PREFETCH_THRESHOLD = 8;    // Prefetch when cursor >= loadedUpTo - threshold

// =============================================================================
// UI Text Constants
// =============================================================================

constexpr const char* BACK_TO_PARENT_TEXT = icons::UI_ARROW_LEFT;

// =============================================================================
// Child Type Helpers
// =============================================================================

inline const char* getChildTypeName(uint8_t type) {
    if (type == CHILD_TYPE_SLOTS) return "[S] Slots";
    if (type == CHILD_TYPE_LAYERS) return "[L] Layers";
    if (type == CHILD_TYPE_DRUMS) return "[D] Drum Pads";
    return "";
}

// Converts array of child types to bitflags (SLOTS|LAYERS|DRUMS)
template <typename ChildTypeArray>
inline uint8_t getChildTypeFlags(const ChildTypeArray& types) {
    uint8_t flags = 0;
    for (uint8_t type : types) {
        if (type == CHILD_TYPE_NONE) break;
        flags |= type;
    }
    return flags;
}

}  // namespace bitwig::state
