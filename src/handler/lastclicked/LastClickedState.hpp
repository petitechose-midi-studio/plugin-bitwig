#pragma once

#include <cstdint>

#include <string>

#include "handler/device/DeviceConstants.hpp"

namespace Bitwig {

/**
 * @brief Shared state for LastClicked parameter handling
 *
 * Holds the current state of Bitwig's last clicked parameter.
 * Shared between LastClickedHostHandler (receives updates) and
 * LastClickedInputHandler (sends value changes).
 */
struct LastClickedState {
    std::string name;
    float value = 0.0f;
    std::string displayValue;
    uint8_t parameterType = static_cast<uint8_t>(Device::ParameterType::KNOB);
    int16_t discreteCount = -1;  // -1=continuous, N=steps
    bool exists = false;

    void reset() {
        name.clear();
        value = 0.0f;
        displayValue.clear();
        parameterType = static_cast<uint8_t>(Device::ParameterType::KNOB);
        discreteCount = -1;
        exists = false;
    }
};

}  // namespace Bitwig
