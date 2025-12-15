#pragma once

/**
 * @file HandlerHostTrack.hpp
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
class HandlerHostTrack {
public:
    HandlerHostTrack(state::BitwigState& state, BitwigProtocol& protocol);
    ~HandlerHostTrack() = default;

    // Non-copyable
    HandlerHostTrack(const HandlerHostTrack&) = delete;
    HandlerHostTrack& operator=(const HandlerHostTrack&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
