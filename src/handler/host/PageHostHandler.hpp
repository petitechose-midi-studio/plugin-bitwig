#pragma once

/**
 * @file PageHostHandler.hpp
 * @brief Handles page-related messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles:
 * - Page names for selector
 * - Page changes (initializes all 8 parameter slots)
 *
 * @see DeviceHostHandler for device list/info
 * @see RemoteControlHostHandler for individual parameter updates
 */

#include "ParameterEncoderPort.hpp"
#include "protocol/ProtocolCallbacks.hpp"
#include "state/DeviceInfoState.hpp"
#include "state/ParameterState.hpp"
#include "state/SelectorState.hpp"

namespace bitwig::handler {

/**
 * @brief Page protocol handler (Host -> State)
 *
 * Receives page-related protocol messages and updates BitwigState.
 * Also configures encoders when page changes (initial parameter setup).
 */
class PageHostHandler {
public:
    PageHostHandler(state::DeviceInfoState& device,
                    state::ParameterState& parameters,
                    state::PageSelectorState& pageSelector,
                    Protocol::ProtocolCallbacks& protocol,
                    ParameterEncoderPort& encoders);
    ~PageHostHandler() = default;

    // Non-copyable
    PageHostHandler(const PageHostHandler&) = delete;
    PageHostHandler& operator=(const PageHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    state::DeviceInfoState& device_;
    state::ParameterState& parameters_;
    state::PageSelectorState& pageSelector_;
    Protocol::ProtocolCallbacks& protocol_;
    ParameterEncoderPort& encoders_;
};

}  // namespace bitwig::handler
