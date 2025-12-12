#pragma once

/**
 * @file TransportHostHandler.hpp
 * @brief Handles transport messages from Bitwig → updates BitwigState
 *
 * Adapted to use BitwigState (Signal-based) instead of direct view updates.
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Transport protocol handler
 *
 * Sets up protocol callbacks to update TransportState signals.
 * Views subscribe to these signals for automatic UI updates.
 */
class TransportHostHandler {
public:
    TransportHostHandler(state::BitwigState& state, BitwigProtocol& protocol);
    ~TransportHostHandler() = default;

    // Non-copyable
    TransportHostHandler(const TransportHostHandler&) = delete;
    TransportHostHandler& operator=(const TransportHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
