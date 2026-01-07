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

#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Page protocol handler (Host -> State)
 *
 * Receives page-related protocol messages and updates BitwigState.
 * Also configures encoders when page changes (initial parameter setup).
 */
class PageHostHandler {
public:
    PageHostHandler(state::BitwigState& state,
                    BitwigProtocol& protocol,
                    oc::api::EncoderAPI& encoders);
    ~PageHostHandler() = default;

    // Non-copyable
    PageHostHandler(const PageHostHandler&) = delete;
    PageHostHandler& operator=(const PageHostHandler&) = delete;

private:
    void setupProtocolCallbacks();

    // Encoder configuration based on parameter types (called on page change)
    template <typename RemoteControlArray>
    void updateRemoteControlEncoderModes(const RemoteControlArray& remoteControls);

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
