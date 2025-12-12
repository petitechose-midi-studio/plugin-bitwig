#pragma once

#include <cstdint>

#include "ui/font/icon.hpp"

namespace Bitwig::Device {

// ============================================================================
// PARAMETER CONSTANTS
// ============================================================================

constexpr uint8_t PARAMETER_COUNT = 8;

// ============================================================================
// DEVICE NAVIGATION CONSTANTS
// ============================================================================

constexpr uint8_t MAX_DEVICES = 16;
constexpr uint8_t MAX_CHILD_TYPES = 4;

// ============================================================================
// CHILD TYPE DEFINITIONS
// ============================================================================

enum class ChildType : uint8_t { NONE = 0, SLOTS = 1, LAYERS = 2, DRUMS = 4 };

inline const char *getChildTypeName(ChildType type) {
    switch (type) {
        case ChildType::SLOTS: return "[S] Slots";
        case ChildType::LAYERS: return "[L] Layers";
        case ChildType::DRUMS: return "[D] Drum Pads";
        default: return "";
    }
}

// Converts array of child types to bitflags (SLOTS|LAYERS|DRUMS)
template <typename ChildTypeArray>
inline uint8_t getChildTypeFlags(const ChildTypeArray &types) {
    uint8_t flags = 0;
    for (uint8_t type : types) {
        if (type == static_cast<uint8_t>(ChildType::NONE)) break;
        flags |= type;
    }
    return flags;
}

// ============================================================================
// PARAMETER TYPE DEFINITIONS
// ============================================================================

enum class ParameterType : uint8_t {
    KNOB = 0,    // Continuous parameter (optimistic updates)
    BUTTON = 1,  // On/Off parameter (wait for echo with displayValue)
    LIST = 2     // Discrete selection (wait for echo with displayValue)
};

// ============================================================================
// UI TEXT CONSTANTS
// ============================================================================

constexpr const char *BACK_TO_PARENT_TEXT = Icon::ARROW_LEFT;

}  // namespace Bitwig::Device
