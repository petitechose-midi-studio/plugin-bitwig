#include "HintBar.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/theme/BitwigTheme.hpp"

namespace bitwig {

namespace style = oc::ui::lvgl::style;
using namespace bitwig::theme;

HintBar::HintBar(lv_obj_t* parent, HintBarPosition position)
    : position_(position), parent_(parent) {
    if (!parent_) return;

    container_ = lv_obj_create(parent_);
    if (!container_) return;

    style::apply(container_).transparent().noScroll();

    applyGridLayout();
}

HintBar::~HintBar() {
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void HintBar::applyGridLayout() {
    if (!container_) return;

    if (position_ == HintBarPosition::BOTTOM) {
        static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
                                             LV_GRID_TEMPLATE_LAST};
        static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

        lv_obj_set_size(container_, LV_PCT(100), size_);
        lv_obj_set_layout(container_, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
    } else {
        static const lv_coord_t col_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
        static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
                                             LV_GRID_TEMPLATE_LAST};

        lv_obj_set_size(container_, size_, LV_PCT(100));
        lv_obj_set_layout(container_, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
        lv_grid_align_t col_align =
            (position_ == HintBarPosition::LEFT) ? LV_GRID_ALIGN_START : LV_GRID_ALIGN_END;
        lv_obj_set_style_grid_column_align(container_, col_align, LV_STATE_DEFAULT);
    }
}

void HintBar::setSize(lv_coord_t size) {
    size_ = size;
    if (container_) {
        if (position_ == HintBarPosition::BOTTOM) {
            lv_obj_set_height(container_, size_);
        } else {
            lv_obj_set_width(container_, size_);
        }
    }
}

void HintBar::setCell(int index, lv_obj_t* element) {
    if (!container_ || !element || index < 0 || index >= 3) return;

    if (position_ == HintBarPosition::BOTTOM) {
        static const lv_grid_align_t h_aligns[] = {LV_GRID_ALIGN_START, LV_GRID_ALIGN_CENTER,
                                                   LV_GRID_ALIGN_END};
        lv_obj_set_grid_cell(element, h_aligns[index], index, 1, LV_GRID_ALIGN_CENTER, 0, 1);
        if (index == 0) lv_obj_set_style_pad_left(element, Layout::HINT_BAR_EDGE_PAD, LV_STATE_DEFAULT);
        else if (index == 2) lv_obj_set_style_pad_right(element, Layout::HINT_BAR_EDGE_PAD, LV_STATE_DEFAULT);
    } else {
        static const lv_grid_align_t v_aligns[] = {LV_GRID_ALIGN_START, LV_GRID_ALIGN_CENTER,
                                                   LV_GRID_ALIGN_END};
        lv_grid_align_t h_align =
            (position_ == HintBarPosition::LEFT) ? LV_GRID_ALIGN_START : LV_GRID_ALIGN_END;
        lv_obj_set_grid_cell(element, h_align, 0, 1, v_aligns[index], index, 1);
        if (index == 0) lv_obj_set_style_pad_top(element, Layout::HINT_BAR_EDGE_PAD, LV_STATE_DEFAULT);
        else if (index == 2) lv_obj_set_style_pad_bottom(element, Layout::HINT_BAR_EDGE_PAD, LV_STATE_DEFAULT);
    }
}

void HintBar::show() {
    if (container_) lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

void HintBar::hide() {
    if (container_) lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

bool HintBar::isVisible() const {
    return container_ && !lv_obj_has_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

}  // namespace bitwig
