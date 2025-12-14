#include "ParameterKnobWidget.hpp"

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;

namespace Bitwig {

ParameterKnobWidget::ParameterKnobWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, bool centered)
    : parent_(parent ? parent : lv_screen_active()), index_(index) {
    createUI(width, height, centered);
}

ParameterKnobWidget::~ParameterKnobWidget() {
    knob_.reset();
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void ParameterKnobWidget::createUI(lv_coord_t width, lv_coord_t height, bool centered) {
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

    // Knob widget - self-sizing, centered horizontally, aligned to bottom (closer to label)
    knob_ = std::make_unique<oc::ui::lvgl::KnobWidget>(container_);
    knob_->centered(centered)
        .bgColor(Color::KNOB_BACKGROUND)
        .trackColor(Color::KNOB_VALUE_RIBBON)
        .valueColor(Color::KNOB_VALUE_INDICATOR)
        .flashColor(Color::DATA_ACTIVE);
    lv_obj_set_grid_cell(knob_->getElement(),
        LV_GRID_ALIGN_CENTER, 0, 1,  // Horizontal: center
        LV_GRID_ALIGN_END, 0, 1);    // Vertical: bottom of FR(1) space

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

void ParameterKnobWidget::setName(const std::string& name) {
    if (name_label_) {
        lv_label_set_text(name_label_, name.c_str());
    }
}

void ParameterKnobWidget::setValue(float value) {
    if (knob_) {
        knob_->setValue(value);
    }
}

void ParameterKnobWidget::setValueWithDisplay(float value, const char* displayValue) {
    // KNOB: only update knob value, no text display
    (void)displayValue;
    setValue(value);
}

void ParameterKnobWidget::setVisible(bool visible) {
    if (container_) {
        if (visible) {
            lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void ParameterKnobWidget::setOrigin(float origin) {
    if (knob_) {
        knob_->origin(origin);
    }
}

}  // namespace Bitwig
