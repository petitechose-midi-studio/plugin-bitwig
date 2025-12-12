#include "HandlerInputDeviceSelector.hpp"

#include <oc/ui/lvgl/Scope.hpp>

#include "config/App.hpp"
#include "protocol/struct/DeviceSelectByIndexMessage.hpp"
#include "protocol/struct/DeviceStateChangeMessage.hpp"
#include "protocol/struct/EnterDeviceChildMessage.hpp"
#include "protocol/struct/ExitToParentMessage.hpp"
#include "protocol/struct/RequestDeviceChildrenMessage.hpp"
#include "protocol/struct/RequestDeviceListMessage.hpp"
#include "protocol/struct/RequestTrackListMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using Config::ButtonID;
using Config::EncoderID;

namespace {
int wrapIndex(int value, int modulo) {
    return ((value % modulo) + modulo) % modulo;
}
}  // namespace

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
    auto& ds = state_.deviceSelector;

    // Show cached list immediately if available (cache-first)
    if (ds.names.size() > 0 && !ds.visible.get()) {
        ds.visible.set(true);
    }

    // Set encoder to relative mode
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    // Reset navigation mode
    navigation_.mode = SelectorMode::Devices;

    // Always request fresh data from Bitwig
    if (!requested_) {
        protocol_.send(Protocol::RequestDeviceListMessage{});
        requested_ = true;
    }
}

void HandlerInputDeviceSelector::navigate(float delta) {
    auto& ds = state_.deviceSelector;
    int itemCount = ds.showingChildren.get()
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

    if (navigation_.mode == SelectorMode::Devices) {
        enterDeviceAtIndex(index);
    } else {
        enterChildAtIndex(index);
    }
}

void HandlerInputDeviceSelector::select() {
    auto& ds = state_.deviceSelector;
    int index = ds.currentIndex.get();

    if (navigation_.mode == SelectorMode::Devices) {
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
        navigation_.deviceIndex = deviceIndex;
        navigation_.mode = SelectorMode::Children;
        protocol_.send(Protocol::RequestDeviceChildrenMessage{static_cast<uint8_t>(deviceIndex), 0});
    } else {
        // No children - select/focus this device directly
        protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
    }
}

void HandlerInputDeviceSelector::enterChildAtIndex(int selectorIndex) {
    if (selectorIndex == 0) {
        // Back to device list
        navigation_.mode = SelectorMode::Devices;
        protocol_.send(Protocol::RequestDeviceListMessage{});
        return;
    }

    int listIndex = selectorIndex - 1;
    uint8_t itemType = (listIndex < static_cast<int>(navigation_.itemTypes.size()))
                           ? navigation_.itemTypes[listIndex]
                           : 0;
    uint8_t childIndex = (listIndex < static_cast<int>(navigation_.childIndices.size()))
                             ? navigation_.childIndices[listIndex]
                             : listIndex;

    protocol_.send(Protocol::EnterDeviceChildMessage{navigation_.deviceIndex, itemType, childIndex});
}

void HandlerInputDeviceSelector::toggleState() {
    auto& ds = state_.deviceSelector;
    int selectorIndex = ds.currentIndex.get();
    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    size_t count = ds.names.size();

    if (deviceIndex >= 0 && static_cast<size_t>(deviceIndex) < count) {
        protocol_.send(Protocol::DeviceStateChangeMessage{static_cast<uint8_t>(deviceIndex), true});
    }
}

void HandlerInputDeviceSelector::requestTrackList() {
    // Skip if track list was just closed by this same press
    if (trackListRequested_) {
        trackListRequested_ = false;
        return;
    }

    auto& ts = state_.trackSelector;

    // Show cached track list immediately if available (cache-first)
    if (ts.names.size() > 0 && !ts.visible.get()) {
        ts.visible.set(true);
    }

    protocol_.send(Protocol::RequestTrackListMessage{});
    trackListRequested_ = true;
}

void HandlerInputDeviceSelector::close() {
    // Hide track selector if visible
    if (state_.trackSelector.visible.get()) {
        state_.trackSelector.visible.set(false);
        trackListRequested_ = false;
    }

    // Hide device selector
    state_.deviceSelector.visible.set(false);

    requested_ = false;
    buttons_.setLatch(ButtonID::LEFT_CENTER, false);
}

bool HandlerInputDeviceSelector::hasChildren(uint8_t deviceIndex) const {
    // Check if device has children based on hasSlots/hasLayers/hasDrums
    if (deviceIndex >= state_.deviceSelector.hasSlots.size()) return false;

    return state_.deviceSelector.hasSlots[deviceIndex] ||
           state_.deviceSelector.hasLayers[deviceIndex] ||
           state_.deviceSelector.hasDrums[deviceIndex];
}

int HandlerInputDeviceSelector::getAdjustedDeviceIndex(int selectorIndex) const {
    return state_.deviceSelector.isNested.get() ? selectorIndex - 1 : selectorIndex;
}

}  // namespace bitwig::handler
