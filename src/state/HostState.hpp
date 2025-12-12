#pragma once

/**
 * @file HostState.hpp
 * @brief Signal-based state for DAW host connection
 */

#include <oc/state/Signal.hpp>
#include <oc/state/SignalString.hpp>

namespace bitwig::state {

using oc::state::Signal;
using oc::state::SignalLabel;

/**
 * @brief State for DAW host connection
 *
 * Tracks whether Bitwig is connected and responding.
 * Used to switch between connected/disconnected UI modes.
 */
struct HostState {
    Signal<bool> connected{false};
    SignalLabel hostName;
    SignalLabel apiVersion;

    HostState() {
        hostName.set("Bitwig Studio");
    }

    void reset() {
        connected.set(false);
        apiVersion.set("");
    }
};

}  // namespace bitwig::state
