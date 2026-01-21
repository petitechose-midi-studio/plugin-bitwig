#include "DevicePageInputHandler.hpp"

#include <oc/debug/InvariantAssert.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include <config/App.hpp>
#include "handler/InputUtils.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;
using OverlayType = ui::OverlayType;

DevicePageInputHandler::DevicePageInputHandler(state::BitwigState& state,
                                               OverlayCtx overlayCtx,
                                               BitwigProtocol& protocol,
                                               core::api::InputAPI input)
    : state_(state)
    , overlay_ctx_(overlayCtx)
    , protocol_(protocol)
    , input_(input) {
    setupBindings();
}

void DevicePageInputHandler::setupBindings() {
    // === VIEW-LEVEL BINDING ===
    // Open page selector (latch behavior for toggle)
    input_.buttons.button(ButtonID::LEFT_BOTTOM)
        .press()
        .latch()
        .scope(scope(overlay_ctx_.scopeElement))
        .then([this]() { openSelector(); });

    // === OVERLAY-LEVEL BINDINGS ===

    // Close and confirm on release (long press or second toggle press)
    input_.buttons.button(ButtonID::LEFT_BOTTOM)
        .release()
        .scope(scope(overlay_ctx_.overlayElement))
        .then([this]() { closeSelector(); });

    // Navigate pages (scoped to overlay - active while overlay visible)
    input_.encoders.encoder(EncoderID::NAV)
        .turn()
        .scope(scope(overlay_ctx_.overlayElement))
        .then([this](float delta) { navigate(delta); });

    // Confirm selection on NAV button (without closing)
    input_.buttons.button(ButtonID::NAV)
        .release()
        .scope(scope(overlay_ctx_.overlayElement))
        .then([this]() { confirmSelection(); });

    // Cancel on LEFT_TOP (close without confirming)
    input_.buttons.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlay_ctx_.overlayElement))
        .then([this]() { cancel(); });
}

void DevicePageInputHandler::openSelector() {
    // Show overlay (ExclusiveVisibilityStack handles exclusive visibility)
    // Page names are preloaded by DeviceHostHandler on device change
    if (!state_.pageSelector.visible.get()) {
        state_.overlays.show(OverlayType::PAGE_SELECTOR, false);
    }

    // Set encoder to relative mode for navigation
    input_.encoders.setMode(EncoderID::NAV, oc::interface::EncoderMode::RELATIVE);
}

void DevicePageInputHandler::navigate(float delta) {
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

void DevicePageInputHandler::confirmSelection() {
    int index = state_.pageSelector.selectedIndex.get();
    if (index >= 0) {
        protocol_.devicePageSelect(static_cast<uint8_t>(index));
    }
}

void DevicePageInputHandler::closeSelector() {
    int index = state_.pageSelector.selectedIndex.get();

    // Confirm selection on close
    if (index >= 0) {
        protocol_.devicePageSelect(static_cast<uint8_t>(index));
    }

    // OverlayManager handles latch cleanup synchronously before hiding
    overlay_ctx_.controller.hide();

    OC_ASSERT_OVERLAY_LIFECYCLE(
        !input_.buttons.isLatched(ButtonID::LEFT_BOTTOM),
        "PageSelector latch should be cleared after hide()"
    );
}

void DevicePageInputHandler::cancel() {
    // OverlayManager handles latch cleanup synchronously before hiding
    overlay_ctx_.controller.hide();

    OC_ASSERT_OVERLAY_LIFECYCLE(
        !input_.buttons.isLatched(ButtonID::LEFT_BOTTOM),
        "PageSelector latch should be cleared after hide()"
    );
}

}  // namespace bitwig::handler
