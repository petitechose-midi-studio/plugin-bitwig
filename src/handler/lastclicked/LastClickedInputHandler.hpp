#pragma once

#include "LastClickedState.hpp"

#include "api/ControllerAPI.hpp"
#include "protocol/Protocol.hpp"

namespace Bitwig {

/**
 * @brief Input handler for LastClicked parameter
 *
 * Handles user input and sends commands to host:
 * - OPT encoder turn: Send LAST_CLICKED_VALUE_CHANGE
 * - OPT encoder press: Send LAST_CLICKED_TOUCH (automation)
 *
 * Reads shared state to determine current parameter configuration.
 */
class LastClickedInputHandler {
public:
    LastClickedInputHandler(ControllerAPI& api, Protocol::Protocol& protocol,
                            LastClickedState& state, lv_obj_t* scope);
    ~LastClickedInputHandler();
private:
    void setupInputBindings();
    void handleEncoderTurn(float value);

    ControllerAPI& api_;
    Protocol::Protocol& protocol_;
    LastClickedState& state_;
    lv_obj_t* scope_;
};

}  // namespace Bitwig
