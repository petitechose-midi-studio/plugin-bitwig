#include "ParameterButtonWidget.hpp"

#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;

namespace bitwig {

ParameterButtonWidget::ParameterButtonWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                             uint8_t index)
    : BaseParameterWidget(parent, index) {
    createUI(width, height);
}

ParameterButtonWidget::~ParameterButtonWidget() {
    button_.reset();
}

void ParameterButtonWidget::createUI(lv_coord_t width, lv_coord_t height) {
    createContainerWithGrid(width, height);

    // Button widget - self-sizing, centered in grid cell
    button_ = std::make_unique<oc::ui::lvgl::ButtonWidget>(container_);
    button_->offColor(Color::KNOB_BACKGROUND)
        .onColor(Color::DATA_ACTIVE)
        .textOffColor(Color::TEXT_PRIMARY)
        .textOnColor(Color::TEXT_DARK);
    lv_obj_set_grid_cell(button_->getElement(),
        LV_GRID_ALIGN_CENTER, 0, 1,   // Horizontal: center
        LV_GRID_ALIGN_CENTER, 0, 1);  // Vertical: center in FR(1) space

    createNameLabel();
}

void ParameterButtonWidget::setValue(float value) {
    if (button_) {
        button_->setState(value > 0.5f);
    }
}

void ParameterButtonWidget::setValueWithDisplay(float value, const char* displayValue) {
    (void)displayValue;
    setValue(value);
}

}  // namespace bitwig
