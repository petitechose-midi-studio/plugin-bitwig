#pragma once

/**
 * @file LastClickedInputHandler.hpp
 * @brief Handles last clicked parameter input -> sends protocol messages
 *
 * InputHandler pattern:
 * - OPT encoder turn: adjust value of last clicked parameter in Bitwig
 *
 * The "last clicked" parameter is the most recently clicked parameter in Bitwig's UI.
 * This allows quick adjustment without navigating to a specific device/page.
 */

#include <lvgl.h>

#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Last clicked parameter input handler (Input -> Protocol)
 */
class LastClickedInputHandler {
public:
    LastClickedInputHandler(state::BitwigState& state,
                            BitwigProtocol& protocol,
                            oc::api::EncoderAPI& encoders,
                            lv_obj_t* scopeElement);

    ~LastClickedInputHandler() = default;

    // Non-copyable
    LastClickedInputHandler(const LastClickedInputHandler&) = delete;
    LastClickedInputHandler& operator=(const LastClickedInputHandler&) = delete;

private:
    void setupBindings();
    void handleEncoderTurn(float value);

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    lv_obj_t* scope_element_;
};

}  // namespace bitwig::handler
