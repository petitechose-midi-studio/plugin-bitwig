#include "HandlerInputDevicePage.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/InputUtils.hpp"
#include "protocol/struct/DevicePageSelectByIndexMessage.hpp"
#include "protocol/struct/RequestDevicePageNamesMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;
using OverlayType = state::OverlayType;

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

    // Open page selector (latch behavior for toggle)
    buttons_.button(ButtonID::LEFT_BOTTOM)
        .press()
        .latch()
        .scope(scope(scopeElement_))
        .then([this]() { requestPageList(); });

    // Close and confirm on release (long press or second toggle press)
    buttons_.button(ButtonID::LEFT_BOTTOM)
        .release()
        .scope(scope(scopeElement_))
        .then([this]() { closeSelector(); });

    // === OVERLAY-LEVEL BINDINGS (overlayElement_) ===

    // Navigate pages (scoped to overlay - active while overlay visible)
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .scope(scope(overlayElement_))
        .then([this](float delta) { navigate(delta); });

    // Confirm selection on NAV button (without closing)
    buttons_.button(ButtonID::NAV)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { confirmSelection(); });

    // Cancel on LEFT_TOP (close without confirming)
    buttons_.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { cancel(); });
}

void HandlerInputDevicePage::requestPageList() {
    // Show overlay (OverlayManager handles exclusive visibility)
    if (!state_.pageSelector.visible.get()) {
        state_.overlays.show(OverlayType::PAGE_SELECTOR, false);
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

    // Hide page selector via OverlayManager
    state_.overlays.hide();

    // Confirm selection on close
    if (index >= 0) {
        protocol_.send(Protocol::DevicePageSelectByIndexMessage{static_cast<uint8_t>(index)});
    }

    requested_ = false;
    buttons_.setLatch(ButtonID::LEFT_BOTTOM, false);
}

void HandlerInputDevicePage::cancel() {
    // Hide page selector via OverlayManager (no confirmation)
    state_.overlays.hide();
    requested_ = false;
    buttons_.setLatch(ButtonID::LEFT_BOTTOM, false);
}

}  // namespace bitwig::handler
