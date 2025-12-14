#include "HandlerInputLastClicked.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "protocol/struct/LastClickedValueChangeMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using EncoderID = Config::EncoderID;

HandlerInputLastClicked::HandlerInputLastClicked(state::BitwigState& state,
                                                 BitwigProtocol& protocol,
                                                 oc::api::EncoderAPI& encoders,
                                                 lv_obj_t* scopeElement)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , scopeElement_(scopeElement) {
    setupBindings();
}

void HandlerInputLastClicked::setupBindings() {
    encoders_.encoder(EncoderID::OPT)
        .turn()
        .scope(scope(scopeElement_))
        .then([this](float value) { handleEncoderTurn(value); });
}

void HandlerInputLastClicked::handleEncoderTurn(float value) {
    if (!state_.lastClicked.exists.get()) return;

    // Optimistic update
    state_.lastClicked.value.set(value);

    // Send to host
    protocol_.send(Protocol::LastClickedValueChangeMessage{value, "", false});
}

}  // namespace bitwig::handler
