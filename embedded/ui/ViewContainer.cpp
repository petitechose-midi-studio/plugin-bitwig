#include "ViewContainer.hpp"
#include "transportbar/TransportBar.hpp"
#include "theme/BitwigTheme.hpp"
#include "log/Macros.hpp"

namespace Plugin::Bitwig {

ViewContainer::ViewContainer(lv_obj_t* parentScreen) {
    container_ = lv_obj_create(parentScreen);
    lv_obj_set_size(container_, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(container_, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(container_, 0, 0);
    lv_obj_set_style_pad_gap(container_, 0, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_bg_color(container_, lv_color_hex(Theme::Color::BACKGROUND_FILL), 0);

    // Cacher au démarrage (splash couvre tout initialement)
    lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);

    mainZone_ = lv_obj_create(container_);
    lv_obj_set_size(mainZone_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(mainZone_, 1);
    lv_obj_set_style_pad_all(mainZone_, 0, 0);
    lv_obj_set_style_pad_gap(mainZone_, 0, 0);
    lv_obj_set_style_border_width(mainZone_, 0, 0);
    lv_obj_set_style_bg_opa(mainZone_, LV_OPA_TRANSP, 0);

    lv_obj_set_layout(mainZone_, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(mainZone_, LV_FLEX_FLOW_COLUMN);

    bottomZone_ = lv_obj_create(container_);
    lv_obj_set_size(bottomZone_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(bottomZone_, 0, 0);
    lv_obj_set_style_border_width(bottomZone_, 0, 0);
    lv_obj_set_style_bg_opa(bottomZone_, LV_OPA_TRANSP, 0);
}

ViewContainer::~ViewContainer() {
    if (container_) {
        lv_obj_delete(container_);
    }
}

}  // namespace Plugin::Bitwig
