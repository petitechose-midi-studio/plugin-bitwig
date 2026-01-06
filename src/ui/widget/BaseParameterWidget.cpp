#include "BaseParameterWidget.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;
namespace style = oc::ui::lvgl::style;

namespace bitwig::ui {

BaseParameterWidget::BaseParameterWidget(lv_obj_t* parent, uint8_t index)
    : parent_(parent ? parent : lv_screen_active()), index_(index) {}

BaseParameterWidget::~BaseParameterWidget() {
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void BaseParameterWidget::createContainerWithGrid(lv_coord_t width, lv_coord_t height) {
    container_ = lv_obj_create(parent_);
    lv_obj_set_size(container_, width, height);
    style::apply(container_).transparent().noScroll();

    // Grid: 1 column, 2 rows (CONTENT for widget, CONTENT for label)
    // Widget uses SquareSizePolicy to set its height = width automatically
    static const int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const int32_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);
}

void BaseParameterWidget::createNameLabel() {
    // Use framework Label widget with auto-scroll for overflow text
    name_label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    name_label_->alignment(LV_TEXT_ALIGN_CENTER)
               .color(Color::TEXT_PRIMARY)
               .font(bitwig_fonts.param_label)
               .ownsLvglObjects(false);

    // Offset text slightly upward, ensure no top clipping
    lv_obj_set_style_pad_top(name_label_->getElement(), 0, 0);
    lv_obj_set_style_translate_y(name_label_->getElement(), Layout::PARAMETER_LABEL_OFFSET, 0);

    lv_obj_set_grid_cell(name_label_->getElement(),
        LV_GRID_ALIGN_STRETCH, 0, 1,  // Horizontal: full width
        LV_GRID_ALIGN_CENTER, 1, 1);  // Vertical: center in CONTENT row
}

void BaseParameterWidget::setName(const std::string& name) {
    if (name_label_) {
        name_label_->setText(name);
    }
}

void BaseParameterWidget::setVisible(bool visible) {
    if (container_) {
        if (visible) {
            lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

}  // namespace bitwig
