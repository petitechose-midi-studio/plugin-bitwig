#pragma once

#include <memory>
#include <string>
#include <vector>

#include <oc/ui/lvgl/widget/EnumWidget.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>
#include <oc/ui/lvgl/widget/StateIndicator.hpp>

#include "BaseParameterWidget.hpp"

namespace bitwig {

/**
 * @brief Parameter widget with EnumWidget + name label + value display
 */
class ParameterListWidget : public BaseParameterWidget {
public:
    ParameterListWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, uint8_t index,
                        int16_t discreteCount);
    ~ParameterListWidget() override;

    void setValue(float value) override;
    void setValueWithDisplay(float value, const char* displayValue) override;
    void setDiscreteMetadata(int16_t discreteCount, const std::vector<std::string>& valueNames,
                             uint8_t currentIndex) override;

    // Automation indicator support
    void setHasAutomation(bool hasAutomation) override;

private:
    void createUI(lv_coord_t width, lv_coord_t height);
    void updateValueDisplay();

    std::unique_ptr<oc::ui::lvgl::Label> value_label_;
    std::unique_ptr<oc::ui::lvgl::EnumWidget> enum_widget_;
    std::unique_ptr<oc::ui::lvgl::StateIndicator> automationIndicator_;
    int16_t discrete_count_ = 0;
    std::vector<std::string> value_names_;
    uint8_t current_index_ = 0;
    bool hasAutomation_ = false;
};

}  // namespace bitwig
