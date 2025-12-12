#pragma once

/**
 * @file PluginLifecycleHandler.hpp
 * @brief Handles host connection/disconnection → updates BitwigState
 *
 * Adapted to use BitwigState (Signal-based) instead of ViewManager.
 * BitwigContext uses state_.host.connected to determine isConnected().
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Host lifecycle handler
 *
 * Sets up protocol callbacks for HostInitialized/HostDeactivated.
 * Updates state_.host.connected which BitwigContext uses for isConnected().
 */
class PluginLifecycleHandler {
public:
    PluginLifecycleHandler(state::BitwigState& state, BitwigProtocol& protocol);
    ~PluginLifecycleHandler() = default;

    // Non-copyable
    PluginLifecycleHandler(const PluginLifecycleHandler&) = delete;
    PluginLifecycleHandler& operator=(const PluginLifecycleHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
