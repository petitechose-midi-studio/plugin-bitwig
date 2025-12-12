#pragma once

#include <cstdint>

namespace bitwig::state {

// =============================================================================
// Capacity Constants
// =============================================================================

constexpr uint8_t PARAMETER_COUNT = 8;
constexpr uint8_t MAX_DEVICES = 16;
constexpr uint8_t MAX_TRACKS = 16;
constexpr uint8_t MAX_PAGES = 8;
constexpr uint8_t MAX_DISCRETE_VALUES = 16;
constexpr uint8_t MAX_CHILDREN = 8;

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
