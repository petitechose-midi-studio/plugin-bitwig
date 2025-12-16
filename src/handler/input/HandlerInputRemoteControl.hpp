#pragma once

/**
 * @file HandlerInputRemoteControl.hpp
 * @brief Handles remote control encoder input -> sends protocol messages
 *
 * InputHandler pattern:
 * - Receives APIs from context
 * - Defines its own bindings in constructor
 * - Sends protocol messages on input events
 * - Does optimistic state updates
 *
 * Controls the 8 remote control parameter encoders:
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
 * @brief Remote control encoder input handler (Input -> Protocol)
 */
class HandlerInputRemoteControl {
public:
    HandlerInputRemoteControl(state::BitwigState& state,
                              BitwigProtocol& protocol,
                              oc::api::EncoderAPI& encoders,
                              oc::api::ButtonAPI& buttons,
                              lv_obj_t* scopeElement);

    ~HandlerInputRemoteControl() = default;

    // Non-copyable
    HandlerInputRemoteControl(const HandlerInputRemoteControl&) = delete;
    HandlerInputRemoteControl& operator=(const HandlerInputRemoteControl&) = delete;

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
