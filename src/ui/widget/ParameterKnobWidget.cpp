#include "ParameterKnobWidget.hpp"

#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;

namespace Bitwig {

ParameterKnobWidget::ParameterKnobWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, bool centered)
    : index_(index) {
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
    container_ = lv_obj_create(lv_obj_get_parent(lv_screen_active()));
    lv_obj_set_size(container_, width, height);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(container_, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);

    // Flex column layout
    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    // Name label (top)
    name_label_ = lv_label_create(container_);
    lv_label_set_text(name_label_, "");
    lv_obj_set_style_text_color(name_label_, lv_color_hex(Color::TEXT_LIGHT), LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(name_label_, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);

    // Knob container
    lv_obj_t* knob_container = lv_obj_create(container_);
    lv_obj_set_size(knob_container, 60, 60);
    lv_obj_set_style_bg_opa(knob_container, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(knob_container, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(knob_container, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(knob_container, LV_OBJ_FLAG_SCROLLABLE);

    // Knob widget
    knob_ = std::make_unique<oc::ui::lvgl::KnobWidget>(knob_container);
    knob_->centered(centered)
        .bgColor(Color::KNOB_BACKGROUND)
        .trackColor(Color::KNOB_VALUE_RIBBON)
        .valueColor(Color::KNOB_VALUE_INDICATOR)
        .flashColor(Color::DATA_ACTIVE);

    // Value label (bottom)
    value_label_ = lv_label_create(container_);
    lv_label_set_text(value_label_, "");
    lv_obj_set_style_text_color(value_label_, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(value_label_, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
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
    setValue(value);
    if (value_label_ && displayValue) {
        lv_label_set_text(value_label_, displayValue);
    }
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
