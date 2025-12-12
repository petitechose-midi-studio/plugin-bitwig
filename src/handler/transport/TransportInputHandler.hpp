#pragma once

/**
 * @file TransportInputHandler.hpp
 * @brief Transport input actions → sends protocol messages
 *
 * Helper class for transport input handling.
 * BitwigContext sets up the actual input bindings and calls these methods.
 *
 * Note: Input bindings (onButton, onEncoder) are set up in BitwigContext
 * because IContext's binding methods are protected.
 */

#include "protocol/BitwigProtocol.hpp"

namespace bitwig::handler {

/**
 * @brief Transport input helper
 *
 * Sends transport commands to Bitwig via protocol.
 * State updates come back via TransportHostHandler → BitwigState.
 */
class TransportInputHandler {
public:
    explicit TransportInputHandler(BitwigProtocol& protocol);
    ~TransportInputHandler() = default;

    // Non-copyable
    TransportInputHandler(const TransportInputHandler&) = delete;
    TransportInputHandler& operator=(const TransportInputHandler&) = delete;

    // =========================================================================
    // Actions (called by BitwigContext input bindings)
    // =========================================================================

    /// Toggle play/pause
    void togglePlay();

    /// Toggle record
    void toggleRecord();

    /// Stop transport
    void stop();

    /// Adjust tempo by delta (relative)
    void adjustTempo(float delta);

private:
    BitwigProtocol& protocol_;
};

}  // namespace bitwig::handler
