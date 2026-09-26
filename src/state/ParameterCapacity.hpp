#pragma once

#include <cstdint>

/**
 * @file ParameterCapacity.hpp
 * @brief Capacity constants shared by parameter state and protocol handlers
 *
 * Kept free of UI includes so parameter state and its host handlers can be
 * exercised without LVGL. Constants.hpp re-exports these names for existing
 * call sites.
 */

namespace bitwig::state {

constexpr uint8_t PARAMETER_COUNT = 8;
constexpr uint8_t MAX_DISCRETE_VALUES = 16;

}  // namespace bitwig::state
