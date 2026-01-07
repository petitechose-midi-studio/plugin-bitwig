#pragma once

/**
 * @file TrackHostHandler.hpp
 * @brief Handles track-related messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles:
 * - Track info changes (name, color, type)
 * - Track list for selector
 * - Mute/Solo state updates
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Track protocol handler (Host -> State)
 *
 * Receives track-related protocol messages and updates BitwigState.
 */
class TrackHostHandler {
public:
    TrackHostHandler(state::BitwigState& state, BitwigProtocol& protocol);
    ~TrackHostHandler() = default;

    // Non-copyable
    TrackHostHandler(const TrackHostHandler&) = delete;
    TrackHostHandler& operator=(const TrackHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
