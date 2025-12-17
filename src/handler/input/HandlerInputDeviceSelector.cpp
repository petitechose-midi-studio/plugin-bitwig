#include "HandlerInputDeviceSelector.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "handler/InputUtils.hpp"
#include "handler/NestedIndexUtils.hpp"
#include "protocol/struct/DeviceSelectByIndexMessage.hpp"
#include "protocol/struct/DeviceStateChangeMessage.hpp"
#include "protocol/struct/EnterDeviceChildMessage.hpp"
#include "protocol/struct/ExitToParentMessage.hpp"
#include "protocol/struct/RequestDeviceChildrenMessage.hpp"
#include "protocol/struct/RequestDeviceListMessage.hpp"
#include "protocol/struct/RequestTrackListMessage.hpp"

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

    // Show cached list immediately if available (cache-first)
    if (ds.names.size() > 0 && !ds.visible.get()) {
        state_.overlays.show(OverlayType::DEVICE_SELECTOR, false);
    }

    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    if (!requested_) {
        protocol_.send(Protocol::RequestDeviceListMessage{});
        requested_ = true;
    }
}

void HandlerInputDeviceSelector::navigate(float delta) {
    auto& ds = state_.deviceSelector;
    int itemCount = isShowingChildren()
        ? static_cast<int>(ds.childrenNames.size())
        : static_cast<int>(ds.names.size());

    if (itemCount == 0) return;

    int currentIndex = ds.currentIndex.get();
    int newIndex = currentIndex + static_cast<int>(delta);
    newIndex = wrapIndex(newIndex, itemCount);
    ds.currentIndex.set(newIndex);
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
        // Back to device list - request fresh list (host will set showingChildren=false)
        protocol_.send(Protocol::RequestDeviceListMessage{});
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

    // Show track selector (pushes device selector to stack for restoration)
    state_.overlays.show(OverlayType::TRACK_SELECTOR, true);
    protocol_.send(Protocol::RequestTrackListMessage{});
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
