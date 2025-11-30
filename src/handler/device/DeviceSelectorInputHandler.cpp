#include "DeviceSelectorInputHandler.hpp"
#include "TrackInputHandler.hpp"
#include "InputUtils.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../protocol/struct/RequestDeviceListMessage.hpp"
#include "../../protocol/struct/RequestDeviceChildrenMessage.hpp"
#include "../../protocol/struct/EnterDeviceChildMessage.hpp"
#include "../../protocol/struct/ExitToParentMessage.hpp"
#include "../../protocol/struct/DeviceStateChangeMessage.hpp"
#include "../../protocol/struct/DeviceSelectByIndexMessage.hpp"
#include "../../protocol/struct/RequestTrackListMessage.hpp"

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

DeviceSelectorInputHandler::DeviceSelectorInputHandler(ControllerAPI& api, DeviceView& view,
                                                       Protocol::Protocol& protocol,
                                                       TrackInputHandler& trackHandler,
                                                       lv_obj_t* scope)
    : api_(api), view_(view), protocol_(protocol),
      trackHandler_(trackHandler), scope_(scope)
{
    setupBindings();
}

DeviceSelectorInputHandler::~DeviceSelectorInputHandler() = default;

// =============================================================================
// Public API
// =============================================================================

void DeviceSelectorInputHandler::setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex,
                                                    bool isNested,
                                                    const etl::array<uint8_t, 4>* childrenTypes,
                                                    uint8_t childrenTypesCount)
{
    deviceList_.count = deviceCount;
    deviceList_.currentIndex = currentDeviceIndex;
    deviceList_.isNested = isNested;
    navigation_.mode = SelectorMode::Devices;

    for (uint8_t i = 0; i < childrenTypesCount && i < Device::MAX_DEVICES; i++) {
        deviceList_.childrenTypes[i] = childrenTypes[i];
    }

    api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);
    int selectorIndex = isNested ? currentDeviceIndex + 1 : currentDeviceIndex;
    view_.state().deviceSelector.currentIndex = selectorIndex;
    view_.state().dirty.deviceSelector = true;
    view_.sync();
}

void DeviceSelectorInputHandler::setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType,
                                                        uint8_t childrenCount,
                                                        const std::vector<uint8_t>& itemTypes,
                                                        const std::vector<uint8_t>& childIndices)
{
    navigation_.deviceIndex = deviceIndex;
    navigation_.childType = childType;
    navigation_.childrenCount = childrenCount;
    navigation_.mode = SelectorMode::Children;

    for (size_t i = 0; i < itemTypes.size() && i < navigation_.itemTypes.size(); i++) {
        navigation_.itemTypes[i] = itemTypes[i];
        navigation_.childIndices[i] = (i < childIndices.size()) ? childIndices[i] : i;
    }

    api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);
    view_.state().deviceSelector.currentIndex = 1;
    view_.state().dirty.deviceSelector = true;
    view_.sync();
}

// =============================================================================
// Bindings Setup
// =============================================================================

void DeviceSelectorInputHandler::setupBindings() {
    lv_obj_t* overlay = view_.getDeviceSelectorElement();

    // Open device selector (latch behavior)
    api_.onPressed(ButtonID::LEFT_CENTER, [this]()
                   { requestDeviceList(); }, scope_, true);

    // Confirm selection (no dive) and close on release
    api_.onReleased(ButtonID::LEFT_CENTER, [this]()
                    {
        select();
        close(); }, scope_);

    // Close without confirming (scoped to overlay)
    api_.onReleased(ButtonID::LEFT_TOP, [this]()
                    { close(); }, overlay);

    // Navigate devices (scoped to overlay)
    api_.onTurned(EncoderID::NAV, [this](float delta)
                  { navigate(delta); }, overlay);

    // Enter device children (scoped to overlay)
    api_.onReleased(ButtonID::NAV, [this]()
                    { selectAndDive(); }, overlay);

    // Toggle device state (scoped to overlay)
    api_.onReleased(ButtonID::BOTTOM_CENTER, [this]()
                    { toggleState(); }, overlay);

    // Toggle track list (scoped to overlay)
    api_.onReleased(ButtonID::BOTTOM_LEFT, [this]()
                    { requestTrackList(); }, overlay);
}

// =============================================================================
// Handlers
// =============================================================================

void DeviceSelectorInputHandler::requestDeviceList() {
    // Show cached list immediately if available (cache-first)
    auto& state = view_.state().deviceSelector;
    if (!state.names.empty() && !state.visible) {
        state.visible = true;
        view_.state().dirty.deviceSelector = true;
        view_.sync();
    }

    // Always request fresh data from Bitwig
    if (!deviceList_.requested) {
        protocol_.send(Protocol::RequestDeviceListMessage{});
        deviceList_.requested = true;
    }
}

