#include "LevelBar.hpp"
#include <algorithm>

namespace Bitwig
{

LevelBar::LevelBar(lv_obj_t *parent, lv_coord_t width, lv_coord_t height)
    : parent_(parent), width_(width), height_(height)
{
}

LevelBar::~LevelBar()
{
    if (bar_)
        lv_obj_delete(bar_);
}

void LevelBar::ensureCreated()
{
    if (bar_ || !parent_)
        return;

    bar_ = lv_bar_create(parent_);
    if (!bar_)
        return;

    lv_obj_set_size(bar_, width_, height_);
    lv_bar_set_range(bar_, 0, 100);

    // Background style (dark)
    lv_obj_set_style_bg_color(bar_, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar_, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_, 0, LV_PART_MAIN);

    // Indicator style
    lv_obj_set_style_radius(bar_, 0, LV_PART_INDICATOR);
}

void LevelBar::render(const LevelBarProps &props)
{
    ensureCreated();
    if (!bar_)
        return;

    float value = std::clamp(props.value, 0.0f, 1.0f);
    lv_bar_set_value(bar_, static_cast<int32_t>(value * 100), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(bar_, lv_color_hex(props.color), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar_, props.opacity, LV_PART_INDICATOR);
}

} // namespace Bitwig
