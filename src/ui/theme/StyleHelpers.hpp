#pragma once

#include <lvgl.h>
#include "BitwigTheme.hpp"

namespace Theme {
namespace Style {

/**
 * @brief Apply transparent container style (no bg, no border, no padding)
 * Common pattern for layout containers that should be invisible.
 */
inline void applyTransparentContainer(lv_obj_t* obj) {
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
}

/**
 * @brief Apply flex row layout with center vertical alignment
 * @param obj Target object
 * @param hAlign Horizontal alignment (default: START)
 * @param gap Gap between items (default: GAP_MD)
 */
inline void applyFlexRow(lv_obj_t* obj,
                         lv_flex_align_t hAlign = LV_FLEX_ALIGN_START,
                         int16_t gap = Layout::GAP_MD) {
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(obj, hAlign, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(obj, gap, 0);
}

/**
 * @brief Apply flex column layout with center horizontal alignment
 * @param obj Target object
 * @param vAlign Vertical alignment (default: START)
 * @param gap Gap between items (default: GAP_MD)
 */
inline void applyFlexColumn(lv_obj_t* obj,
                            lv_flex_align_t vAlign = LV_FLEX_ALIGN_START,
                            int16_t gap = Layout::GAP_MD) {
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, vAlign, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(obj, gap, 0);
}

/**
 * @brief Create a transparent flex row container
 * @param parent Parent object
 * @param hAlign Horizontal alignment
 * @param gap Gap between items
 * @return Created container
 */
inline lv_obj_t* createFlexRowContainer(lv_obj_t* parent,
                                        lv_flex_align_t hAlign = LV_FLEX_ALIGN_START,
                                        int16_t gap = Layout::GAP_MD) {
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    applyTransparentContainer(container);
    applyFlexRow(container, hAlign, gap);
    return container;
}

/**
 * @brief Apply horizontal padding only
 */
inline void applyPadHorizontal(lv_obj_t* obj, int16_t pad) {
    lv_obj_set_style_pad_left(obj, pad, 0);
    lv_obj_set_style_pad_right(obj, pad, 0);
}

/**
 * @brief Apply vertical padding only
 */
inline void applyPadVertical(lv_obj_t* obj, int16_t pad) {
    lv_obj_set_style_pad_top(obj, pad, 0);
    lv_obj_set_style_pad_bottom(obj, pad, 0);
}

/**
 * @brief Set text color from theme color constant
 */
inline void setTextColor(lv_obj_t* obj, uint32_t color) {
    lv_obj_set_style_text_color(obj, lv_color_hex(color), 0);
}

/**
 * @brief Set background color from theme color constant
 */
inline void setBgColor(lv_obj_t* obj, uint32_t color, uint8_t opacity = Opacity::FULL) {
    lv_obj_set_style_bg_color(obj, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(obj, opacity, 0);
}

}  // namespace Style
}  // namespace Theme