void DeviceSelectorInputHandler::navigate(float delta) {
    int itemCount = view_.getDeviceSelectorItemCount();
    if (itemCount == 0) return;

    auto& state = view_.state().deviceSelector;
    int newIndex = state.currentIndex + static_cast<int>(delta);
    newIndex = InputUtils::wrapIndex(newIndex, itemCount);
    state.currentIndex = newIndex;
    view_.state().dirty.deviceSelector = true;
    view_.sync();
}

void DeviceSelectorInputHandler::selectAndDive()
{
    int index = view_.state().deviceSelector.currentIndex;

    if (navigation_.mode == SelectorMode::Devices) {
        enterDeviceAtIndex(index);
    } else {
        enterChildAtIndex(index);
    }
}

void DeviceSelectorInputHandler::select()
{
    int index = view_.state().deviceSelector.currentIndex;

    if (navigation_.mode == SelectorMode::Devices)
    {
        // Handle back button in nested mode
        if (deviceList_.isNested && index == 0)
        {
            protocol_.send(Protocol::ExitToParentMessage{});
            return;
        }
        // Select device directly (no dive into children)
        int deviceIndex = getAdjustedDeviceIndex(index);
        if (deviceIndex >= 0 && deviceIndex < deviceList_.count)
        {
            protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
        }
    }
    else
    {
        // In children mode, confirm the selected child
        enterChildAtIndex(index);
    }
}

void DeviceSelectorInputHandler::enterDeviceAtIndex(int selectorIndex) {
    if (deviceList_.isNested && selectorIndex == 0) {
        protocol_.send(Protocol::ExitToParentMessage{});
        return;
    }

    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    if (deviceIndex < 0 || deviceIndex >= deviceList_.count) return;

    if (hasChildren(deviceIndex)) {
        navigation_.deviceIndex = deviceIndex;
        protocol_.send(Protocol::RequestDeviceChildrenMessage{static_cast<uint8_t>(deviceIndex), 0});
    } else {
        // No children - select/focus this device directly
        protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
    }
}

void DeviceSelectorInputHandler::enterChildAtIndex(int selectorIndex) {
    if (selectorIndex == 0) {
        protocol_.send(Protocol::RequestDeviceListMessage{});
        return;
    }

    int listIndex = selectorIndex - 1;
    uint8_t itemType = (listIndex < static_cast<int>(navigation_.itemTypes.size()))
                       ? navigation_.itemTypes[listIndex] : 0;
    uint8_t childIndex = (listIndex < static_cast<int>(navigation_.childIndices.size()))
                         ? navigation_.childIndices[listIndex] : listIndex;

    protocol_.send(Protocol::EnterDeviceChildMessage{navigation_.deviceIndex, itemType, childIndex});
}

void DeviceSelectorInputHandler::toggleState()
{
    int selectorIndex = view_.state().deviceSelector.currentIndex;
    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);

    if (deviceIndex >= 0 && deviceIndex < deviceList_.count) {
        protocol_.send(Protocol::DeviceStateChangeMessage{static_cast<uint8_t>(deviceIndex), true});
    }
}

void DeviceSelectorInputHandler::requestTrackList() {
    // Skip if track list was just closed by this same press
    if (trackHandler_.isTrackListRequested()) {
        trackHandler_.setTrackListRequested(false);
        return;
    }

    // Show cached track list immediately if available (cache-first)
    auto& state = view_.state().trackSelector;
    if (!state.names.empty() && !state.visible) {
        state.visible = true;
        view_.state().dirty.trackSelector = true;
        view_.sync();
    }

    protocol_.send(Protocol::RequestTrackListMessage{});
    trackHandler_.setTrackListRequested(true);
}

void DeviceSelectorInputHandler::close() {
    if (view_.isTrackSelectorVisible()) {
        view_.state().trackSelector.visible = false;
        view_.state().dirty.trackSelector = true;
        trackHandler_.setTrackListRequested(false);
    }

    // Hide device selector
    view_.state().deviceSelector.visible = false;
    view_.state().dirty.deviceSelector = true;
    view_.sync();

    deviceList_.requested = false;
    api_.setLatch(ButtonID::LEFT_CENTER, false);
}

// =============================================================================
// Helpers
// =============================================================================

bool DeviceSelectorInputHandler::hasChildren(uint8_t deviceIndex) const {
    return deviceIndex < Device::MAX_DEVICES &&
           deviceList_.childrenTypes[deviceIndex][0] != Device::None;
}

int DeviceSelectorInputHandler::getAdjustedDeviceIndex(int selectorIndex) const {
    return deviceList_.isNested ? selectorIndex - 1 : selectorIndex;
}

} // namespace Bitwig
