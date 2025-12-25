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
    automationIndicator_.reset();
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

    // Automation indicator - overlay inside button widget
    automationIndicator_ = std::make_unique<oc::ui::lvgl::StateIndicator>(
        button_->getElement(), Layout::AUTOMATION_INDICATOR_SIZE);
    automationIndicator_->color(oc::ui::lvgl::StateIndicator::State::ACTIVE, Color::AUTOMATION_INDICATOR);
    automationIndicator_->setState(oc::ui::lvgl::StateIndicator::State::OFF);
    lv_obj_add_flag(automationIndicator_->getElement(), LV_OBJ_FLAG_FLOATING);
    lv_obj_set_align(automationIndicator_->getElement(), LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(automationIndicator_->getElement(), -Layout::PAD_MD);  // 6px inside bottom
    lv_obj_add_flag(automationIndicator_->getElement(), LV_OBJ_FLAG_HIDDEN);

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

void ParameterButtonWidget::setHasAutomation(bool hasAutomation) {
    hasAutomation_ = hasAutomation;
    if (automationIndicator_) {
        if (hasAutomation) {
            lv_obj_clear_flag(automationIndicator_->getElement(), LV_OBJ_FLAG_HIDDEN);
            automationIndicator_->setState(oc::ui::lvgl::StateIndicator::State::ACTIVE);
        } else {
            lv_obj_add_flag(automationIndicator_->getElement(), LV_OBJ_FLAG_HIDDEN);
            automationIndicator_->setState(oc::ui::lvgl::StateIndicator::State::OFF);
        }
    }
}

}  // namespace bitwig
