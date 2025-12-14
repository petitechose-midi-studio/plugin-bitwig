#include "HandlerInputDevicePage.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "protocol/struct/DevicePageSelectByIndexMessage.hpp"
#include "protocol/struct/RequestDevicePageNamesMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;

namespace {
int wrapIndex(int value, int modulo) {
    return ((value % modulo) + modulo) % modulo;
}
}  // namespace

HandlerInputDevicePage::HandlerInputDevicePage(state::BitwigState& state,
                                               BitwigProtocol& protocol,
                                               oc::api::EncoderAPI& encoders,
                                               oc::api::ButtonAPI& buttons,
                                               lv_obj_t* scopeElement,
                                               lv_obj_t* overlayElement)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , buttons_(buttons)
    , scopeElement_(scopeElement)
    , overlayElement_(overlayElement) {
    setupBindings();
}

void HandlerInputDevicePage::setupBindings() {
    // === VIEW-LEVEL BINDINGS (scopeElement_) ===

    // Open page selector (latch behavior)
    buttons_.button(ButtonID::LEFT_BOTTOM)
        .press()
        .latch()
        .scope(scope(scopeElement_))
        .then([this]() { requestPageList(); });

    // Close page selector on release
    buttons_.button(ButtonID::LEFT_BOTTOM)
        .release()
        .scope(scope(scopeElement_))
        .then([this]() { closeSelector(); });

    // Navigate pages (while LEFT_BOTTOM pressed)
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .when(buttons_.pressed(ButtonID::LEFT_BOTTOM))
        .scope(scope(scopeElement_))
        .then([this](float delta) { navigate(delta); });

    // === OVERLAY-LEVEL BINDING (overlayElement_) ===

    // Confirm selection on NAV button (scoped to page selector overlay)
    buttons_.button(ButtonID::NAV)
        .press()
        .scope(scope(overlayElement_))
        .then([this]() { confirmSelection(); });
}

void HandlerInputDevicePage::requestPageList() {
    // Show cached page list immediately if available (cache-first)
    if (state_.pageSelector.names.size() > 0 && !state_.pageSelector.visible.get()) {
        state_.pageSelector.visible.set(true);
    }

    // Set encoder to relative mode for navigation
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    // Always request fresh data from Bitwig
    if (!requested_) {
        protocol_.send(Protocol::RequestDevicePageNamesMessage{});
        requested_ = true;
    }
}

void HandlerInputDevicePage::navigate(float delta) {
    size_t count = state_.pageSelector.names.size();
    if (count == 0) return;

    int currentIndex = state_.pageSelector.selectedIndex.get();
    int newIndex = currentIndex + static_cast<int>(delta);
    newIndex = wrapIndex(newIndex, static_cast<int>(count));
    state_.pageSelector.selectedIndex.set(newIndex);
}

void HandlerInputDevicePage::confirmSelection() {
    int index = state_.pageSelector.selectedIndex.get();
    if (index >= 0) {
        protocol_.send(Protocol::DevicePageSelectByIndexMessage{static_cast<uint8_t>(index)});
    }
}

void HandlerInputDevicePage::closeSelector() {
    int index = state_.pageSelector.selectedIndex.get();

    // Hide page selector
    state_.pageSelector.visible.set(false);

    // Confirm selection on close
    if (index >= 0) {
        protocol_.send(Protocol::DevicePageSelectByIndexMessage{static_cast<uint8_t>(index)});
    }

    requested_ = false;
    buttons_.setLatch(ButtonID::LEFT_BOTTOM, false);
}

}  // namespace bitwig::handler
