#pragma once

/**
 * @file RemoteControlHostHandler.hpp
 * @brief Handles remote control parameter messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles individual parameter updates:
 * - RemoteControlUpdate (full parameter refresh)
 * - RemoteControlDiscreteValues (enum values update)
 * - RemoteControlValueChange (value/display update from host)
 * - RemoteControlNameChange (parameter name update)
 * - RemoteControlModulatedValueChange (modulation offset for ribbon)
 *
 * @see PageHostHandler for bulk parameter init on page change
 * @see DeviceHostHandler for device info/list
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
class RemoteControlHostHandler {
public:
    RemoteControlHostHandler(state::BitwigState& state,
                             BitwigProtocol& protocol,
                             oc::api::EncoderAPI& encoders);
    ~RemoteControlHostHandler() = default;

    // Non-copyable
    RemoteControlHostHandler(const RemoteControlHostHandler&) = delete;
    RemoteControlHostHandler& operator=(const RemoteControlHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
