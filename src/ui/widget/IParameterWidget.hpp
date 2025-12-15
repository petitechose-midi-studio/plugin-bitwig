#pragma once

#include <string>
#include <vector>

#include <oc/ui/lvgl/IWidget.hpp>

namespace bitwig {

/**
 * @brief Interface for parameter widgets (polymorphism)
 *
 * Defines common operations for all parameter widget types:
 * - ParameterKnobWidget (continuous/centered knobs)
 * - ParameterListWidget (enum/list selectors)
 * - ParameterButtonWidget (toggle buttons)
 */
class IParameterWidget : public oc::ui::lvgl::IWidget {
public:
    ~IParameterWidget() override = default;

    virtual void setName(const std::string& name) = 0;
    virtual void setValue(float value) = 0;
    virtual void setValueWithDisplay(float value, const char* displayValue) = 0;

    virtual void setDiscreteMetadata(int16_t discreteCount,
                                     const std::vector<std::string>& valueNames,
                                     uint8_t currentIndex) {
        (void)discreteCount;
        (void)valueNames;
        (void)currentIndex;
    }

    virtual void setVisible(bool visible) = 0;
};

}  // namespace bitwig
