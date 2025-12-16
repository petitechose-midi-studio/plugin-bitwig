#pragma once

/**
 * @file HandlerHostRemoteControl.hpp
 * @brief Handles remote control parameter messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles individual parameter updates:
 * - RemoteControlUpdate (full parameter refresh)
 * - RemoteControlDiscreteValues (enum values update)
 * - RemoteControlValueChange (value/display update from host)
 * - RemoteControlNameChange (parameter name update)
 *
 * @see HandlerHostPage for bulk parameter init on page change
 * @see HandlerHostDevice for device info/list
 */

#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Remote control parameter protocol handler (Host -> State)
 *
 * Receives individual parameter updates and applies them to BitwigState.
 * Also configures encoder modes and positions when parameters change.
 */
class HandlerHostRemoteControl {
public:
    HandlerHostRemoteControl(state::BitwigState& state,
                             BitwigProtocol& protocol,
                             oc::api::EncoderAPI& encoders);
    ~HandlerHostRemoteControl() = default;

    // Non-copyable
    HandlerHostRemoteControl(const HandlerHostRemoteControl&) = delete;
    HandlerHostRemoteControl& operator=(const HandlerHostRemoteControl&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
