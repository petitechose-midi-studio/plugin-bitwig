#include "ButtonHintBar.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/shared/font/binary_font_buffer.hpp"

namespace Bitwig
{

ButtonHintBar::ButtonHintBar(lv_obj_t *parent)
    : parent_(parent)
{
    createLayout();
}

ButtonHintBar::~ButtonHintBar()
{
    if (container_)
    {
        lv_obj_del(container_);
        container_ = nullptr;
    }
}

void ButtonHintBar::createLayout()
{
    if (!parent_)
        return;

    // Create container anchored at bottom-center of parent
    container_ = lv_obj_create(parent_);
    lv_obj_add_flag(container_, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_size(container_, LV_PCT(100), 30);
    lv_obj_align(container_, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_pad_left(container_, 10, 0);
    lv_obj_set_style_pad_right(container_, 10, 0);
    lv_obj_set_style_pad_top(container_, 0, 0);
    lv_obj_set_style_pad_bottom(container_, 0, 0);

    // Grid layout: 3 equal columns
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);

    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(container_, 10, 0);

    // Left label (cell 0, 0) - aligned start
    left_label_ = lv_label_create(container_);
    lv_label_set_text(left_label_, "");
    applyStyle(left_label_);
    lv_obj_set_grid_cell(left_label_, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    // Center label (cell 1, 0) - centered
    center_label_ = lv_label_create(container_);
    lv_label_set_text(center_label_, "");
    applyStyle(center_label_);
    lv_obj_set_grid_cell(center_label_, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    // Right label (cell 2, 0) - aligned end
    right_label_ = lv_label_create(container_);
    lv_label_set_text(right_label_, "");
    applyStyle(right_label_);
    lv_obj_set_grid_cell(right_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
}

void ButtonHintBar::applyStyle(lv_obj_t *label)
{
    lv_obj_set_style_text_color(label, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
    if (fonts.tempo_label)
    {
        lv_obj_set_style_text_font(label, fonts.tempo_label, 0);
    }
}

void ButtonHintBar::setLeftLabel(const char *text)
{
    if (left_label_)
        lv_label_set_text(left_label_, text ? text : "");
}

void ButtonHintBar::setCenterLabel(const char *text)
{
    if (center_label_)
        lv_label_set_text(center_label_, text ? text : "");
}

void ButtonHintBar::setRightLabel(const char *text)
{
    if (right_label_)
        lv_label_set_text(right_label_, text ? text : "");
}

void ButtonHintBar::show()
{
    if (container_)
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

void ButtonHintBar::hide()
{
    if (container_)
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

} // namespace Bitwig
