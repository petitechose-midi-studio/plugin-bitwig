#include "HandlerInputDeviceSelector.hpp"

#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

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
    OC_LOG_INFO("[DeviceSelectorInput] Creating handler...");
    setupBindings();
    OC_LOG_INFO("[DeviceSelectorInput] Bindings created");
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
    OC_LOG_INFO("[DeviceSelectorInput] >> requestDeviceList()");
    auto& ds = state_.deviceSelector;

    // Show cached list immediately if available (cache-first)
    if (ds.names.size() > 0 && !ds.visible.get()) {
        ds.visible.set(true);
    }

    // Set encoder to relative mode
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    // Always request fresh data from Bitwig
    if (!requested_) {
        protocol_.send(Protocol::RequestDeviceListMessage{});
        requested_ = true;
    }
    OC_LOG_INFO("[DeviceSelectorInput] << requestDeviceList() done");
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
    OC_LOG_INFO("[DeviceSelectorInput] >> selectAndDive()");
    int index = state_.deviceSelector.currentIndex.get();

    if (!isShowingChildren()) {
        enterDeviceAtIndex(index);
    } else {
        enterChildAtIndex(index);
    }
    OC_LOG_INFO("[DeviceSelectorInput] << selectAndDive() done");
}

void HandlerInputDeviceSelector::select() {
    OC_LOG_INFO("[DeviceSelectorInput] >> select()");
    auto& ds = state_.deviceSelector;
    int index = ds.currentIndex.get();

    if (!isShowingChildren()) {
        // Handle back button in nested mode
        if (ds.isNested.get() && index == 0) {
            protocol_.send(Protocol::ExitToParentMessage{});
            return;
        }
        // Select device directly (no dive into children)
        int deviceIndex = getAdjustedDeviceIndex(index);
        size_t count = ds.names.size();
        if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
            protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
        }
    } else {
        // In children mode, confirm the selected child
        enterChildAtIndex(index);
    }
    OC_LOG_INFO("[DeviceSelectorInput] << select() done");
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

    if (hasChildren(deviceIndex)) {
        // Store which device we're entering children of
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

    // Read item type directly from state (set by HostHandler on DeviceChildren message)
    int listIndex = selectorIndex - 1;
    uint8_t itemType = 0;
    if (listIndex < static_cast<int>(ds.childrenTypes.size())) {
        itemType = ds.childrenTypes[listIndex];
    }

    // Child index is simply the list index
    uint8_t childIndex = static_cast<uint8_t>(listIndex);

    protocol_.send(Protocol::EnterDeviceChildMessage{currentDeviceIndex_, itemType, childIndex});
}

void HandlerInputDeviceSelector::toggleState() {
    OC_LOG_INFO("[DeviceSelectorInput] >> toggleState()");
    auto& ds = state_.deviceSelector;

    // Only toggle state when showing devices (not children)
    if (isShowingChildren()) return;

    int selectorIndex = ds.currentIndex.get();
    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    size_t count = ds.names.size();

    if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
        protocol_.send(Protocol::DeviceStateChangeMessage{static_cast<uint8_t>(deviceIndex), true});
    }
    OC_LOG_INFO("[DeviceSelectorInput] << toggleState() done");
}

void HandlerInputDeviceSelector::requestTrackList() {
    OC_LOG_INFO("[DeviceSelectorInput] >> requestTrackList()");

    auto& ts = state_.trackSelector;
    auto& ds = state_.deviceSelector;

    // Already visible - don't toggle (handles spurious triggers after close())
    if (ts.visible.get()) {
        OC_LOG_INFO("[DeviceSelectorInput] << requestTrackList() skipped (already visible)");
        return;
    }

    // Hide device selector, show track selector
    ds.visible.set(false);
    ts.visible.set(true);

    protocol_.send(Protocol::RequestTrackListMessage{});
    OC_LOG_INFO("[DeviceSelectorInput] << requestTrackList() done");
}

void HandlerInputDeviceSelector::close() {
    OC_LOG_INFO("[DeviceSelectorInput] >> close()");
    // Hide track selector if visible
    if (state_.trackSelector.visible.get()) { state_.trackSelector.visible.set(false); }

    // Hide device selector
    state_.deviceSelector.visible.set(false);

    requested_ = false;
    buttons_.setLatch(ButtonID::LEFT_CENTER, false);
    OC_LOG_INFO("[DeviceSelectorInput] << close() done");
}

bool HandlerInputDeviceSelector::hasChildren(uint8_t deviceIndex) const {
    auto& ds = state_.deviceSelector;
    if (deviceIndex >= ds.hasSlots.size()) return false;

    return ds.hasSlots[deviceIndex] ||
           ds.hasLayers[deviceIndex] ||
           ds.hasDrums[deviceIndex];
}

int HandlerInputDeviceSelector::getAdjustedDeviceIndex(int selectorIndex) const {
    return utils::toRawIndex(selectorIndex, state_.deviceSelector.isNested.get());
}

bool HandlerInputDeviceSelector::isShowingChildren() const {
    return state_.deviceSelector.showingChildren.get();
}

}  // namespace bitwig::handler
