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

#include <oc/api/EncoderAPI.hpp>
#include <oc/util/Index.hpp>

#include <config/App.hpp>
#include "protocol/ParameterType.hpp"
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

/**
 * @brief Configure encoder mode based on parameter type
 * @param encoders Encoder API reference
 * @param encoderId Encoder to configure
 * @param parameterType Parameter type (KNOB = continuous, else discrete)
 * @param discreteCount Number of discrete steps (for non-KNOB types)
 * @param value Initial encoder position
 */
inline void configureEncoderForParameter(oc::api::EncoderAPI& encoders,
                                         EncoderID encoderId,
                                         ParameterType parameterType,
                                         uint8_t discreteCount,
                                         float value) {
    if (parameterType == ParameterType::KNOB) {
        encoders.setContinuous(encoderId);
    } else {
        encoders.setDiscreteSteps(encoderId, discreteCount);
    }
    encoders.setPosition(encoderId, value);
}

// =============================================================================
// Index Utilities (re-exported from framework)
// =============================================================================

using oc::util::wrapIndex;
using oc::util::shouldPrefetch;

}  // namespace bitwig::handler
