#pragma once

/**
 * @file InputUtils.hpp
 * @brief Shared utilities for input handlers
 *
 * Contains common functions and constants used across multiple handlers
 * to avoid code duplication.
 */

#include <cstdint>

#include <oc/api/EncoderAPI.hpp>
#include <oc/util/Index.hpp>

#include <ms/device_support/v1/ControlLayout.hpp>
#include "protocol/ParameterType.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

// =============================================================================
// Encoder Mapping
// =============================================================================

using EncoderID = ms::device_support::v1::EncoderID;
using ButtonID = ms::device_support::v1::ButtonID;

using ms::device_support::v1::control::MACRO_BUTTONS;
using ms::device_support::v1::control::MACRO_ENCODERS;

static_assert(MACRO_ENCODERS.size() == bitwig::state::PARAMETER_COUNT);
static_assert(MACRO_BUTTONS.size() == bitwig::state::PARAMETER_COUNT);

/**
 * @brief Get encoder ID for a parameter index
 * @param paramIndex Parameter slot index (0-7)
 * @return Encoder ID or EncoderID{0} if invalid
 */
inline EncoderID getEncoderIdForParameter(uint8_t paramIndex) {
    return (paramIndex < MACRO_ENCODERS.size())
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
