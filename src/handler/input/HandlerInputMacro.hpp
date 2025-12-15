#pragma once

/**
 * @file HandlerInputMacro.hpp
 * @brief Handles macro encoder input -> sends protocol messages
 *
 * InputHandler pattern:
 * - Receives APIs from context
 * - Defines its own bindings in constructor
 * - Sends protocol messages on input events
 * - Does optimistic state updates
 *
 * Controls the 8 macro parameter encoders:
 * - Encoder turns: value changes (optimistic UI + protocol)
 * - Encoder buttons: touch messages for automation
 */

#include <lvgl.h>

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>

#include "config/App.hpp"
#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Macro encoder input handler (Input -> Protocol)
 */
class HandlerInputMacro {
public:
    HandlerInputMacro(state::BitwigState& state,
                      BitwigProtocol& protocol,
                      oc::api::EncoderAPI& encoders,
                      oc::api::ButtonAPI& buttons,
                      lv_obj_t* scopeElement);

    ~HandlerInputMacro() = default;

    // Non-copyable
    HandlerInputMacro(const HandlerInputMacro&) = delete;
    HandlerInputMacro& operator=(const HandlerInputMacro&) = delete;

private:
    void setupBindings();
    void handleValueChange(uint8_t index, float value);
    void sendTouch(uint8_t index, bool touched);

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    oc::api::ButtonAPI& buttons_;
    lv_obj_t* scopeElement_;
};

}  // namespace bitwig::handler
