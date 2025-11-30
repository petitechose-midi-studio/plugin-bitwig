#include "ButtonHintBar.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"
#include "font/FontLoader.hpp"

using namespace Theme;

namespace Bitwig
{

ButtonHintBar::ButtonHintBar(lv_obj_t *parent)
    : parent_(parent)
{
}

ButtonHintBar::~ButtonHintBar()
{
    if (container_)
    {
        lv_obj_del(container_);
        container_ = nullptr;
    }
}

void ButtonHintBar::ensureCreated()
{
    if (container_ || !parent_)
        return;

    container_ = lv_obj_create(parent_);
    if (!container_)
        return;

    // Floating bar at bottom
    lv_obj_add_flag(container_, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_size(container_, LV_PCT(100), Layout::BUTTON_HINT_HEIGHT);
    lv_obj_align(container_, LV_ALIGN_BOTTOM_MID, 0, 0);

    // Transparent container with horizontal padding
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    Style::applyPadHorizontal(container_, Layout::PAD_LG);
    Style::applyPadVertical(container_, 0);

    // Grid layout: 3 equal columns
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(container_, Layout::GAP_LG, 0);

    left_label_ = lv_label_create(container_);
    applyStyle(left_label_);
    lv_obj_set_grid_cell(left_label_, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    center_label_ = lv_label_create(container_);
    applyStyle(center_label_);
    lv_obj_set_grid_cell(center_label_, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    right_label_ = lv_label_create(container_);
    applyStyle(right_label_);
    lv_obj_set_grid_cell(right_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
}

void ButtonHintBar::applyStyle(lv_obj_t *label)
{
    Style::setTextColor(label, Color::TEXT_LIGHT);
    if (fonts.tempo_label)
    {
        lv_obj_set_style_text_font(label, fonts.tempo_label, 0);
    }
}

void ButtonHintBar::renderItem(lv_obj_t *label, const ButtonHintItem &item, lv_grid_align_t align, int col)
{
    if (!label)
        return;

    // Icon takes precedence over text
    if (item.icon)
    {
        Icon::set(label, item.icon, Icon::L);
        lv_obj_set_style_text_color(label, lv_color_hex(item.color), 0);
        lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    }
    else
    {
        lv_label_set_text(label, item.text ? item.text : "");
        Style::setTextColor(label, Color::TEXT_LIGHT);
        lv_obj_set_grid_cell(label, align, col, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    }

    lv_obj_set_style_text_opa(label, item.active ? Opacity::FULL : Opacity::FADED, 0);
}

void ButtonHintBar::render(const ButtonHintBarProps &props)
{
    ensureCreated();
    if (!container_)
        return;

    renderItem(left_label_, props.left, LV_GRID_ALIGN_START, 0);
    renderItem(center_label_, props.center, LV_GRID_ALIGN_CENTER, 1);
    renderItem(right_label_, props.right, LV_GRID_ALIGN_END, 2);

    if (props.visible)
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

} // namespace Bitwig
