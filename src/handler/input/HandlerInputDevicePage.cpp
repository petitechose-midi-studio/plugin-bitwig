#include "HandlerInputDevicePage.hpp"

#include <oc/debug/InvariantAssert.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/InputUtils.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;
using OverlayType = ui::OverlayType;

HandlerInputDevicePage::HandlerInputDevicePage(state::BitwigState& state,
                                               ::ui::OverlayController<bitwig::ui::OverlayType>& overlays,
                                               BitwigProtocol& protocol,
                                               oc::api::EncoderAPI& encoders,
                                               oc::api::ButtonAPI& buttons,
                                               lv_obj_t* scopeElement,
                                               lv_obj_t* overlayElement)
    : state_(state)
    , overlays_(overlays)
    , protocol_(protocol)
    , encoders_(encoders)
    , buttons_(buttons)
    , scopeElement_(scopeElement)
    , overlayElement_(overlayElement) {
    setupBindings();
}

void HandlerInputDevicePage::setupBindings() {
    // === VIEW-LEVEL BINDING ===
    // Open page selector (latch behavior for toggle)
    buttons_.button(ButtonID::LEFT_BOTTOM)
        .press()
        .latch()
        .scope(scope(scopeElement_))
        .then([this]() { openSelector(); });

    // === OVERLAY-LEVEL BINDINGS ===

    // Close and confirm on release (long press or second toggle press)
    buttons_.button(ButtonID::LEFT_BOTTOM)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { closeSelector(); });

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

void HandlerInputDevicePage::openSelector() {
    // Show overlay (ExclusiveVisibilityStack handles exclusive visibility)
    // Page names are preloaded by HandlerHostDevice on device change
    if (!state_.pageSelector.visible.get()) {
        state_.overlays.show(OverlayType::PAGE_SELECTOR, false);
    }

    // Set encoder to relative mode for navigation
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);
}

void HandlerInputDevicePage::navigate(float delta) {
    // Use totalCount for navigation (windowed loading)
    uint8_t totalCount = state_.pageSelector.totalCount.get();
    if (totalCount == 0) {
        // Fallback to names.size() for legacy/compatibility
        totalCount = static_cast<uint8_t>(state_.pageSelector.names.size());
    }
    if (totalCount == 0) return;

    int currentIndex = state_.pageSelector.selectedIndex.get();
    int newIndex = currentIndex + static_cast<int>(delta);
    newIndex = wrapIndex(newIndex, static_cast<int>(totalCount));
    state_.pageSelector.selectedIndex.set(newIndex);

    // Prefetch next window if approaching end of loaded data
    uint8_t loadedUpTo = state_.pageSelector.loadedUpTo.get();
    if (newIndex >= 0 &&
        shouldPrefetch<state::PREFETCH_THRESHOLD>(newIndex, loadedUpTo, totalCount)) {
        protocol_.requestDevicePageNamesWindow(loadedUpTo);
    }
}

void HandlerInputDevicePage::confirmSelection() {
    int index = state_.pageSelector.selectedIndex.get();
    if (index >= 0) {
        protocol_.devicePageSelect(static_cast<uint8_t>(index));
    }
}

void HandlerInputDevicePage::closeSelector() {
    int index = state_.pageSelector.selectedIndex.get();

    // Confirm selection on close
    if (index >= 0) {
        protocol_.devicePageSelect(static_cast<uint8_t>(index));
    }

    // OverlayController handles latch cleanup synchronously before hiding
    overlays_.hide();

    OC_ASSERT_OVERLAY_LIFECYCLE(
        !buttons_.isLatched(ButtonID::LEFT_BOTTOM),
        "PageSelector latch should be cleared after hide()"
    );
}

void HandlerInputDevicePage::cancel() {
    // OverlayController handles latch cleanup synchronously before hiding
    overlays_.hide();

    OC_ASSERT_OVERLAY_LIFECYCLE(
        !buttons_.isLatched(ButtonID::LEFT_BOTTOM),
        "PageSelector latch should be cleared after hide()"
    );
}

}  // namespace bitwig::handler
