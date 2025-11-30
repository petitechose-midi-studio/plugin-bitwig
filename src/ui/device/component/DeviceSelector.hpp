#pragma once

#include "widget/BaseSelector.hpp"
#include "ui/shared/widget/HintBar.hpp"
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace Bitwig
{

struct DeviceSelectorProps
{
    // Device list mode
    const std::vector<std::string> *names = nullptr;
    const std::vector<uint8_t> *deviceTypes = nullptr;   // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    const std::vector<bool> *deviceStates = nullptr;
    const std::vector<bool> *hasSlots = nullptr;
    const std::vector<bool> *hasLayers = nullptr;
    const std::vector<bool> *hasDrums = nullptr;

    // Children mode
    const std::vector<std::string> *childrenNames = nullptr;
    const std::vector<uint8_t> *childrenTypes = nullptr;

    int selectedIndex = 0;
    bool showingChildren = false;
    bool showFooter = false;
    bool visible = false;
};

/**
 * Device selector with hierarchical navigation.
 * Stateless - all data comes from props.
 */
class DeviceSelector : public UI::BaseSelector
{
public:
    explicit DeviceSelector(lv_obj_t *parent);
    ~DeviceSelector() override;

    void render(const DeviceSelectorProps &props);

    void updateDeviceState(int displayIndex, bool enabled);

private:
    void renderDeviceList(const DeviceSelectorProps &props);
    void renderChildren(const DeviceSelectorProps &props);
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

    std::vector<std::string> prev_items_;  // Cache for itemsChanged detection
    std::vector<lv_obj_t *> type_icons_;   // LVGL objects for device type
    std::vector<lv_obj_t *> state_icons_;  // LVGL objects for device state
    std::vector<lv_obj_t *> folder_icons_; // LVGL objects for folder indicators

    std::unique_ptr<UI::HintBar> footer_;
    lv_obj_t* footer_track_ = nullptr;
    lv_obj_t* footer_state_ = nullptr;
};

} // namespace Bitwig
