#include "LastClickedInputHandler.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include <config/App.hpp>

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using EncoderID = Config::EncoderID;

LastClickedInputHandler::LastClickedInputHandler(state::BitwigState& state,
                                                 BitwigProtocol& protocol,
                                                 oc::api::EncoderAPI& encoders,
                                                 lv_obj_t* scopeElement)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , scope_element_(scopeElement) {
    setupBindings();
}

void LastClickedInputHandler::setupBindings() {
    // Scoped binding to deviceView
    encoders_.encoder(EncoderID::OPT).turn().scope(scope(scope_element_)).then([this](float value) {
        handleEncoderTurn(value);
    });
}

void LastClickedInputHandler::handleEncoderTurn(float value) {
    // Optimistic update
    state_.lastClicked.value.set(value);

    // Send to host using explicit method
    protocol_.lastClickedValue(value);
}

}  // namespace bitwig::handler
