#pragma once

/**
 * @file RemoteControlHostHandler.hpp
 * @brief Handles remote control parameter messages from Bitwig -> updates ParameterState
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

#include "ParameterEncoderPort.hpp"
#include "protocol/ProtocolCallbacks.hpp"
#include "state/ParameterState.hpp"

namespace bitwig::handler {

/**
 * @brief Remote control parameter protocol handler (Host -> State)
 *
 * Receives individual parameter updates and applies them to the parameter
 * slots. Encoder side effects go through ParameterEncoderPort, so the update
 * rules stay testable without the OpenControl input graph.
 */
class RemoteControlHostHandler {
public:
    RemoteControlHostHandler(state::ParameterState& parameters,
                             Protocol::ProtocolCallbacks& protocol,
                             ParameterEncoderPort& encoders);
    ~RemoteControlHostHandler() = default;

    // Non-copyable
    RemoteControlHostHandler(const RemoteControlHostHandler&) = delete;
    RemoteControlHostHandler& operator=(const RemoteControlHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::ParameterState& parameters_;
    Protocol::ProtocolCallbacks& protocol_;
    ParameterEncoderPort& encoders_;
};

}  // namespace bitwig::handler
