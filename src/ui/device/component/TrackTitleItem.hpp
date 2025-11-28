#pragma once

#include <lvgl.h>
#include <string>
#include <cstdint>
#include <memory>
#include "ui/shared/widget/Label.hpp"

namespace Bitwig
{

    /**
     * @brief Reusable track title component with color indicator and type icon
     *
     * Displays: [color bar] [type icon] [track name]
     * Used in DeviceStateBar (top bar) and TrackListOverlay items.
     */
    class TrackTitleItem
    {
    public:
        explicit TrackTitleItem(lv_obj_t *parent);
        ~TrackTitleItem();

        void setName(const std::string &name);
        void setColor(uint32_t color);
        void setType(uint8_t trackType);

        lv_obj_t *getContainer() const { return container_; }
        lv_obj_t *getColorBar() const { return color_bar_; }
        lv_obj_t *getTypeIcon() const { return type_icon_; }

    private:
        static const char *getTrackTypeIcon(uint8_t trackType);

        lv_obj_t *container_ = nullptr;
        lv_obj_t *color_bar_ = nullptr;
        lv_obj_t *type_icon_ = nullptr;
        std::unique_ptr<Label> label_;
    };

} // namespace Bitwig
