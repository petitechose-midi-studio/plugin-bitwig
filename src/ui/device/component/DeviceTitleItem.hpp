#pragma once

#include <lvgl.h>
#include <string>
#include <cstdint>
#include <memory>
#include "ui/shared/widget/Label.hpp"

namespace Bitwig
{

    /**
     * @brief Reusable device title component with state icon
     *
     * Displays: [state icon] [device name]
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

        bool isEnabled() const { return enabled_; }
        lv_obj_t *getContainer() const { return container_; }

    private:
        void updateIcon();

        lv_obj_t *container_ = nullptr;
        lv_obj_t *icon_ = nullptr;
        std::unique_ptr<Label> label_;
        IconSize icon_size_;
        bool enabled_ = false;
    };

} // namespace Bitwig
