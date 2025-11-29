#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include <memory>

namespace Bitwig {

class DeviceView;
class DeviceController;
class MacroInputHandler;
class DevicePageInputHandler;
class DeviceSelectorInputHandler;
class TrackInputHandler;

/**
 * @brief Orchestrator for all device view input handlers
 *
 * Delegates to specialized sub-handlers:
 * - MacroInputHandler: encoder/button controls for parameters
 * - DevicePageInputHandler: page selector overlay
 * - DeviceSelectorInputHandler: device navigation overlay
 * - TrackInputHandler: track list overlay
 */
class DeviceInputHandler {
public:
    DeviceInputHandler(ControllerAPI& api, DeviceView& view, DeviceController& controller,
                       Protocol::Protocol& protocol, lv_obj_t* scope);
    ~DeviceInputHandler();

    // --- Public API: State updates from host ---
    void setPageSelectionState(uint8_t pageCount, uint8_t currentIndex);
    void setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex, bool isNested,
                            const etl::array<uint8_t, 4>* childrenTypes, uint8_t childrenTypesCount);
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

    std::unique_ptr<MacroInputHandler> macroHandler_;
    std::unique_ptr<DevicePageInputHandler> pageHandler_;
    std::unique_ptr<DeviceSelectorInputHandler> deviceSelectorHandler_;
    std::unique_ptr<TrackInputHandler> trackHandler_;
};

} // namespace Bitwig
