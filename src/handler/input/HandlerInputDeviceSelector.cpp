#include "HandlerInputDeviceSelector.hpp"

#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/InputUtils.hpp"
#include "handler/NestedIndexUtils.hpp"
#include "protocol/struct/DeviceSelectByIndexMessage.hpp"
#include "state/Constants.hpp"
#include "protocol/struct/DeviceStateChangeMessage.hpp"
#include "protocol/struct/EnterDeviceChildMessage.hpp"
#include "protocol/struct/ExitToParentMessage.hpp"
#include "protocol/struct/RequestDeviceChildrenMessage.hpp"
#include "protocol/struct/RequestDeviceListWindowMessage.hpp"
#include "protocol/struct/RequestTrackListWindowMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;
using OverlayType = state::OverlayType;

HandlerInputDeviceSelector::HandlerInputDeviceSelector(state::BitwigState& state,
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

    // Auto-reset latch and state when overlay hidden externally (by OverlayManager)
    visibleSub_ = state_.deviceSelector.visible.subscribe([this](bool visible) {
        if (!visible) {
            buttons_.setLatch(ButtonID::LEFT_CENTER, false);
            requested_ = false;
            // Reset children view so next open shows device list, not stale children
            state_.deviceSelector.showingChildren.set(false);
        }
    });
}

void HandlerInputDeviceSelector::setupBindings() {
    // === VIEW-LEVEL BINDINGS (scopeElement_) ===
    // These are lower priority and work when no overlay is capturing input

    // Open device selector (latch behavior)
    buttons_.button(ButtonID::LEFT_CENTER)
        .press()
        .latch()
        .scope(scope(scopeElement_))
        .then([this]() { requestDeviceList(); });

    // Confirm and close on release
    buttons_.button(ButtonID::LEFT_CENTER)
        .release()
        .scope(scope(scopeElement_))
        .then([this]() {
            select();
            close();
        });

    // === OVERLAY-LEVEL BINDINGS (overlayElement_) ===
    // These have higher priority and only fire when overlay is visible

    // Cancel on LEFT_TOP (close without confirming)
    buttons_.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { close(); });

    // Navigate devices with NAV encoder (scoped to overlay)
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .scope(scope(overlayElement_))
        .then([this](float delta) { navigate(delta); });

    // Enter device children with NAV button (scoped to overlay)
    buttons_.button(ButtonID::NAV)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { selectAndDive(); });

    // Toggle device state (scoped to overlay)
    buttons_.button(ButtonID::BOTTOM_CENTER)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { toggleState(); });

    // Request track list (scoped to overlay)
    buttons_.button(ButtonID::BOTTOM_LEFT)
        .release()
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
        protocol_.send(Protocol::RequestDeviceListWindowMessage{0});
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
        loadedUpTo > state::PREFETCH_THRESHOLD &&
        static_cast<uint8_t>(deviceIndex) >= loadedUpTo - state::PREFETCH_THRESHOLD &&
        loadedUpTo < totalCount) {
        protocol_.send(Protocol::RequestDeviceListWindowMessage{loadedUpTo});
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
            protocol_.send(Protocol::ExitToParentMessage{});
            return;
        }
        int deviceIndex = getAdjustedDeviceIndex(index);
        size_t count = ds.names.size();
        if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
            protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
        }
    } else {
        enterChildAtIndex(index);
    }
}

void HandlerInputDeviceSelector::enterDeviceAtIndex(int selectorIndex) {
    auto& ds = state_.deviceSelector;

    if (ds.isNested.get() && selectorIndex == 0) {
        protocol_.send(Protocol::ExitToParentMessage{});
        return;
    }

    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    size_t count = ds.names.size();
    if (deviceIndex < 0 || static_cast<size_t>(deviceIndex) >= count) return;

    if (hasChildrenAtDisplayIndex(selectorIndex)) {
        // Store which device we're entering children of (raw bank index for host)
        currentDeviceIndex_ = static_cast<uint8_t>(deviceIndex);
        // Request children - HostHandler will set showingChildren=true when response arrives
        protocol_.send(Protocol::RequestDeviceChildrenMessage{static_cast<uint8_t>(deviceIndex), 0});
    } else {
        // No children - select/focus this device directly
        protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
    }
}

void HandlerInputDeviceSelector::enterChildAtIndex(int selectorIndex) {
    auto& ds = state_.deviceSelector;

    if (selectorIndex == 0) {
        // Back to device list - reset cache and request fresh windowed list
        ds.names.clear();
        ds.totalCount.set(0);
        ds.loadedUpTo.set(0);
        protocol_.send(Protocol::RequestDeviceListWindowMessage{0});
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

    protocol_.send(Protocol::EnterDeviceChildMessage{currentDeviceIndex_, itemType, childIndex});
}

void HandlerInputDeviceSelector::toggleState() {
    auto& ds = state_.deviceSelector;
    if (isShowingChildren()) return;

    int selectorIndex = ds.currentIndex.get();
    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    size_t count = ds.names.size();

    if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
        protocol_.send(Protocol::DeviceStateChangeMessage{static_cast<uint8_t>(deviceIndex), true});
    }
}

void HandlerInputDeviceSelector::requestTrackList() {
    auto& ts = state_.trackSelector;

    if (ts.visible.get()) return;

    // Reset track cache for fresh load
    ts.names.clear();
    ts.totalCount.set(0);
    ts.loadedUpTo.set(0);

    // Show track selector (pushes device selector to stack for restoration)
    state_.overlays.show(OverlayType::TRACK_SELECTOR, true);
    // Request first window (windowed loading)
    protocol_.send(Protocol::RequestTrackListWindowMessage{0});
}

void HandlerInputDeviceSelector::close() {
    // Hide all overlays via OverlayManager (subscription handles latch reset)
    state_.overlays.hideAll();
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
