#pragma once

#include <cstdint>

#include "ui/font/icon.hpp"

namespace bitwig::state {

// =============================================================================
// Capacity Constants
// =============================================================================

constexpr uint8_t PARAMETER_COUNT = 8;
constexpr uint8_t MAX_DEVICES = 16;
constexpr uint8_t MAX_TRACKS = 32;  // Fixed: was 16, must match protocol capacity
constexpr uint8_t MAX_PAGES = 8;
constexpr uint8_t MAX_DISCRETE_VALUES = 16;
constexpr uint8_t MAX_CHILDREN = 8;
constexpr uint8_t MAX_CHILD_TYPES = 4;

// =============================================================================
// UI Text Constants
// =============================================================================

constexpr const char* BACK_TO_PARENT_TEXT = Icon::UI_ARROW_LEFT;

// =============================================================================
// Device Types
// =============================================================================

enum class DeviceType : uint8_t {
    UNKNOWN = 0,
    AUDIO = 1,
    INSTRUMENT = 2,
    NOTE = 3
};

// =============================================================================
// Parameter Types
// =============================================================================

enum class ParameterType : uint8_t {
    KNOB = 0,    // Continuous parameter (optimistic updates)
    BUTTON = 1,  // On/Off parameter (wait for echo)
    LIST = 2     // Discrete selection (wait for echo)
};

// =============================================================================
// Child Types (for device navigation)
// =============================================================================

enum class ChildType : uint8_t {
    NONE = 0,
    SLOTS = 1,
    LAYERS = 2,
    DRUMS = 4
};

inline const char* getChildTypeName(ChildType type) {
    switch (type) {
        case ChildType::SLOTS: return "[S] Slots";
        case ChildType::LAYERS: return "[L] Layers";
        case ChildType::DRUMS: return "[D] Drum Pads";
        default: return "";
    }
}

// Converts array of child types to bitflags (SLOTS|LAYERS|DRUMS)
template <typename ChildTypeArray>
inline uint8_t getChildTypeFlags(const ChildTypeArray& types) {
    uint8_t flags = 0;
    for (uint8_t type : types) {
        if (type == static_cast<uint8_t>(ChildType::NONE)) break;
        flags |= type;
    }
    return flags;
}

// =============================================================================
// Track Types
// =============================================================================

enum class TrackType : uint8_t {
    UNKNOWN = 0,
    AUDIO = 1,
    INSTRUMENT = 2,
    HYBRID = 3,
    GROUP = 4,
    EFFECT = 5,
    MASTER = 6
};

}  // namespace bitwig::state
