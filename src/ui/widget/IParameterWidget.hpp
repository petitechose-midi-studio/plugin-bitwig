#pragma once

#include <string>

#include <oc/ui/lvgl/IWidget.hpp>

namespace bitwig::ui {

/**
 * @brief Interface for parameter widgets (polymorphism)
 *
 * Defines common operations for all parameter widget types:
 * - ParameterKnobWidget (continuous/centered knobs)
 * - ParameterListWidget (enum/list selectors)
 * - ParameterButtonWidget (toggle buttons)
 *
 * @note Discrete metadata (for LIST/BUTTON types) is handled by
 *       BaseParameterWidget::setDiscreteMetadata(), not in this interface.
 */
class IParameterWidget : public oc::ui::lvgl::IWidget {
public:
    ~IParameterWidget() override = default;

    virtual void setName(const std::string& name) = 0;
    virtual void setValue(float value) = 0;
    virtual void setValueWithDisplay(float value, const char* displayValue) = 0;
    virtual void setVisible(bool visible) = 0;

    // Automation indicator support (default: no-op for widgets that don't support it)
    virtual void setHasAutomation(bool /*hasAutomation*/) {}
};

}  // namespace bitwig
