#pragma once

#include "widget/BaseSelector.hpp"
#include "ButtonHintBar.hpp"
#include <string>
#include <vector>
#include <memory>

namespace Bitwig
{

/**
 * Device selector with hierarchical navigation.
 * Supports device list, children (slots/layers/drums), and state indicators.
 */
class DeviceSelector : public UI::BaseSelector
{
public:
    explicit DeviceSelector(lv_obj_t *parent);
    ~DeviceSelector() override;

    void setItems(const std::vector<std::string> &items);

    void setDeviceItems(const std::vector<std::string> &names,
                        int currentIndex,
                        const std::vector<bool> &deviceStates,
                        const std::vector<bool> &hasSlots,
                        const std::vector<bool> &hasLayers,
                        const std::vector<bool> &hasDrums);

    void setChildrenItems(const std::vector<std::string> &items,
                          const std::vector<uint8_t> &itemTypes);

    void setCurrentItemIndex(int index);
    void setDeviceStateAtIndex(int displayIndex, bool enabled);

    void show() override;
    void showWithFooter();
    void showWithoutFooter();
    void hide() override;

private:
    void createFooter();
    void clearIndicators();
    void createIndicators();
    bool isNonDeviceItem(size_t index) const;
    bool hasChildren(size_t index) const;
    lv_obj_t *createDeviceStateIcon(lv_obj_t *parent, bool enabled);
    lv_obj_t *createFolderIcon(lv_obj_t *parent);

    std::vector<std::string> items_;
    std::vector<bool> device_states_;
    std::vector<bool> has_slots_;
    std::vector<bool> has_layers_;
    std::vector<bool> has_drums_;
    std::vector<lv_obj_t *> folder_icons_;
    int current_item_index_ = 0;

    std::unique_ptr<ButtonHintBar> footer_;
};

} // namespace Bitwig
