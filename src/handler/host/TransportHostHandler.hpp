#pragma once

/**
 * @file TransportHostHandler.hpp
 * @brief Handles transport messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Views subscribe to state signals for automatic UI updates.
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Transport protocol handler (Host -> State)
 *
 * Receives:
 * - TransportPlayMessage
 * - TransportRecordMessage
 * - TransportStopMessage
 * - TransportTempoMessage
 *
 * Updates: state_.transport.*
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
