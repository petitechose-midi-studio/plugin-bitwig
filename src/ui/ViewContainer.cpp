#include "ViewContainer.hpp"
#include "transportbar/TransportBar.hpp"
#include "theme/BitwigTheme.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    ViewContainer::ViewContainer(lv_obj_t *parentScreen)
    {
        container_ = lv_obj_create(parentScreen);
        lv_obj_set_size(container_, LV_PCT(100), LV_PCT(100));
        lv_obj_set_layout(container_, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(container_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_gap(container_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(container_, lv_color_hex(Theme::Color::BACKGROUND_FILL), LV_STATE_DEFAULT);

        // Hidden at startup (splash covers everything initially)
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);

        main_zone_ = lv_obj_create(container_);
        lv_obj_set_size(main_zone_, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_grow(main_zone_, 1);
        lv_obj_set_style_pad_all(main_zone_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_gap(main_zone_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(main_zone_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(main_zone_, LV_OPA_TRANSP, LV_STATE_DEFAULT);

        lv_obj_set_layout(main_zone_, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(main_zone_, LV_FLEX_FLOW_COLUMN);

        bottom_zone_ = lv_obj_create(container_);
        lv_obj_set_size(bottom_zone_, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_style_pad_all(bottom_zone_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(bottom_zone_, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(bottom_zone_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    }

    ViewContainer::~ViewContainer()
    {
        if (container_)
        {
            lv_obj_delete(container_);
            container_ = nullptr;
        }
    }

} // namespace Bitwig
