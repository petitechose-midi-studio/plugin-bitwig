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
};

// Conversion helpers
inline ViewType toViewType(uint8_t value) {
    return static_cast<ViewType>(value);
}

inline uint8_t fromViewType(ViewType value) {
    return static_cast<uint8_t>(value);
}
