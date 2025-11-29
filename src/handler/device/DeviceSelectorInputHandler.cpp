#include "DeviceSelectorInputHandler.hpp"
#include "TrackInputHandler.hpp"
#include "InputUtils.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../ui/device/DeviceController.hpp"
#include "../../protocol/struct/RequestDeviceListMessage.hpp"
#include "../../protocol/struct/RequestDeviceChildrenMessage.hpp"
#include "../../protocol/struct/EnterDeviceChildMessage.hpp"
#include "../../protocol/struct/ExitToParentMessage.hpp"
#include "../../protocol/struct/DeviceStateChangeMessage.hpp"
#include "../../protocol/struct/RequestTrackListMessage.hpp"

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

DeviceSelectorInputHandler::DeviceSelectorInputHandler(ControllerAPI& api, DeviceView& view,
                                                       DeviceController& controller,
                                                       Protocol::Protocol& protocol,
                                                       TrackInputHandler& trackHandler,
                                                       lv_obj_t* scope)
    : api_(api), view_(view), controller_(controller), protocol_(protocol),
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
    navigation_.currentSelectorIndex = isNested ? currentDeviceIndex + 1 : currentDeviceIndex;
    controller_.handleDeviceSelectorSetIndex(navigation_.currentSelectorIndex);
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
    navigation_.currentSelectorIndex = 1;
    controller_.handleDeviceSelectorSetIndex(1);
}

// =============================================================================
// Bindings Setup
// =============================================================================

void DeviceSelectorInputHandler::setupBindings() {
    lv_obj_t* overlay = view_.getDeviceSelectorElement();

    // Open/close device selector (latch behavior)
    api_.onPressed(ButtonID::LEFT_CENTER, [this]() { requestDeviceList(); }, scope_, true);
    api_.onReleased(ButtonID::LEFT_CENTER, [this]() { close(); }, scope_);

    // Navigate devices
    api_.onTurnedWhilePressed(EncoderID::NAV, ButtonID::LEFT_CENTER,
        [this](float delta) { navigate(delta); }, scope_);

    // Enter device children (scoped to overlay)
    api_.onPressed(ButtonID::NAV, [this]() { enterSelected(); }, overlay);

    // Toggle device state (scoped to overlay)
    api_.onReleased(ButtonID::BOTTOM_CENTER, [this]() { toggleSelectedState(); }, overlay);

    // Toggle track list (no latch, simple toggle like device state)
    api_.onPressed(ButtonID::BOTTOM_LEFT, [this]() { requestTrackList(); }, overlay);
}

// =============================================================================
// Handlers
// =============================================================================

void DeviceSelectorInputHandler::requestDeviceList() {
    if (!deviceList_.requested) {
        protocol_.send(Protocol::RequestDeviceListMessage{});
        deviceList_.requested = true;
    }
}

void DeviceSelectorInputHandler::navigate(float delta) {
    int itemCount = view_.getDeviceSelectorItemCount();
    if (itemCount == 0) return;

    navigation_.currentSelectorIndex += static_cast<int>(delta);
    navigation_.currentSelectorIndex = InputUtils::wrapIndex(navigation_.currentSelectorIndex, itemCount);
    controller_.handleDeviceSelectorSetIndex(navigation_.currentSelectorIndex);
}

void DeviceSelectorInputHandler::enterSelected() {
    int index = controller_.getDeviceSelectorSelectedIndex();
    if (index < 0) return;

    if (navigation_.mode == SelectorMode::Devices) {
        enterDeviceAtIndex(index);
    } else {
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

void DeviceSelectorInputHandler::toggleSelectedState() {
    int selectorIndex = controller_.getDeviceSelectorSelectedIndex();
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

    protocol_.send(Protocol::RequestTrackListMessage{});
    trackHandler_.setTrackListRequested(true);
}

void DeviceSelectorInputHandler::close() {
    if (view_.isTrackSelectorVisible()) {
        view_.hideTrackSelector();
        trackHandler_.setTrackListRequested(false);
    }
    controller_.handleDeviceSelectorConfirm();

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
