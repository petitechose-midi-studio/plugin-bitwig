#include "DeviceInputHandler.hpp"
#include "MacroInputHandler.hpp"
#include "DevicePageInputHandler.hpp"
#include "DeviceSelectorInputHandler.hpp"
#include "TrackInputHandler.hpp"
#include "../../ui/device/DeviceView.hpp"

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

DeviceInputHandler::DeviceInputHandler(ControllerAPI& api, DeviceView& view,
                                       Protocol::Protocol& protocol, lv_obj_t* scope)
    : api_(api), scope_(scope)
{
    macroHandler_ = std::make_unique<MacroInputHandler>(api, view, protocol, scope);
    pageHandler_ = std::make_unique<DevicePageInputHandler>(api, view, protocol, scope);
    trackHandler_ = std::make_unique<TrackInputHandler>(api, view, protocol);
    deviceSelectorHandler_ = std::make_unique<DeviceSelectorInputHandler>(
        api, view, protocol, *trackHandler_, scope);
}

DeviceInputHandler::~DeviceInputHandler() {
    api_.clearScope(scope_);
}

// =============================================================================
// Public API - Delegation to Sub-handlers
// =============================================================================

void DeviceInputHandler::setPageSelectionState(uint8_t pageCount, uint8_t currentIndex) {
    pageHandler_->setPageSelectionState(pageCount, currentIndex);
}

void DeviceInputHandler::setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex,
                                            bool isNested,
                                            const etl::array<uint8_t, 4>* childrenTypes,
                                            uint8_t childrenTypesCount)
{
    deviceSelectorHandler_->setDeviceListState(deviceCount, currentDeviceIndex, isNested,
                                               childrenTypes, childrenTypesCount);
}

void DeviceInputHandler::setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType,
                                                uint8_t childrenCount,
                                                const std::vector<uint8_t>& itemTypes,
                                                const std::vector<uint8_t>& childIndices)
{
    deviceSelectorHandler_->setDeviceChildrenState(deviceIndex, childType, childrenCount,
                                                   itemTypes, childIndices);
}

void DeviceInputHandler::setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested) {
    trackHandler_->setTrackListState(trackCount, currentTrackIndex, isNested);
}

bool DeviceInputHandler::isDeviceListRequested() const {
    return deviceSelectorHandler_->isRequested();
}

bool DeviceInputHandler::isTrackListRequested() const {
    return trackHandler_->isTrackListRequested();
}

} // namespace Bitwig
