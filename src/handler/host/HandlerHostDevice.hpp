#pragma once

/**
 * @file HandlerHostDevice.hpp
 * @brief Handles device-related messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * This is the most complex HostHandler, handling:
 * - Device info and state changes
 * - Device/track/page lists for selectors
 * - Macro parameter updates (values, names, types, discrete values)
 *
 * Also configures encoders based on parameter types (continuous/discrete).
 */

#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Device protocol handler (Host -> State)
 *
 * Receives all device-related protocol messages and updates BitwigState.
 * Configures encoder modes based on parameter types.
 */
class HandlerHostDevice {
public:
    HandlerHostDevice(state::BitwigState& state,
                      BitwigProtocol& protocol,
                      oc::api::EncoderAPI& encoders);
    ~HandlerHostDevice() = default;

    // Non-copyable
    HandlerHostDevice(const HandlerHostDevice&) = delete;
    HandlerHostDevice& operator=(const HandlerHostDevice&) = delete;

private:
    void setupProtocolCallbacks();

    // Encoder configuration based on parameter type
    template <typename MacroArray>
    void updateMacroEncoderModes(const MacroArray& macros);

    // Index adjustments for nested navigation
    int toDeviceDisplayIndex(int rawIndex) const;
    int toTrackDisplayIndex(int rawIndex) const;

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
