#pragma once

/**
 * @file LastClickedState.hpp
 * @brief Signal-based state for Bitwig's last clicked parameter
 */

#include <oc/state/Signal.hpp>
#include <oc/state/SignalString.hpp>

#include "protocol/ParameterType.hpp"

namespace bitwig::state {

using oc::state::Signal;
using oc::state::SignalLabel;
using oc::state::SignalTiny;

/**
 * @brief State for Bitwig's last clicked parameter
 *
 * When user clicks a parameter in Bitwig, this gets updated.
 * Allows the controller to control any parameter via a dedicated encoder.
 */
struct LastClickedState {
    SignalLabel name;
    Signal<float> value{0.0f};
    SignalTiny displayValue;
    Signal<ParameterType> parameterType{ParameterType::KNOB};
    Signal<int16_t> discreteCount{-1};  // -1=continuous, N=steps
    Signal<bool> exists{false};

    void reset() {
        name.set("");
        value.set(0.0f);
        displayValue.set("");
        parameterType.set(ParameterType::KNOB);
        discreteCount.set(-1);
        exists.set(false);
    }
};

}  // namespace bitwig::state
