#include "MacroInputHandler.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../protocol/struct/DeviceMacroValueChangeMessage.hpp"
#include "../../protocol/struct/DeviceMacroTouchMessage.hpp"

namespace Bitwig {

namespace {
// Shared mapping arrays for macro controls
constexpr EncoderID MACRO_ENCODERS[] = {
    EncoderID::MACRO_1, EncoderID::MACRO_2, EncoderID::MACRO_3, EncoderID::MACRO_4,
    EncoderID::MACRO_5, EncoderID::MACRO_6, EncoderID::MACRO_7, EncoderID::MACRO_8
};
constexpr ButtonID MACRO_BUTTONS[] = {
    ButtonID::MACRO_1, ButtonID::MACRO_2, ButtonID::MACRO_3, ButtonID::MACRO_4,
    ButtonID::MACRO_5, ButtonID::MACRO_6, ButtonID::MACRO_7, ButtonID::MACRO_8
};
} // namespace

// =============================================================================
// Construction / Destruction
// =============================================================================

MacroInputHandler::MacroInputHandler(ControllerAPI& api, DeviceView& view,
                                     Protocol::Protocol& protocol, lv_obj_t* scope)
    : api_(api), view_(view), protocol_(protocol), scope_(scope)
{
    setupInputBindings();
}

MacroInputHandler::~MacroInputHandler() = default;

// =============================================================================
// Static API
// =============================================================================

EncoderID MacroInputHandler::getEncoderIdForParameter(uint8_t paramIndex) {
    return (paramIndex < Device::PARAMETER_COUNT) ? MACRO_ENCODERS[paramIndex] : EncoderID(0);
}

// =============================================================================
// Bindings Setup
// =============================================================================

void MacroInputHandler::setupInputBindings() {
    for (uint8_t i = 0; i < Device::PARAMETER_COUNT; i++) {
        api_.onTurned(MACRO_ENCODERS[i], [this, i](float v) { handleValueChange(i, v); }, scope_);
        api_.onPressed(MACRO_BUTTONS[i], [this, i]() { sendTouch(i, true); }, scope_);
        api_.onReleased(MACRO_BUTTONS[i], [this, i]() { sendTouch(i, false); }, scope_);
    }
}

// =============================================================================
// Handlers
// =============================================================================

void MacroInputHandler::handleValueChange(uint8_t index, float value) {
    if (index >= Device::PARAMETER_COUNT)
        return;

    view_.state().parameters[index].value = value;
    view_.state().dirty.parameters[index] = true;
    view_.sync();
    protocol_.send(Protocol::DeviceMacroValueChangeMessage{index, value, "", false});
}

void MacroInputHandler::sendTouch(uint8_t index, bool touched) {
    if (index >= Device::PARAMETER_COUNT)
        return;

    protocol_.send(Protocol::DeviceMacroTouchMessage{index, touched});
}

} // namespace Bitwig
