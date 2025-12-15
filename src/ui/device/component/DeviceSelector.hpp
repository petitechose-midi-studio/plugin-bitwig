#pragma once

#include "TrackTitleItem.hpp"

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

#include "ui/widget/HintBar.hpp"
#include "ui/widget/BaseSelector.hpp"

namespace bitwig {

struct DeviceSelectorProps {
    // Device list mode
    const std::vector<std::string> *names = nullptr;
    const std::vector<uint8_t> *deviceTypes = nullptr;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    const std::vector<bool> *deviceStates = nullptr;
    const std::vector<bool> *hasSlots = nullptr;
    const std::vector<bool> *hasLayers = nullptr;
    const std::vector<bool> *hasDrums = nullptr;

    // Children mode
    const std::vector<std::string> *childrenNames = nullptr;
    const std::vector<uint8_t> *childrenTypes = nullptr;

    // Track header
    const char *trackName = nullptr;
    uint32_t trackColor = 0xFFFFFF;
    uint8_t trackType = 0;

    int selectedIndex = 0;
    bool showingChildren = false;
    bool showFooter = false;
    bool visible = false;
};

/**
 * Device selector with hierarchical navigation.
 * Stateless - all data comes from props.
 */
class DeviceSelector : public BaseSelector {
public:
    explicit DeviceSelector(lv_obj_t *parent);
    ~DeviceSelector() override;

    void render(const DeviceSelectorProps &props);

    void updateDeviceState(int displayIndex, bool enabled);
private:
    void renderDeviceList(const DeviceSelectorProps &props);
    void renderChildren(const DeviceSelectorProps &props);
    void createHeader();
    void renderHeader(const DeviceSelectorProps &props);
    void createFooter();
    void renderFooter(const DeviceSelectorProps &props);
    void clearIndicators();
    void createIndicators(const DeviceSelectorProps &props);
    void updateIndicatorStates(const DeviceSelectorProps &props);

    static bool isNonDeviceItem(const std::string &name);
    static bool hasChildren(const DeviceSelectorProps &props, size_t index);

    lv_obj_t *createDeviceTypeIcon(lv_obj_t *parent, uint8_t deviceType);
    lv_obj_t *createDeviceStateIcon(lv_obj_t *parent, bool enabled);
    lv_obj_t *createFolderIcon(lv_obj_t *parent);

    std::vector<std::string> prev_items_;   // Cache for itemsChanged detection
    std::vector<lv_obj_t *> type_icons_;    // LVGL objects for device type
    std::vector<lv_obj_t *> state_icons_;   // LVGL objects for device state
    std::vector<lv_obj_t *> folder_icons_;  // LVGL objects for folder indicators

    lv_obj_t *header_ = nullptr;
    std::unique_ptr<TrackTitleItem> track_header_;

    std::unique_ptr<HintBar> footer_;
    lv_obj_t *footer_track_ = nullptr;
    lv_obj_t *footer_state_ = nullptr;
};

}  // namespace bitwig
