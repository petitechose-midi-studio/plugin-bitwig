#pragma once

/**
 * @file DeviceInfoState.hpp
 * @brief Signal-based state for current device info
 */

#include <oc/state/Signal.hpp>
#include <oc/state/SignalString.hpp>

#include "Constants.hpp"

namespace bitwig::state {

using oc::state::Signal;
using oc::state::SignalLabel;
using oc::state::SignalString;

/**
 * @brief State for the currently selected device
 */
struct DeviceInfoState {
    SignalString name;
    SignalLabel pageName;
    Signal<DeviceType> deviceType{DeviceType::UNKNOWN};
    Signal<bool> enabled{false};
    Signal<bool> hasChildren{false};

    DeviceInfoState() {
        name.set("No Device");
    }

    void reset() {
        name.set("No Device");
        pageName.set("");
        deviceType.set(DeviceType::UNKNOWN);
        enabled.set(false);
        hasChildren.set(false);
    }
};

}  // namespace bitwig::state
