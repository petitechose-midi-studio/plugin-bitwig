#include "ParameterListWidget.hpp"

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;

namespace bitwig {

ParameterListWidget::ParameterListWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, int16_t discreteCount)
    : parent_(parent ? parent : lv_screen_active()),
      index_(index),
      discrete_count_(discreteCount) {
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

    // Enum widget - stretched to fill grid cell
    enum_widget_ = std::make_unique<oc::ui::lvgl::EnumWidget>(container_);
    enum_widget_->lineColor(Color::DATA_INACTIVE).flashColor(Color::DATA_ACTIVE);
    lv_obj_set_grid_cell(enum_widget_->getElement(),
        LV_GRID_ALIGN_STRETCH, 0, 1,  // Horizontal: full width
        LV_GRID_ALIGN_STRETCH, 0, 1); // Vertical: fill FR(1) space

    // Value label inside enum widget's inner area
    value_label_ = lv_label_create(enum_widget_->inner());
    lv_label_set_text(value_label_, "");
    lv_obj_set_size(value_label_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_text_color(value_label_, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(value_label_, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(value_label_, bitwig_fonts.param_label, LV_STATE_DEFAULT);
    lv_obj_center(value_label_);

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
    // Note: Don't call updateValueDisplay() here - setValueWithDisplay() handles the display
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

}  // namespace bitwig
