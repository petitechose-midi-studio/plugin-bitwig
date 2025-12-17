#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include <oc/ui/lvgl/widget/VirtualList.hpp>

#include "ui/track/TrackTitleItem.hpp"
#include "ui/widget/HintBar.hpp"

namespace bitwig {

// Import VirtualList types from shared component
using oc::ui::lvgl::widget::VirtualList;
using oc::ui::lvgl::widget::VirtualSlot;
using oc::ui::lvgl::widget::ScrollMode;

struct DeviceSelectorProps {
    // Device list mode
    std::vector<std::string> names;
    std::vector<uint8_t> deviceTypes;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    std::vector<bool> deviceStates;
    std::vector<bool> hasSlots;
    std::vector<bool> hasLayers;
    std::vector<bool> hasDrums;

    // Children mode
    std::vector<std::string> childrenNames;
    std::vector<uint8_t> childrenTypes;

    // Track header
    const char *trackName = nullptr;
    uint32_t trackColor = 0xFFFFFF;
    uint8_t trackType = 0;

    int selectedIndex = 0;
    int totalCount = 0;      // Total devices (for pagination display)
    bool isNested = false;   // Has back button at index 0
    bool showingChildren = false;
    bool showFooter = false;
    bool visible = false;
};

/**
 * @brief Widgets inside a single VirtualList slot (reusable pool)
 */
struct DeviceSlotWidgets {
    lv_obj_t *typeIcon = nullptr;
    lv_obj_t *stateIcon = nullptr;
    lv_obj_t *label = nullptr;
    lv_obj_t *folderIcon = nullptr;
    lv_obj_t *indexLabel = nullptr;  // Item index (last element)
    bool created = false;
};

/**
 * Device selector with hierarchical navigation.
 * Uses VirtualList for O(1) rendering of large device lists.
 * Stateless - all data comes from props.
 */
class DeviceSelector : public oc::ui::lvgl::IComponent {
public:
    explicit DeviceSelector(lv_obj_t *parent);
    ~DeviceSelector() override;

    DeviceSelector(const DeviceSelector &) = delete;
    DeviceSelector &operator=(const DeviceSelector &) = delete;

    void render(const DeviceSelectorProps &props);
    void updateDeviceState(int displayIndex, bool enabled);

    // IComponent
    void show() override;
    void hide() override;
    bool isVisible() const override { return visible_; }
    lv_obj_t *getElement() const override { return overlay_; }
    lv_obj_t *getContainer() const { return container_; }

private:
    // Render modes
    void renderDeviceList(const DeviceSelectorProps &props);
    void renderChildren(const DeviceSelectorProps &props);

    // Overlay structure
    void createOverlay();
    void createHeader();
    void renderHeader(const DeviceSelectorProps &props);
    void createFooter();
    void renderFooter(const DeviceSelectorProps &props);

    // VirtualList callbacks
    void bindSlot(VirtualSlot &slot, int index, bool isSelected);
    void updateSlotHighlight(VirtualSlot &slot, bool isSelected);
    void ensureSlotWidgets(VirtualSlot &slot, int slotIndex);

    // Slot population
    void populateSlotForDevice(DeviceSlotWidgets &widgets, int index);
    void populateSlotForChild(DeviceSlotWidgets &widgets, int index);

    // Highlight
    void applyHighlightStyle(DeviceSlotWidgets &widgets, bool isSelected);
    void hideAllWidgets(DeviceSlotWidgets &widgets);

    // Widget creation
    lv_obj_t *createIcon(lv_obj_t *parent);
    lv_obj_t *createLabel(lv_obj_t *parent);

    // Helpers
    static bool isNonDeviceItem(const std::string &name);
    static bool hasChildren(const DeviceSelectorProps &props, size_t index);

    // Overlay
    lv_obj_t *parent_ = nullptr;
    lv_obj_t *overlay_ = nullptr;
    lv_obj_t *container_ = nullptr;

    // Virtual list
    std::unique_ptr<VirtualList> list_;
    std::vector<DeviceSlotWidgets> slotWidgets_;

    // Header
    lv_obj_t *header_ = nullptr;
    std::unique_ptr<TrackTitleItem> track_header_;

    // Footer
    std::unique_ptr<HintBar> footer_;
    lv_obj_t *footer_track_ = nullptr;
    lv_obj_t *footer_state_ = nullptr;

    // Pagination (in header)
    std::unique_ptr<oc::ui::lvgl::Label> page_label_;

    // State
    DeviceSelectorProps currentProps_;
    bool visible_ = false;
    bool showingChildren_ = false;
};

}  // namespace bitwig
