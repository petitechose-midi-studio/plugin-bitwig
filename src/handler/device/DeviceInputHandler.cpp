#include "DeviceInputHandler.hpp"

#include "DevicePageInputHandler.hpp"
#include "DeviceSelectorInputHandler.hpp"
#include "MacroInputHandler.hpp"
#include "TrackInputHandler.hpp"

#include "ui/device/DeviceView.hpp"

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

DeviceInputHandler::DeviceInputHandler(ControllerAPI& api, DeviceView& view,
                                       Protocol::Protocol& protocol, lv_obj_t* scope)
    : api_(api), scope_(scope) {
    macro_handler_ = std::make_unique<MacroInputHandler>(api, view, protocol, scope);
    page_handler_ = std::make_unique<DevicePageInputHandler>(api, view, protocol, scope);
    track_handler_ = std::make_unique<TrackInputHandler>(api, view, protocol);
    device_selector_handler_ =
        std::make_unique<DeviceSelectorInputHandler>(api, view, protocol, *track_handler_, scope);
}

DeviceInputHandler::~DeviceInputHandler() { api_.clearScope(scope_); }

// =============================================================================
// Public API - Delegation to Sub-handlers
// =============================================================================

void DeviceInputHandler::setPageSelectionState(uint8_t pageCount, uint8_t currentIndex) {
    page_handler_->setPageSelectionState(pageCount, currentIndex);
}

void DeviceInputHandler::setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex,
                                            bool isNested,
                                            const std::array<uint8_t, 4>* childrenTypes,
                                            uint8_t childrenTypesCount) {
    device_selector_handler_->setDeviceListState(deviceCount, currentDeviceIndex, isNested,
                                                 childrenTypes, childrenTypesCount);
}

void DeviceInputHandler::setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType,
                                                uint8_t childrenCount,
                                                const std::vector<uint8_t>& itemTypes,
                                                const std::vector<uint8_t>& childIndices) {
    device_selector_handler_->setDeviceChildrenState(deviceIndex, childType, childrenCount,
                                                     itemTypes, childIndices);
}

void DeviceInputHandler::setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex,
                                           bool isNested) {
    track_handler_->setTrackListState(trackCount, currentTrackIndex, isNested);
}

bool DeviceInputHandler::isDeviceListRequested() const {
    return device_selector_handler_->isRequested();
}

bool DeviceInputHandler::isTrackListRequested() const {
    return track_handler_->isTrackListRequested();
}

}  // namespace Bitwig
