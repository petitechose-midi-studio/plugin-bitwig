#include "HandlerInputDeviceSelector.hpp"

#include <oc/debug/InvariantAssert.hpp>
#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/InputUtils.hpp"
#include "handler/NestedIndexUtils.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;
using OverlayType = ui::OverlayType;

HandlerInputDeviceSelector::HandlerInputDeviceSelector(state::BitwigState& state,
                                                       core::ui::OverlayController<bitwig::ui::OverlayType>& overlays,
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

    // Auto-reset local state when overlay hidden externally
    // Note: Latch clearing is handled by OverlayController
    visibleSub_ = state_.deviceSelector.visible.subscribe([this](bool visible) {
        if (!visible) {
            requested_ = false;
            // Reset children view so next open shows device list, not stale children
            state_.deviceSelector.showingChildren.set(false);
        }
    });
}

void HandlerInputDeviceSelector::setupBindings() {
    // === VIEW-LEVEL BINDING ===
    // Open binding on view scope (works when overlay not yet visible)
    buttons_.button(ButtonID::LEFT_CENTER)
        .press()
        .latch()
        .scope(scope(scopeElement_))
        .then([this]() { requestDeviceList(); });

    // === OVERLAY-LEVEL BINDINGS ===

    // Confirm and close on release
    buttons_.button(ButtonID::LEFT_CENTER)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() {
            select();
            close();
        });

    // Cancel on LEFT_TOP
    buttons_.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { close(); });

    // Navigate devices
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .scope(scope(overlayElement_))
        .then([this](float delta) { navigate(delta); });

    // Enter device children
    buttons_.button(ButtonID::NAV)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { selectAndDive(); });

    // Toggle device state
    buttons_.button(ButtonID::BOTTOM_CENTER)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { toggleState(); });

    // Open TrackSelector with latch behavior (same as LEFT_CENTER for DeviceSelector)
    // Short press: opens and latches, release does nothing
    // Long press or second short press: release confirms and closes
    buttons_.button(ButtonID::BOTTOM_LEFT)
        .press()
        .latch()
        .scope(scope(overlayElement_))
        .then([this]() { requestTrackList(); });
}

void HandlerInputDeviceSelector::requestDeviceList() {
    auto& ds = state_.deviceSelector;

    // Always show overlay immediately (even if empty - will show "No Device")
    if (!ds.visible.get()) {
        state_.overlays.show(OverlayType::DEVICE_SELECTOR, false);
    }

    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    if (!requested_) {
        // Reset cache for fresh load
        ds.names.clear();
        ds.totalCount.set(0);
        ds.loadedUpTo.set(0);
        ds.loading.set(true);  // Mark as loading until host responds
        // Request first window (windowed loading)
        protocol_.requestDeviceListWindow(0);
        requested_ = true;
    }
}

void HandlerInputDeviceSelector::navigate(float delta) {
    auto& ds = state_.deviceSelector;

    // When showing children, no windowed loading (children are loaded all at once)
    if (isShowingChildren()) {
        int itemCount = static_cast<int>(ds.childrenNames.size());
        if (itemCount == 0) return;

        int currentIndex = ds.currentIndex.get();
        int newIndex = currentIndex + static_cast<int>(delta);
        newIndex = wrapIndex(newIndex, itemCount);
        ds.currentIndex.set(newIndex);
        return;
    }

    // Device list navigation with windowed loading
    uint8_t totalCount = ds.totalCount.get();
    // Account for back button in nested mode
    int displayCount = ds.isNested.get() ? totalCount + 1 : totalCount;
    if (displayCount == 0) {
        // Fallback to names.size() for legacy/compatibility
        displayCount = static_cast<int>(ds.names.size());
    }
    if (displayCount == 0) return;

    int currentIndex = ds.currentIndex.get();
    int newIndex = currentIndex + static_cast<int>(delta);
    newIndex = wrapIndex(newIndex, displayCount);
    ds.currentIndex.set(newIndex);

    // Prefetch next window if approaching end of loaded data
    uint8_t loadedUpTo = ds.loadedUpTo.get();
    // Convert display index to device index (for nested, display 0 is back button)
    int deviceIndex = ds.isNested.get() ? newIndex - 1 : newIndex;

    // Only prefetch if we have a valid device index approaching the loaded boundary
    if (deviceIndex >= 0 &&
        shouldPrefetch<state::PREFETCH_THRESHOLD>(deviceIndex, loadedUpTo, totalCount)) {
        protocol_.requestDeviceListWindow(loadedUpTo);
    }
}

void HandlerInputDeviceSelector::selectAndDive() {
    int index = state_.deviceSelector.currentIndex.get();

    if (!isShowingChildren()) {
        enterDeviceAtIndex(index);
    } else {
        enterChildAtIndex(index);
    }
}

