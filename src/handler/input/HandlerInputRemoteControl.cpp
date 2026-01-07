#include "HandlerInputRemoteControl.hpp"

#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "handler/InputUtils.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using namespace bitwig::state;

HandlerInputRemoteControl::HandlerInputRemoteControl(state::BitwigState& state,
                                                     BitwigProtocol& protocol,
                                                     core::api::InputAPI input,
                                                     lv_obj_t* scopeElement)
    : state_(state)
    , protocol_(protocol)
    , input_(input)
    , scopeElement_(scopeElement) {
    setupBindings();
}

void HandlerInputRemoteControl::setupBindings() {
    for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
        // Encoder turn -> value change
        input_.encoders.encoder(MACRO_ENCODERS[i])
            .turn()
            .scope(scope(scopeElement_))
            .then([this, i](float v) { handleValueChange(i, v); });

        // Button press -> touch start
        input_.buttons.button(MACRO_BUTTONS[i])
            .press()
            .scope(scope(scopeElement_))
            .then([this, i]() { sendTouch(i, true); });

        // Button release -> touch end
        input_.buttons.button(MACRO_BUTTONS[i])
            .release()
            .scope(scope(scopeElement_))
            .then([this, i]() { sendTouch(i, false); });
    }

    // NAV button double tap -> restore automation for all parameters
    input_.buttons.button(Config::ButtonID::NAV)
        .doubleTap()
        .scope(scope(scopeElement_))
        .then([this]() { handleGlobalRestore(); });
}

void HandlerInputRemoteControl::handleValueChange(uint8_t index, float value) {
    if (index >= PARAMETER_COUNT) return;

    auto& slot = state_.parameters.slots[index];

    // Optimistic update for value
    slot.value.set(value);

    // Hide modulation ribbon immediately when user moves the knob
    // This prevents visual latency while waiting for host roundtrip
    slot.isModulated.set(false);

    // Note: automationActive is now derived from host batch (touchedMask)
    // Host is source of truth to avoid race conditions

    // Send to host using explicit method
    protocol_.remoteControlValue(index, value);
}

void HandlerInputRemoteControl::sendTouch(uint8_t index, bool touched) {
    if (index >= PARAMETER_COUNT) return;

    protocol_.deviceRemoteControlTouch(index, touched);
}

void HandlerInputRemoteControl::handleGlobalRestore() {
    // Request host to restore automation for all parameters
    // Host will set value = modulatedValue and update touchedMask
    // automationActive will be updated via next batch (host is source of truth)
    for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
        protocol_.deviceRemoteControlRestoreAutomation(i);
    }
}

}  // namespace bitwig::handler
