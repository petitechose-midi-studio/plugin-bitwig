#pragma once

/**
 * @file InputUtils.hpp
 * @brief Shared utilities for input handlers
 *
 * Contains common functions and constants used across multiple handlers
 * to avoid code duplication.
 */

#include <array>
#include <cstdint>

#include "config/App.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

// =============================================================================
// Encoder Mapping
// =============================================================================

using EncoderID = Config::EncoderID;
using ButtonID = Config::ButtonID;

/**
 * @brief Macro encoder IDs in slot order (0-7)
 */
inline constexpr std::array<EncoderID, bitwig::state::PARAMETER_COUNT> MACRO_ENCODERS = {
    EncoderID::MACRO_1, EncoderID::MACRO_2, EncoderID::MACRO_3, EncoderID::MACRO_4,
    EncoderID::MACRO_5, EncoderID::MACRO_6, EncoderID::MACRO_7, EncoderID::MACRO_8
};

/**
 * @brief Macro button IDs in slot order (0-7)
 */
inline constexpr std::array<ButtonID, bitwig::state::PARAMETER_COUNT> MACRO_BUTTONS = {
    ButtonID::MACRO_1, ButtonID::MACRO_2, ButtonID::MACRO_3, ButtonID::MACRO_4,
    ButtonID::MACRO_5, ButtonID::MACRO_6, ButtonID::MACRO_7, ButtonID::MACRO_8
};

/**
 * @brief Get encoder ID for a parameter index
 * @param paramIndex Parameter slot index (0-7)
 * @return Encoder ID or EncoderID{0} if invalid
 */
inline EncoderID getEncoderIdForParameter(uint8_t paramIndex) {
    return (paramIndex < bitwig::state::PARAMETER_COUNT)
        ? MACRO_ENCODERS[paramIndex]
        : EncoderID{0};
}

// =============================================================================
// Index Utilities
// =============================================================================

/**
 * @brief Wrap index within range [0, modulo) with proper negative handling
 * @param value Index to wrap
 * @param modulo Range size (must be > 0)
 * @return Wrapped index
 */
inline int wrapIndex(int value, int modulo) {
    return ((value % modulo) + modulo) % modulo;
}

/**
 * @brief Adjust selector index for nested mode (back button at index 0)
 * @param selectorIndex Visual index in the selector
 * @param isNested Whether the selector is showing nested items with back button
 * @return Actual item index (selectorIndex - 1 if nested, else selectorIndex)
 */
inline int adjustIndexForNested(int selectorIndex, bool isNested) {
    return isNested ? selectorIndex - 1 : selectorIndex;
}

}  // namespace bitwig::handler
