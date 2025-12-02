#include "LastClickedInputHandler.hpp"

namespace Bitwig {

LastClickedInputHandler::LastClickedInputHandler(ControllerAPI& api, Protocol::Protocol& protocol,
                                                 LastClickedState& state, lv_obj_t* scope)
    : api_(api), protocol_(protocol), state_(state), scope_(scope) {
    setupInputBindings();
}

LastClickedInputHandler::~LastClickedInputHandler() { api_.clearScope(scope_); }

void LastClickedInputHandler::setupInputBindings() {
    api_.onTurned(EncoderID::OPT, [this](float value) { handleEncoderTurn(value); }, scope_);
}

void LastClickedInputHandler::handleEncoderTurn(float value) {
    if (!state_.exists) { return; }

    state_.value = value;

    protocol_.send(Protocol::LastClickedValueChangeMessage{value, "", false});
}

}  // namespace Bitwig
