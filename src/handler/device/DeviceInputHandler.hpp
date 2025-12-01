#pragma once

#include "api/ControllerAPI.hpp"
#include "protocol/Protocol.hpp"
#include <array>
#include <memory>

namespace Bitwig {

class DeviceView;
class MacroInputHandler;
class DevicePageInputHandler;
class DeviceSelectorInputHandler;
class TrackInputHandler;

/**
 * @brief Orchestrator for all device view input handlers
 */
class DeviceInputHandler {
public:
    DeviceInputHandler(ControllerAPI& api, DeviceView& view,
                       Protocol::Protocol& protocol, lv_obj_t* scope);
    ~DeviceInputHandler();

    // --- Public API: State updates from host ---
    void setPageSelectionState(uint8_t pageCount, uint8_t currentIndex);
    void setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex, bool isNested,
                            const std::array<uint8_t, 4>* childrenTypes, uint8_t childrenTypesCount);
    void setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType, uint8_t childrenCount,
                                const std::vector<uint8_t>& itemTypes,
                                const std::vector<uint8_t>& childIndices);
    void setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested);

    // --- Public API: State queries ---
    bool isDeviceListRequested() const;
    bool isTrackListRequested() const;

private:
    ControllerAPI& api_;
    lv_obj_t* scope_;

    std::unique_ptr<MacroInputHandler> macro_handler_;
    std::unique_ptr<DevicePageInputHandler> page_handler_;
    std::unique_ptr<DeviceSelectorInputHandler> device_selector_handler_;
    std::unique_ptr<TrackInputHandler> track_handler_;
};

} // namespace Bitwig
