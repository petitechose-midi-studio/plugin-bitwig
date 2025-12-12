#include "ParameterListWidget.hpp"

#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;

namespace Bitwig {

ParameterListWidget::ParameterListWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, int16_t discreteCount)
    : index_(index), discrete_count_(discreteCount) {
    createUI(width, height);
}

ParameterListWidget::~ParameterListWidget() {
    enum_widget_.reset();
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void ParameterListWidget::createUI(lv_coord_t width, lv_coord_t height) {
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

    // Enum widget container
    lv_obj_t* enum_container = lv_obj_create(container_);
    lv_obj_set_size(enum_container, 70, 50);
    lv_obj_set_style_bg_opa(enum_container, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(enum_container, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(enum_container, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(enum_container, LV_OBJ_FLAG_SCROLLABLE);

    // Enum widget
    enum_widget_ = std::make_unique<oc::ui::lvgl::EnumWidget>(enum_container);
    enum_widget_->lineColor(Color::DATA_INACTIVE).flashColor(Color::DATA_ACTIVE);

    // Value label inside enum widget
    value_label_ = lv_label_create(enum_widget_->inner());
    lv_label_set_text(value_label_, "");
    lv_obj_set_style_text_color(value_label_, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(value_label_, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_center(value_label_);
}

void ParameterListWidget::setName(const std::string& name) {
    if (name_label_) {
        lv_label_set_text(name_label_, name.c_str());
    }
}

void ParameterListWidget::setValue(float value) {
    // Convert normalized value to index
    if (discrete_count_ > 0) {
        current_index_ = static_cast<uint8_t>(value * (discrete_count_ - 1));
        updateValueDisplay();
    }
}

void ParameterListWidget::setValueWithDisplay(float value, const char* displayValue) {
    if (discrete_count_ > 0) {
        current_index_ = static_cast<uint8_t>(value * (discrete_count_ - 1));
    }
    if (value_label_ && displayValue) {
        lv_label_set_text(value_label_, displayValue);
    }
    if (enum_widget_) {
        enum_widget_->triggerFlash();
    }
}

void ParameterListWidget::setDiscreteMetadata(int16_t discreteCount,
                                              const std::vector<std::string>& valueNames,
                                              uint8_t currentIndex) {
    discrete_count_ = discreteCount;
    value_names_ = valueNames;
    current_index_ = currentIndex;
    updateValueDisplay();
}

void ParameterListWidget::setVisible(bool visible) {
    if (container_) {
        if (visible) {
            lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void ParameterListWidget::updateValueDisplay() {
    if (value_label_ && current_index_ < value_names_.size()) {
        lv_label_set_text(value_label_, value_names_[current_index_].c_str());
    }
}

}  // namespace Bitwig