void HandlerInputDeviceSelector::select() {
    auto& ds = state_.deviceSelector;
    int index = ds.currentIndex.get();

    if (!isShowingChildren()) {
        if (ds.isNested.get() && index == 0) {
            protocol_.exitToParent();
            return;
        }
        int deviceIndex = getAdjustedDeviceIndex(index);
        size_t count = ds.names.size();
        if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
            protocol_.deviceSelect(static_cast<uint8_t>(deviceIndex));
        }
    } else {
        enterChildAtIndex(index);
    }
}

void HandlerInputDeviceSelector::enterDeviceAtIndex(int selectorIndex) {
    auto& ds = state_.deviceSelector;

    if (ds.isNested.get() && selectorIndex == 0) {
        protocol_.exitToParent();
        return;
    }

    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    size_t count = ds.names.size();
    if (deviceIndex < 0 || static_cast<size_t>(deviceIndex) >= count) return;

    if (hasChildrenAtDisplayIndex(selectorIndex)) {
        // Store which device we're entering children of (raw bank index for host)
        currentDeviceIndex_ = static_cast<uint8_t>(deviceIndex);
        // Request children - HostHandler will set showingChildren=true when response arrives
        protocol_.requestDeviceChildren(static_cast<uint8_t>(deviceIndex), 0);
    } else {
        // No children - select/focus this device directly
        protocol_.deviceSelect(static_cast<uint8_t>(deviceIndex));
    }
}

void HandlerInputDeviceSelector::enterChildAtIndex(int selectorIndex) {
    auto& ds = state_.deviceSelector;

    if (selectorIndex == 0) {
        // Back to device list - reset cache and request fresh windowed list
        ds.names.clear();
        ds.totalCount.set(0);
        ds.loadedUpTo.set(0);
        protocol_.requestDeviceListWindow(0);
        return;
    }

    // childrenTypes layout: [0]=back button placeholder, [1]=first child type, [2]=second child type...
    // selectorIndex=1 → first actual child → childrenTypes[1]
    uint8_t itemType = 0;
    if (selectorIndex < static_cast<int>(ds.childrenTypes.size())) {
        itemType = ds.childrenTypes[selectorIndex];
    }

    // childIndex for host is 0-based (selectorIndex=1 → childIndex=0)
    uint8_t childIndex = static_cast<uint8_t>(selectorIndex - 1);

    protocol_.enterDeviceChild(currentDeviceIndex_, itemType, childIndex);
}

void HandlerInputDeviceSelector::toggleState() {
    auto& ds = state_.deviceSelector;
    if (isShowingChildren()) return;

    int selectorIndex = ds.currentIndex.get();
    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    size_t count = ds.names.size();

    if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
        protocol_.deviceState(static_cast<uint8_t>(deviceIndex), true);
    }
}

void HandlerInputDeviceSelector::requestTrackList() {
    auto& ts = state_.trackSelector;

    if (ts.visible.get()) return;

    // Show track selector overlay immediately on user request
    // NOTE: No need to reset LEFT_CENTER latch - scope-aware latch handles this
    state_.overlays.show(OverlayType::TRACK_SELECTOR, true);

    // Reset track cache for fresh load
    ts.names.clear();
    ts.totalCount.set(0);
    ts.loadedUpTo.set(0);

    // Request first window - data will populate in background
    protocol_.requestTrackListWindow(0);
}

void HandlerInputDeviceSelector::close() {
    // OverlayController handles latch cleanup synchronously before hiding
    overlays_.hideAll();

    // Verify cleanup happened (debug builds only)
    OC_ASSERT_OVERLAY_LIFECYCLE(
        !buttons_.isLatched(ButtonID::LEFT_CENTER),
        "DeviceSelector latch should be cleared after hideAll()"
    );
}

bool HandlerInputDeviceSelector::hasChildrenAtDisplayIndex(int displayIndex) const {
    // displayIndex is the UI list index (includes back button when nested)
    // hasSlots/hasLayers/hasDrums are also indexed by display index
    auto& ds = state_.deviceSelector;
    if (displayIndex < 0 || displayIndex >= static_cast<int>(ds.hasSlots.size())) return false;

    return ds.hasSlots[displayIndex] ||
           ds.hasLayers[displayIndex] ||
           ds.hasDrums[displayIndex];
}

int HandlerInputDeviceSelector::getAdjustedDeviceIndex(int selectorIndex) const {
    return utils::toRawIndex(selectorIndex, state_.deviceSelector.isNested.get());
}

bool HandlerInputDeviceSelector::isShowingChildren() const {
    return state_.deviceSelector.showingChildren.get();
}

}  // namespace bitwig::handler
