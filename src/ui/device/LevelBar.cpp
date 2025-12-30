#include "LevelBar.hpp"

#include <algorithm>

#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;

namespace bitwig {

LevelBar::LevelBar(lv_obj_t *parent, lv_coord_t width, lv_coord_t height)
    : width_(width), height_(height) {
    if (!parent) return;

    bar_ = lv_bar_create(parent);
    if (!bar_) return;

    lv_obj_set_size(bar_, width_, height_);
    lv_bar_set_range(bar_, 0, 100);

    // Background style (dark)
    lv_obj_set_style_bg_color(bar_, lv_color_hex(Color::KNOB_BACKGROUND), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar_, Opacity::FULL, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_, 0, LV_PART_MAIN);

    // Indicator style
    lv_obj_set_style_radius(bar_, 0, LV_PART_INDICATOR);
}

LevelBar::~LevelBar() {
    if (bar_) {
        lv_obj_delete(bar_);
        bar_ = nullptr;
    }
}

void LevelBar::render(const LevelBarProps &props) {
    if (!bar_) return;

    float value = std::clamp(props.value, 0.0f, 1.0f);
    lv_bar_set_value(bar_, static_cast<int32_t>(value * 100), LV_ANIM_OFF);
    lv_obj_set_style_bg_color(bar_, lv_color_hex(props.color), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar_, props.opacity, LV_PART_INDICATOR);
}

}  // namespace bitwig
