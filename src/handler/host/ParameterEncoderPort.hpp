#pragma once

#include <cstdint>

#include "protocol/ParameterType.hpp"

namespace bitwig::handler {

/**
 * @file ParameterEncoderPort.hpp
 * @brief Narrow encoder output used by the parameter host handlers
 *
 * Expressed in parameter indices instead of physical encoder IDs so the
 * remote-control update rules can be tested with a recording double.
 * EncoderApiParameterPort maps the indices to macro encoders in production.
 */
class ParameterEncoderPort {
public:
    virtual ~ParameterEncoderPort() = default;

    /**
     * @brief Apply mode + position for a parameter slot
     *
     * Mirrors the historical configureEncoderForParameter sequence:
     * continuous or discrete mode, then the normalized value.
     */
    virtual void configure(uint8_t parameterIndex,
                           ParameterType type,
                           uint8_t discreteCount,
                           float value) = 0;

    /// Apply the normalized position only
    virtual void setPosition(uint8_t parameterIndex, float value) = 0;
};

}  // namespace bitwig::handler
