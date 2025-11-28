#pragma once

#include <lvgl.h>
#include <string>
#include <cstdint>

namespace Bitwig
{

    /**
     * @brief Reusable device title component with state icon
     *
     * Displays: [state icon] [folder icon] [device name]
     * Creates children directly in parent (no intermediate container).
     * Used in DeviceStateBar (top bar) and DeviceListOverlay items.
     */
    class DeviceTitleItem
    {
    public:
        enum class IconSize
        {
            Small = 12,  // For top bar
            Medium = 14  // For list items
        };

        explicit DeviceTitleItem(lv_obj_t *parent, IconSize iconSize = IconSize::Medium);
        ~DeviceTitleItem();

        void setName(const std::string &name);
        void setState(bool enabled);
        void setHasChildren(bool hasChildren);

        bool isEnabled() const { return enabled_; }
        bool hasChildren() const { return has_children_; }

        /**
         * @brief Get approximate content width (sum of children widths + gaps)
         * Call after layout update for accurate results.
         */
        lv_coord_t getContentWidth() const;

    private:
        void updateIcon();
        void updateFolderIcon();

        lv_obj_t *parent_ = nullptr;
        lv_obj_t *icon_ = nullptr;
        lv_obj_t *folder_icon_ = nullptr;
        lv_obj_t *label_ = nullptr;  // Raw lv_label, owned by LVGL parent
        IconSize icon_size_;
        bool enabled_ = false;
        bool has_children_ = false;
    };

} // namespace Bitwig
