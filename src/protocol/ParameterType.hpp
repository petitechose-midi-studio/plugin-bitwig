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
    COUNT = 3,  // Sentinel - must be last
};

// Conversion helpers
inline ParameterType toParameterType(uint8_t value) {
    return static_cast<ParameterType>(value);
}

inline uint8_t fromParameterType(ParameterType value) {
    return static_cast<uint8_t>(value);
}

// Name helper
inline const char* parameterTypeName(ParameterType value) {
    switch (value) {
        case ParameterType::KNOB: return "Knob";
        case ParameterType::BUTTON: return "Button";
        case ParameterType::LIST: return "List";
        default: return "Unknown";
    }
}
