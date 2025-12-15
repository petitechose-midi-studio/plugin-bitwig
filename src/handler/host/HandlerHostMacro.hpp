#pragma once

/**
 * @file HandlerHostMacro.hpp
 * @brief Handles macro parameter messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles individual parameter updates:
 * - MacroUpdate (full parameter refresh)
 * - MacroDiscreteValues (enum values update)
 * - MacroValueChange (value/display update from host)
 * - MacroNameChange (parameter name update)
 *
 * @see HandlerHostPage for bulk parameter init on page change
 * @see HandlerHostDevice for device info/list
 */

#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Macro parameter protocol handler (Host -> State)
 *
 * Receives individual parameter updates and applies them to BitwigState.
 * Also configures encoder modes and positions when parameters change.
 */
class HandlerHostMacro {
public:
    HandlerHostMacro(state::BitwigState& state,
                     BitwigProtocol& protocol,
                     oc::api::EncoderAPI& encoders);
    ~HandlerHostMacro() = default;

    // Non-copyable
    HandlerHostMacro(const HandlerHostMacro&) = delete;
    HandlerHostMacro& operator=(const HandlerHostMacro&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
