#pragma once

/**
 * @file HandlerHostPage.hpp
 * @brief Handles page-related messages from Bitwig -> updates BitwigState
 *
 * HostHandler pattern: Protocol callbacks -> State updates
 * Handles:
 * - Page names for selector
 * - Page changes (initializes all 8 parameter slots)
 *
 * @see HandlerHostDevice for device list/info
 * @see HandlerHostMacro for individual parameter updates
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
class HandlerHostPage {
public:
    HandlerHostPage(state::BitwigState& state,
                    BitwigProtocol& protocol,
                    oc::api::EncoderAPI& encoders);
    ~HandlerHostPage() = default;

    // Non-copyable
    HandlerHostPage(const HandlerHostPage&) = delete;
    HandlerHostPage& operator=(const HandlerHostPage&) = delete;

private:
    void setupProtocolCallbacks();

    // Encoder configuration based on parameter types (called on page change)
    template <typename MacroArray>
    void updateMacroEncoderModes(const MacroArray& macros);

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
};

}  // namespace bitwig::handler
