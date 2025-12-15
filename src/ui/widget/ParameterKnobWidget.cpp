#include "ParameterKnobWidget.hpp"

#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;

namespace bitwig {

ParameterKnobWidget::ParameterKnobWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, bool centered)
    : BaseParameterWidget(parent, index) {
    createUI(width, height, centered);
}

ParameterKnobWidget::~ParameterKnobWidget() {
    knob_.reset();
}

void ParameterKnobWidget::createUI(lv_coord_t width, lv_coord_t height, bool centered) {
    createContainerWithGrid(width, height);

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

    createNameLabel();
}

void ParameterKnobWidget::setValue(float value) {
    if (knob_) {
        knob_->setValue(value);
    }
}

void ParameterKnobWidget::setValueWithDisplay(float value, const char* displayValue) {
    (void)displayValue;
    setValue(value);
}

void ParameterKnobWidget::setOrigin(float origin) {
    if (knob_) {
        knob_->origin(origin);
    }
}

}  // namespace bitwig
