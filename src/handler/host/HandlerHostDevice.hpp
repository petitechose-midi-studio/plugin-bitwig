#pragma once

/**
 * @file HandlerHostDevice.hpp
 * @brief Handles device-related messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles:
 * - Device info and state changes
 * - Device list and children for selector
 *
 * @see HandlerHostTrack for track-related messages
 * @see HandlerHostPage for page names and changes
 * @see HandlerHostMacro for macro parameter updates
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Device protocol handler (Host -> State)
 *
 * Receives device info and selector messages and updates BitwigState.
 */
class HandlerHostDevice {
public:
    HandlerHostDevice(state::BitwigState& state, BitwigProtocol& protocol);
    ~HandlerHostDevice() = default;

    // Non-copyable
    HandlerHostDevice(const HandlerHostDevice&) = delete;
    HandlerHostDevice& operator=(const HandlerHostDevice&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
