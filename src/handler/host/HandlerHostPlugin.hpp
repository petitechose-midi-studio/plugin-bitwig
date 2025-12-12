#pragma once

/**
 * @file HandlerHostPlugin.hpp
 * @brief Handles host connection/disconnection -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * BitwigContext uses state_.host.connected to determine isConnected().
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Host lifecycle handler (Host -> State)
 *
 * Receives:
 * - HostInitializedMessage
 * - HostDeactivatedMessage
 *
 * Updates: state_.host.connected
 * Side effects: Requests initial data on connect, resets state on disconnect
 */
class HandlerHostPlugin {
public:
    HandlerHostPlugin(state::BitwigState& state, BitwigProtocol& protocol);
    ~HandlerHostPlugin() = default;

    // Non-copyable
    HandlerHostPlugin(const HandlerHostPlugin&) = delete;
    HandlerHostPlugin& operator=(const HandlerHostPlugin&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
