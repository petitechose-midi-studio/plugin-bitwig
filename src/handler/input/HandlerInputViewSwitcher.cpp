#include "HandlerInputViewSwitcher.hpp"

#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/InputUtils.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;
using OverlayType = state::OverlayType;
// ViewType is in global scope from protocol/ViewType.hpp

HandlerInputViewSwitcher::HandlerInputViewSwitcher(state::BitwigState& state,
                                                   state::OverlayController& overlays,
                                                   oc::api::EncoderAPI& encoders,
                                                   oc::api::ButtonAPI& buttons,
                                                   lv_obj_t* scopeElement,
                                                   lv_obj_t* overlayElement)
    : state_(state)
    , overlays_(overlays)
    , encoders_(encoders)
    , buttons_(buttons)
    , scopeElement_(scopeElement)
    , overlayElement_(overlayElement) {
    setupBindings();
}

void HandlerInputViewSwitcher::setupBindings() {
    // === VIEW-LEVEL BINDING ===
    // Open view selector (latch behavior for toggle)
    buttons_.button(ButtonID::LEFT_TOP)
        .press()
        .latch()
        .scope(scope(scopeElement_))
        .then([this]() { openSelector(); });

    // === OVERLAY-LEVEL BINDINGS ===

    // Close and confirm on release (long press or second toggle press)
    buttons_.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { closeSelector(); });

    // Navigate views (scoped to overlay - active while overlay visible)
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .scope(scope(overlayElement_))
        .then([this](float delta) { navigate(delta); });

    // Confirm selection on NAV button (without closing)
    buttons_.button(ButtonID::NAV)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { confirmSelection(); });
}

void HandlerInputViewSwitcher::openSelector() {
    OC_LOG_DEBUG("[ViewSwitcher] openSelector");

    // Show overlay (ExclusiveVisibilityStack handles exclusive visibility)
    if (!state_.viewSelector.visible.get()) {
        state_.overlays.show(OverlayType::VIEW_SELECTOR, false);
    }

    // Set encoder to relative mode for navigation
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    // Set current view as selected index
    state_.viewSelector.selectedIndex.set(static_cast<int>(state_.views.current()));
}

void HandlerInputViewSwitcher::navigate(float delta) {
    int currentIndex = state_.viewSelector.selectedIndex.get();
    int newIndex = currentIndex + static_cast<int>(delta);
    newIndex = wrapIndex(newIndex, static_cast<int>(state::VIEW_TYPE_COUNT));
    state_.viewSelector.selectedIndex.set(newIndex);
}

void HandlerInputViewSwitcher::confirmSelection() {
    int index = state_.viewSelector.selectedIndex.get();
    if (index >= 0 && index < static_cast<int>(state::VIEW_TYPE_COUNT)) {
        state_.views.switchTo(static_cast<ViewType>(index));
        OC_LOG_INFO("[ViewSwitcher] Confirmed view: {}", VIEW_NAMES[index]);
    }
}

void HandlerInputViewSwitcher::closeSelector() {
    int index = state_.viewSelector.selectedIndex.get();

    // Confirm selection on close
    if (index >= 0 && index < static_cast<int>(state::VIEW_TYPE_COUNT)) {
        state_.views.switchTo(static_cast<ViewType>(index));
        OC_LOG_INFO("[ViewSwitcher] Switched to: {}", VIEW_NAMES[index]);
    }

    // OverlayController handles latch cleanup synchronously before hiding
    overlays_.hide();
}

void HandlerInputViewSwitcher::cancel() {
    // OverlayController handles latch cleanup synchronously before hiding
    overlays_.hide();
    OC_LOG_DEBUG("[ViewSwitcher] Cancelled");
}

}  // namespace bitwig::handler
