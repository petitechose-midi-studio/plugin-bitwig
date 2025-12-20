#pragma once

/**
 * @file ParameterState.hpp
 * @brief Signal-based state for device parameters (8 macros)
 *
 * Each parameter has reactive signals for value, name, display value, etc.
 * UI components subscribe to these signals for automatic updates.
 */

#include <array>
#include <string>

#include <oc/state/Signal.hpp>
#include <oc/state/SignalString.hpp>
#include <oc/state/SignalVector.hpp>

#include "Constants.hpp"

namespace bitwig::state {

using oc::state::Signal;
using oc::state::SignalLabel;
using oc::state::SignalTiny;
using oc::state::SignalVector;

/**
 * @brief State for a single device parameter
 *
 * Contains all reactive properties for one of the 8 macro parameters.
 * Handlers update these signals, views subscribe to them.
 */
struct ParameterSlot {
    // Value and display
    Signal<float> value{0.0f};
    Signal<float> origin{0.0f};           // Bipolar center point
    Signal<float> modulationOffset{0.0f}; // Offset from value (modulatedValue = value + offset)
    Signal<bool> isModulated{false};      // True when modulation is active (automation, LFO, etc.)
    SignalLabel name;
    SignalTiny displayValue;

    // Metadata
    Signal<ParameterType> type{ParameterType::KNOB};
    Signal<int16_t> discreteCount{-1};  // -1=continuous, N=steps
    Signal<uint8_t> currentValueIndex{0};
    SignalVector<std::string, MAX_DISCRETE_VALUES> discreteValues;

    // Visibility and state
    Signal<bool> visible{true};
    Signal<bool> loading{false};
    Signal<bool> metadataSet{false};

    void reset() {
        value.set(0.0f);
        origin.set(0.0f);
        modulationOffset.set(0.0f);
        isModulated.set(false);
        name.set("");
        displayValue.set("");
        type.set(ParameterType::KNOB);
        discreteCount.set(-1);
        currentValueIndex.set(0);
        discreteValues.clear();
        visible.set(true);
        loading.set(false);
        metadataSet.set(false);
    }
};;

/**
 * @brief State for all 8 device parameters
 */
struct ParameterState {
    std::array<ParameterSlot, PARAMETER_COUNT> slots;

    ParameterSlot& operator[](size_t i) { return slots[i]; }
    const ParameterSlot& operator[](size_t i) const { return slots[i]; }

    void resetAll() {
        for (auto& slot : slots) {
            slot.reset();
        }
    }
};

}  // namespace bitwig::state
