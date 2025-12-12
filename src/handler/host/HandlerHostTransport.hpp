#pragma once

/**
 * @file HandlerHostTransport.hpp
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
class HandlerHostTransport {
public:
    HandlerHostTransport(state::BitwigState& state, BitwigProtocol& protocol);
    ~HandlerHostTransport() = default;

    // Non-copyable
    HandlerHostTransport(const HandlerHostTransport&) = delete;
    HandlerHostTransport& operator=(const HandlerHostTransport&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
