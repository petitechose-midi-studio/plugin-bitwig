/**
 * ViewType.hpp - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Description: Active view on the controller
 *
 */

#pragma once

#include <cstdint>

enum class ViewType : uint8_t {
    REMOTE_CONTROLS = 0,
    MIX = 1,
    CLIP = 2,
    COUNT = 3,  // Sentinel - must be last
};

// Conversion helpers
inline ViewType toViewType(uint8_t value) {
    return static_cast<ViewType>(value);
}

inline uint8_t fromViewType(ViewType value) {
    return static_cast<uint8_t>(value);
}

// Name helper
inline const char* viewTypeName(ViewType value) {
    switch (value) {
        case ViewType::REMOTE_CONTROLS: return "Remote Controls";
        case ViewType::MIX: return "Mix";
        case ViewType::CLIP: return "Clip";
        default: return "Unknown";
    }
}
