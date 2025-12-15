#include "ParameterListWidget.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;
namespace style = oc::ui::lvgl::style;

namespace bitwig {

ParameterListWidget::ParameterListWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, int16_t discreteCount)
    : BaseParameterWidget(parent, index), discrete_count_(discreteCount) {
    createUI(width, height);
}

ParameterListWidget::~ParameterListWidget() {
    enum_widget_.reset();
}

void ParameterListWidget::createUI(lv_coord_t width, lv_coord_t height) {
    createContainerWithGrid(width, height);

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
    style::apply(value_label_).textColor(Color::TEXT_PRIMARY);
    lv_obj_set_style_text_align(value_label_, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(value_label_, bitwig_fonts.param_label, LV_STATE_DEFAULT);
    lv_obj_center(value_label_);

    createNameLabel();
}

void ParameterListWidget::setValue(float value) {
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
}

void ParameterListWidget::updateValueDisplay() {
    if (value_label_ && current_index_ < value_names_.size()) {
        lv_label_set_text(value_label_, value_names_[current_index_].c_str());
    }
}

}  // namespace bitwig
