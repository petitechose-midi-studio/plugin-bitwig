#pragma once

/**
 * @file OverlayTypes.hpp
 * @brief Overlay type definitions for Bitwig plugin
 */

#include <cstdint>

namespace bitwig::state {

/**
 * @brief Overlay types managed by OverlayManager
 */
enum class OverlayType : uint8_t {
    NONE = 0,
    PAGE_SELECTOR,
    DEVICE_SELECTOR,
    TRACK_SELECTOR,
    VIEW_SELECTOR,
    COUNT  // Must be last
};

}  // namespace bitwig::state
