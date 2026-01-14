#pragma once

/**
 * @file DeviceTitleItem.hpp
 * @brief List item widget for device entries in DeviceSelector
 *
 * Displays a single device row with:
 * - Type icon (Audio FX, Instrument, Note FX)
 * - State icon (enabled/disabled indicator)
 * - Device name label
 * - Folder icon (for devices with children: chains, layers, drums)
 *
 * Implements IListItem for VirtualList slot recycling.
 *
 * @see DeviceSelector for the parent list component
 * @see DeviceStateBar for device display in the top bar
 */

#include <cstdint>
#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IListItem.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include "protocol/DeviceType.hpp"

namespace bitwig::ui {

struct DeviceTitleItemProps {
    const char *name = "";
    DeviceType deviceType = DeviceType::UNKNOWN;
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
    enum class IconSize : uint8_t { SMALL = 12, MEDIUM = 14 };

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
    void updateTypeIcon(DeviceType deviceType);
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

}  // namespace bitwig::ui
