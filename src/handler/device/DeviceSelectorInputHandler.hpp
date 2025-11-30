#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "DeviceConstants.hpp"
#include <etl/array.h>

namespace Bitwig {

class DeviceView;
class TrackInputHandler;

/**
 * @brief Hardware input handler for DeviceSelector overlay
 *
 * Handles device navigation when DeviceSelector overlay is visible:
 * - Open/close with LEFT_CENTER (latch behavior)
 * - Navigate devices with encoder
 * - Enter device children with NAV button
 * - Toggle device state
 * - Request track list
 */
class DeviceSelectorInputHandler {
public:
    DeviceSelectorInputHandler(ControllerAPI& api, DeviceView& view,
                               Protocol::Protocol& protocol, TrackInputHandler& trackHandler,
                               lv_obj_t* scope);
    ~DeviceSelectorInputHandler();

    // --- Public API ---
    void setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex, bool isNested,
                            const etl::array<uint8_t, 4>* childrenTypes, uint8_t childrenTypesCount);
    void setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType, uint8_t childrenCount,
                                const std::vector<uint8_t>& itemTypes,
                                const std::vector<uint8_t>& childIndices);
    bool isRequested() const { return deviceList_.requested; }

private:
    // --- Bindings ---
    void setupBindings();

    // --- Handlers ---
    void requestDeviceList();
    void navigate(float delta);
    void selectAndDive();
    void select();
    void enterDeviceAtIndex(int selectorIndex);
    void enterChildAtIndex(int selectorIndex);
    void toggleState();
    void requestTrackList();
    void close();

    // --- Helpers ---
    bool hasChildren(uint8_t deviceIndex) const;
    int getAdjustedDeviceIndex(int selectorIndex) const;

    // --- Dependencies ---
    ControllerAPI& api_;
    DeviceView& view_;
    Protocol::Protocol& protocol_;
    TrackInputHandler& trackHandler_;
    lv_obj_t* scope_;

    // --- Device list state ---
    struct DeviceListState {
        uint8_t count = 0;
        bool isNested = false;
        bool requested = false;
        etl::array<etl::array<uint8_t, Device::MAX_CHILD_TYPES>, Device::MAX_DEVICES> childrenTypes;
    };
    DeviceListState deviceList_;

    // --- Navigation state ---
    enum class SelectorMode { Devices, Children };
    struct NavigationState {
        SelectorMode mode = SelectorMode::Devices;
        uint8_t deviceIndex = 0;
        uint8_t childType = 0;
        uint8_t childrenCount = 0;
        etl::array<uint8_t, 16> itemTypes = {};
        etl::array<uint8_t, 16> childIndices = {};
        // currentSelectorIndex removed - use view_.state().deviceSelector.currentIndex
    };
    NavigationState navigation_;
};

} // namespace Bitwig
