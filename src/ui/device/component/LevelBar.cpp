#include "LevelBar.hpp"

namespace Bitwig
{

    LevelBar::LevelBar(lv_obj_t *parent, lv_coord_t width, lv_coord_t height)
        : parent_(parent), width_(width), height_(height)
    {
        // Lazy init - don't create LVGL widget here
    }

    LevelBar::~LevelBar()
    {
        // LVGL parent handles cleanup
    }

    void LevelBar::ensureCreated()
    {
        if (bar_ || !parent_) return;

        bar_ = lv_bar_create(parent_);
        if (!bar_) return;

        lv_obj_set_size(bar_, width_, height_);
        lv_bar_set_range(bar_, 0, 100);
        lv_bar_set_value(bar_, static_cast<int32_t>(value_ * 100), LV_ANIM_OFF);

        // Background style (dark)
        lv_obj_set_style_bg_color(bar_, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(bar_, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_radius(bar_, 0, LV_PART_MAIN);

        // Indicator style
        lv_obj_set_style_radius(bar_, 0, LV_PART_INDICATOR);

        updateVisualState();
    }

    void LevelBar::setValue(float normalizedValue)
    {
        if (normalizedValue < 0.0f) normalizedValue = 0.0f;
        if (normalizedValue > 1.0f) normalizedValue = 1.0f;

        value_ = normalizedValue;

        ensureCreated();
        if (bar_)
        {
            lv_bar_set_value(bar_, static_cast<int32_t>(value_ * 100), LV_ANIM_OFF);
        }
    }

    void LevelBar::setColor(uint32_t color)
    {
        color_ = color;
        ensureCreated();
        updateVisualState();
    }

    void LevelBar::setOpacity(lv_opa_t opacity)
    {
        opacity_ = opacity;
        ensureCreated();
        updateVisualState();
    }

    void LevelBar::updateVisualState()
    {
        if (!bar_) return;

        lv_obj_set_style_bg_color(bar_, lv_color_hex(color_), LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(bar_, opacity_, LV_PART_INDICATOR);
    }

} // namespace Bitwig
