#pragma once

/**
 * @file ParameterListWidget.hpp
 * @brief Discrete value selector widget for enumerated Bitwig parameters
 *
 * Displays a segmented enum selector with:
 * - Visual segments for each discrete value
 * - Current value text display above the selector
 * - Parameter name label below
 *
 * Used for ParameterType::LIST in RemoteControlsView (e.g., filter types,
 * oscillator modes, or any parameter with named discrete values).
 *
 * @see BaseParameterWidget for shared layout infrastructure
 * @see ParameterKnobWidget for continuous parameters
 */

#include <memory>
#include <string>
#include <vector>

#include <oc/ui/lvgl/widget/EnumWidget.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>
#include "BaseParameterWidget.hpp"

namespace bitwig::ui {

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

private:
    void createUI(lv_coord_t width, lv_coord_t height);
    void updateValueDisplay();

    std::unique_ptr<oc::ui::lvgl::Label> value_label_;
    std::unique_ptr<oc::ui::lvgl::EnumWidget> enum_widget_;
    int16_t discrete_count_ = 0;
    std::vector<std::string> value_names_;
    uint8_t current_index_ = 0;
};

}  // namespace bitwig::ui
