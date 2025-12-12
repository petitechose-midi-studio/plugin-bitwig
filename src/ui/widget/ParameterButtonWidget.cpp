#include "ParameterButtonWidget.hpp"

#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;

namespace Bitwig {

ParameterButtonWidget::ParameterButtonWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                             uint8_t index)
    : index_(index) {
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

    // Button widget container
    lv_obj_t* button_container = lv_obj_create(container_);
    lv_obj_set_size(button_container, 60, 60);
    lv_obj_set_style_bg_opa(button_container, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(button_container, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(button_container, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(button_container, LV_OBJ_FLAG_SCROLLABLE);

    // Button widget
    button_ = std::make_unique<oc::ui::lvgl::ButtonWidget>(button_container);
    button_->offColor(Color::KNOB_BACKGROUND)
        .onColor(Color::DATA_ACTIVE)
        .textOffColor(Color::TEXT_PRIMARY)
        .textOnColor(Color::TEXT_DARK);
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
    if (button_) {
        button_->setState(value > 0.5f);
        if (displayValue) {
            button_->setText(displayValue);
        }
    }
}

void ParameterButtonWidget::setDiscreteMetadata(int16_t discreteCount,
                                                const std::vector<std::string>& valueNames,
                                                uint8_t currentIndex) {
    (void)discreteCount;
    value_names_ = valueNames;

    // Update button text based on current state
    if (button_ && currentIndex < value_names_.size()) {
        button_->setText(value_names_[currentIndex].c_str());
    }
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

}  // namespace Bitwig
