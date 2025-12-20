#pragma once

#include <memory>

#include <oc/ui/lvgl/widget/KnobWidget.hpp>

#include "BaseParameterWidget.hpp"

namespace bitwig {

/**
 * @brief Parameter widget with KnobWidget + name label (no value text display)
 */
class ParameterKnobWidget : public BaseParameterWidget {
public:
    ParameterKnobWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, uint8_t index,
                        bool centered = false);
    ~ParameterKnobWidget() override;

    void setValue(float value) override;
    void setValueWithDisplay(float value, const char* displayValue) override;

    void setOrigin(float origin);
    void setModulatedValue(float modulatedValue);
    void setIsModulated(bool isModulated);

private:
    void createUI(lv_coord_t width, lv_coord_t height, bool centered);

    std::unique_ptr<oc::ui::lvgl::KnobWidget> knob_;
};

}  // namespace bitwig
