#pragma once

#include <string>
#include <vector>

#include <lvgl.h>

namespace Bitwig {

/**
 * @brief Interface for parameter widgets (polymorphism)
 *
 * Defines common operations for all parameter widget types:
 * - ParameterKnobWidget (continuous/centered knobs)
 * - ParameterListWidget (enum/list selectors)
 * - ParameterButtonWidget (toggle buttons)
 */
class IParameterWidget {
public:
    virtual ~IParameterWidget() = default;

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
    virtual lv_obj_t* getContainer() const = 0;
};

}  // namespace Bitwig
