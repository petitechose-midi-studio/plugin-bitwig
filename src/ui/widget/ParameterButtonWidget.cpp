#include "ParameterButtonWidget.hpp"

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;

namespace bitwig {

ParameterButtonWidget::ParameterButtonWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                             uint8_t index)
    : parent_(parent ? parent : lv_screen_active()), index_(index) {
    createUI(width, height);
}

ParameterButtonWidget::~ParameterButtonWidget() {
    button_.reset();
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void ParameterButtonWidget::createUI(lv_coord_t width, lv_coord_t height) {
    // Container with grid layout: widget row (flexible) + label row (content)
    container_ = lv_obj_create(parent_);
    lv_obj_set_size(container_, width, height);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(container_, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);

    // Grid: 1 column, 2 rows (FR(1) for widget, CONTENT for label)
    static const int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);

    // Button widget - self-sizing, centered in grid cell
    button_ = std::make_unique<oc::ui::lvgl::ButtonWidget>(container_);
    button_->offColor(Color::KNOB_BACKGROUND)
        .onColor(Color::DATA_ACTIVE)
        .textOffColor(Color::TEXT_PRIMARY)
        .textOnColor(Color::TEXT_DARK);
    lv_obj_set_grid_cell(button_->getElement(),
        LV_GRID_ALIGN_CENTER, 0, 1,   // Horizontal: center
        LV_GRID_ALIGN_CENTER, 0, 1);  // Vertical: center in FR(1) space

    // Name label - stretched width, content height
    name_label_ = lv_label_create(container_);
    lv_label_set_text(name_label_, "");
    lv_obj_set_style_text_color(name_label_, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(name_label_, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(name_label_, bitwig_fonts.param_label, LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(name_label_,
        LV_GRID_ALIGN_STRETCH, 0, 1,  // Horizontal: full width
        LV_GRID_ALIGN_CENTER, 1, 1);  // Vertical: center in CONTENT row
}

void ParameterButtonWidget::setName(const std::string& name) {
    if (name_label_) {
        lv_label_set_text(name_label_, name.c_str());
    }
}

void ParameterButtonWidget::setValue(float value) {
    if (button_) {
        button_->setState(value > 0.5f);
    }
}

void ParameterButtonWidget::setValueWithDisplay(float value, const char* displayValue) {
    // BUTTON: only update state (color), no text display
    (void)displayValue;
    if (button_) {
        button_->setState(value > 0.5f);
    }
}

void ParameterButtonWidget::setDiscreteMetadata(int16_t discreteCount,
                                                const std::vector<std::string>& valueNames,
                                                uint8_t currentIndex) {
    // BUTTON: ignore metadata, just use color for on/off
    (void)discreteCount;
    (void)valueNames;
    (void)currentIndex;
}

void ParameterButtonWidget::setVisible(bool visible) {
    if (container_) {
        if (visible) {
            lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

}  // namespace bitwig
