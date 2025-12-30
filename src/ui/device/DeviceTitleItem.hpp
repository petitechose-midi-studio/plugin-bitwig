#pragma once

#include <cstdint>
#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IListItem.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

namespace bitwig {

struct DeviceTitleItemProps {
    const char *name = "";
    uint8_t deviceType = 0;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    bool enabled = false;
    bool hasChildren = false;
};

/**
 * @brief Device title item for device selector lists
 *
 * Displays device name, type icon, state icon and folder indicator.
 * Implements IListItem for consistent handling in VirtualList.
 */
class DeviceTitleItem : public oc::ui::lvgl::IListItem {
public:
    enum class IconSize { SMALL = 12, MEDIUM = 14 };

    explicit DeviceTitleItem(lv_obj_t *parent, IconSize iconSize = IconSize::MEDIUM);
    ~DeviceTitleItem() override;

    DeviceTitleItem(const DeviceTitleItem&) = delete;
    DeviceTitleItem& operator=(const DeviceTitleItem&) = delete;

    void render(const DeviceTitleItemProps &props);
    lv_coord_t getContentWidth() const;

    // IListItem
    void setHighlighted(bool highlighted) override;

    // IComponent
    void show() override;
    void hide() override;
    bool isVisible() const override { return visible_; }
    lv_obj_t* getElement() const override { return container_; }

private:
    void updateTypeIcon(uint8_t deviceType);
    void updateStateIcon(bool enabled);
    void updateFolderIcon(bool hasChildren);

    lv_obj_t *container_ = nullptr;
    lv_obj_t *type_icon_ = nullptr;   // Device type: Audio/Instrument/Note
    lv_obj_t *state_icon_ = nullptr;  // Device state: ON/OFF
    lv_obj_t *folder_icon_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::Label> label_;
    IconSize icon_size_;
    bool visible_ = true;
    bool highlighted_ = false;
};

}  // namespace bitwig
