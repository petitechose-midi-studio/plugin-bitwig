/**
 * ParameterType.hpp - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Description: UI hint for parameter rendering
 *
 */

#pragma once

#include <cstdint>

enum class ParameterType : uint8_t {
    KNOB = 0,
    BUTTON = 1,
    LIST = 2,
};

// Conversion helpers
inline ParameterType toParameterType(uint8_t value) {
    return static_cast<ParameterType>(value);
}

inline uint8_t fromParameterType(ParameterType value) {
    return static_cast<uint8_t>(value);
}
