#pragma once

#include <lvgl.h>
#include <cstdint>

namespace Bitwig
{

    /**
     * @brief Simple horizontal level bar displaying a normalized value (0.0-1.0)
     *
     * Displays a colored bar with configurable opacity representing a value.
     * Used for track volume/level display in TrackListOverlay.
     * Uses lazy initialization - LVGL widget created on first use.
     */
    class LevelBar
    {
    public:
        LevelBar(lv_obj_t *parent, lv_coord_t width = 60, lv_coord_t height = LV_PCT(100));
        ~LevelBar();

        void setValue(float normalizedValue);
        void setColor(uint32_t color);
        void setOpacity(lv_opa_t opacity);

    private:
        void ensureCreated();
        void updateVisualState();

        lv_obj_t *parent_ = nullptr;
        lv_obj_t *bar_ = nullptr;
        lv_coord_t width_ = 60;
        lv_coord_t height_ = 12;
        uint32_t color_ = 0xFFFFFF;
        lv_opa_t opacity_ = LV_OPA_50;
        float value_ = 0.0f;
    };

} // namespace Bitwig
