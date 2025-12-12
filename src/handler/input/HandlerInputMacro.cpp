#include "HandlerInputMacro.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/DeviceConstants.hpp"
#include "protocol/struct/DeviceMacroTouchMessage.hpp"
#include "protocol/struct/DeviceMacroValueChangeMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using namespace Bitwig::Device;
using Config::EncoderID;
using Config::ButtonID;

namespace {
constexpr EncoderID MACRO_ENCODERS[] = {
    EncoderID::MACRO_1, EncoderID::MACRO_2, EncoderID::MACRO_3, EncoderID::MACRO_4,
    EncoderID::MACRO_5, EncoderID::MACRO_6, EncoderID::MACRO_7, EncoderID::MACRO_8
};

constexpr ButtonID MACRO_BUTTONS[] = {
    ButtonID::MACRO_1, ButtonID::MACRO_2, ButtonID::MACRO_3, ButtonID::MACRO_4,
    ButtonID::MACRO_5, ButtonID::MACRO_6, ButtonID::MACRO_7, ButtonID::MACRO_8
};
}  // namespace

HandlerInputMacro::HandlerInputMacro(state::BitwigState& state,
                                     BitwigProtocol& protocol,
                                     oc::api::EncoderAPI& encoders,
                                     oc::api::ButtonAPI& buttons,
                                     lv_obj_t* scopeElement)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , buttons_(buttons)
    , scopeElement_(scopeElement) {
    setupBindings();
}

EncoderID HandlerInputMacro::getEncoderIdForParameter(uint8_t paramIndex) {
    return (paramIndex < PARAMETER_COUNT) ? MACRO_ENCODERS[paramIndex] : EncoderID{0};
}

void HandlerInputMacro::setupBindings() {
    for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
        // Encoder turn -> value change
        encoders_.encoder(MACRO_ENCODERS[i])
            .turn()
            .scope(scope(scopeElement_))
            .then([this, i](float v) { handleValueChange(i, v); });

        // Button press -> touch start
        buttons_.button(MACRO_BUTTONS[i])
            .press()
            .scope(scope(scopeElement_))
            .then([this, i]() { sendTouch(i, true); });

        // Button release -> touch end
        buttons_.button(MACRO_BUTTONS[i])
            .release()
            .scope(scope(scopeElement_))
            .then([this, i]() { sendTouch(i, false); });
    }
}

void HandlerInputMacro::handleValueChange(uint8_t index, float value) {
    if (index >= PARAMETER_COUNT) return;

    // Optimistic update
    state_.parameters.slots[index].value.set(value);

    // Send to host
    protocol_.send(Protocol::DeviceMacroValueChangeMessage{index, value, "", false});
}

void HandlerInputMacro::sendTouch(uint8_t index, bool touched) {
    if (index >= PARAMETER_COUNT) return;

    protocol_.send(Protocol::DeviceMacroTouchMessage{index, touched});
}

}  // namespace bitwig::handler
