#pragma once

/**
 * @file LastClickedHostHandler.hpp
 * @brief Handles LastClicked parameter messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 *
 * Receives:
 * - LAST_CLICKED_UPDATE: Parameter info when user clicks in Bitwig
 * - LAST_CLICKED_VALUE_CHANGE: Echo with display value after value sent
 *
 * Also configures OPT encoder based on parameter type.
 */

#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief LastClicked protocol handler (Host -> State)
 */
class LastClickedHostHandler {
public:
    LastClickedHostHandler(state::BitwigState& state,
                           BitwigProtocol& protocol,
                           oc::api::EncoderAPI& encoders);
    ~LastClickedHostHandler() = default;

    // Non-copyable
    LastClickedHostHandler(const LastClickedHostHandler&) = delete;
    LastClickedHostHandler& operator=(const LastClickedHostHandler&) = delete;

private:
    void setupProtocolCallbacks();
    void handleLastClickedUpdate(const Protocol::LastClickedUpdateMessage& msg);
    void handleLastClickedValueState(const Protocol::LastClickedValueStateMessage& msg);

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
