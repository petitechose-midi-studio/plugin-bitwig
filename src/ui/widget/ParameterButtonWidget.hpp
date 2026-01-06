#pragma once

#include <memory>

#include <oc/ui/lvgl/widget/ButtonWidget.hpp>
#include <oc/ui/lvgl/widget/StateIndicator.hpp>

#include "BaseParameterWidget.hpp"

namespace bitwig::ui {

/**
 * @brief Parameter widget with ButtonWidget + name label (color indicator only, no text)
 */
class ParameterButtonWidget : public BaseParameterWidget {
public:
    ParameterButtonWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, uint8_t index);
    ~ParameterButtonWidget() override;

    void setValue(float value) override;
    void setValueWithDisplay(float value, const char* displayValue) override;

    // Automation indicator support
    void setHasAutomation(bool hasAutomation) override;

private:
    void createUI(lv_coord_t width, lv_coord_t height);

    std::unique_ptr<oc::ui::lvgl::ButtonWidget> button_;
    std::unique_ptr<oc::ui::lvgl::StateIndicator> automationIndicator_;
    bool hasAutomation_ = false;
};

}  // namespace bitwig
