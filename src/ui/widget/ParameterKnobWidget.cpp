#include "ParameterKnobWidget.hpp"

#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;

namespace bitwig::ui {

ParameterKnobWidget::ParameterKnobWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height,
                                         uint8_t index, bool centered)
    : BaseParameterWidget(parent, index) {
    createUI(width, height, centered);
}

ParameterKnobWidget::~ParameterKnobWidget() {
    automationIndicator_.reset();
    knob_.reset();
}

void ParameterKnobWidget::createUI(lv_coord_t width, lv_coord_t height, bool centered) {
    createContainerWithGrid(width, height);

    // Knob widget - stretch horizontally, CONTENT row sizes to knob height
    knob_ = std::make_unique<oc::ui::lvgl::KnobWidget>(container_);
    knob_->sizeMode(oc::ui::lvgl::SizeMode::SquareFromWidth)  // Explicit: height = width
        .centered(centered)
        .bgColor(Color::KNOB_BACKGROUND)
        .trackColor(Color::KNOB_VALUE_RIBBON)
        .valueColor(Color::KNOB_VALUE_INDICATOR)
        .ribbonThickness(0.3)
        .flashColor(Color::DATA_ACTIVE);
    lv_obj_set_grid_cell(knob_->getElement(),
        LV_GRID_ALIGN_STRETCH, 0, 1,  // Horizontal: stretch to get width
        LV_GRID_ALIGN_START, 0, 1);   // Vertical: start in CONTENT row

    // Automation indicator - overlay inside knob widget
    automationIndicator_ = std::make_unique<oc::ui::lvgl::StateIndicator>(
        knob_->getElement(), Layout::AUTOMATION_INDICATOR_SIZE);
    automationIndicator_->color(oc::ui::lvgl::StateIndicator::State::ACTIVE, Color::AUTOMATION_INDICATOR);
    automationIndicator_->setState(oc::ui::lvgl::StateIndicator::State::OFF);
    lv_obj_add_flag(automationIndicator_->getElement(), LV_OBJ_FLAG_FLOATING);
    lv_obj_set_align(automationIndicator_->getElement(), LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(automationIndicator_->getElement(), Layout::AUTOMATION_INDICATOR_OFFSET);
    lv_obj_add_flag(automationIndicator_->getElement(), LV_OBJ_FLAG_HIDDEN);

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

void ParameterKnobWidget::setModulatedValue(float modulatedValue) {
    if (knob_) {
        knob_->setRibbonValue(modulatedValue);
    }
}

void ParameterKnobWidget::setIsModulated(bool isModulated) {
    if (knob_) {
        knob_->setRibbonEnabled(isModulated);
    }
}

void ParameterKnobWidget::setHasAutomation(bool hasAutomation) {
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

}  // namespace bitwig::ui
