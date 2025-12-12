#pragma once

/**
 * @file HandlerInputTransport.hpp
 * @brief Handles transport input -> sends protocol messages
 *
 * InputHandler pattern:
 * - Receives APIs from context
 * - Defines its own bindings in constructor
 * - Sends protocol messages on input events
 * - Optionally does optimistic state updates
 *
 * Button mappings (when transport bar visible):
 * - BOTTOM_LEFT: Toggle play/pause
 * - BOTTOM_CENTER: Stop
 * - BOTTOM_RIGHT: Toggle record
 * - NAV encoder: Adjust tempo (relative delta)
 */

#include <lvgl.h>

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Transport input handler (Input -> Protocol)
 *
 * Bindings are scoped to the provided LVGL element for automatic cleanup.
 */
class HandlerInputTransport {
public:
    /**
     * @brief Construct and set up transport input bindings
     *
     * Transport bindings are global (no scope) - they are the lowest priority
     * and only fire if no scoped binding captures the input first.
     *
     * @param state BitwigState for optimistic updates
     * @param protocol BitwigProtocol for sending commands
     * @param encoders EncoderAPI for encoder bindings
     * @param buttons ButtonAPI for button bindings
     */
    HandlerInputTransport(state::BitwigState& state,
                          BitwigProtocol& protocol,
                          oc::api::EncoderAPI& encoders,
                          oc::api::ButtonAPI& buttons);

    ~HandlerInputTransport() = default;

    // Non-copyable
    HandlerInputTransport(const HandlerInputTransport&) = delete;
    HandlerInputTransport& operator=(const HandlerInputTransport&) = delete;

private:
    void setupBindings();

    void togglePlay();
    void stop();
    void toggleRecord();
    void adjustTempo(float delta);

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    oc::api::ButtonAPI& buttons_;
};

}  // namespace bitwig::handler
