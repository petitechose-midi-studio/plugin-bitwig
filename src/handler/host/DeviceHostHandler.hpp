#pragma once

/**
 * @file DeviceHostHandler.hpp
 * @brief Handles device-related messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles:
 * - Device info and state changes
 * - Device list and children for selector
 *
 * @see TrackHostHandler for track-related messages
 * @see PageHostHandler for page names and changes
 * @see RemoteControlHostHandler for remote control parameter updates
 */

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Device protocol handler (Host -> State)
 *
 * Receives device info and selector messages and updates BitwigState.
 */
class DeviceHostHandler {
public:
    DeviceHostHandler(state::BitwigState& state, BitwigProtocol& protocol);
    ~DeviceHostHandler() = default;

    // Non-copyable
    DeviceHostHandler(const DeviceHostHandler&) = delete;
    DeviceHostHandler& operator=(const DeviceHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
