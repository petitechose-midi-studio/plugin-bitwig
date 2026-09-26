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

#include <string>

#include "protocol/BitwigProtocol.hpp"
#include "state/DeviceInfoState.hpp"
#include "state/ParameterState.hpp"
#include "state/SelectorState.hpp"

namespace bitwig::handler {

/**
 * @brief Device protocol handler (Host -> State)
 *
 * Receives device info and selector messages and updates BitwigState.
 */
class DeviceHostHandler {
public:
    DeviceHostHandler(state::DeviceInfoState& device, state::ParameterState& parameters,
                      state::PageSelectorState& pageSelector,
                      state::DeviceSelectorState& deviceSelector, BitwigProtocol& protocol,
                      const char* backToParentLabel);
    ~DeviceHostHandler() = default;

    // Non-copyable
    DeviceHostHandler(const DeviceHostHandler&) = delete;
    DeviceHostHandler& operator=(const DeviceHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::DeviceInfoState& device_;
    state::ParameterState& parameters_;
    state::PageSelectorState& pageSelector_;
    state::DeviceSelectorState& deviceSelector_;
    BitwigProtocol& protocol_;
    const std::string backToParentLabel_;
};

}  // namespace bitwig::handler
