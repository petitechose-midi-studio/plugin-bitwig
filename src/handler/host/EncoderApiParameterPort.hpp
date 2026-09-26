#pragma once

#include <oc/api/EncoderAPI.hpp>

#include "ParameterEncoderPort.hpp"
#include "handler/InputUtils.hpp"

namespace bitwig::handler {

/**
 * @brief Production adapter from parameter indices to macro encoders
 *
 * Owns the device-support mapping (MACRO_ENCODERS) and ignores slots without
 * a physical encoder, matching the guards previously inlined in the handlers.
 */
class EncoderApiParameterPort final : public ParameterEncoderPort {
public:
    explicit EncoderApiParameterPort(oc::api::EncoderAPI& encoders)
        : encoders_(encoders) {}

    void configure(uint8_t parameterIndex,
                   ParameterType type,
                   uint8_t discreteCount,
                   float value) override {
        auto encoderId = getEncoderIdForParameter(parameterIndex);
        if (encoderId == EncoderID{0}) return;
        configureEncoderForParameter(encoders_, encoderId, type, discreteCount, value);
    }

    void setPosition(uint8_t parameterIndex, float value) override {
        auto encoderId = getEncoderIdForParameter(parameterIndex);
        if (encoderId == EncoderID{0}) return;
        encoders_.setPosition(encoderId, value);
    }

private:
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
