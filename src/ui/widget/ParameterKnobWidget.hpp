#pragma once

/**
 * @file ParameterKnobWidget.hpp
 * @brief Knob widget for continuous Bitwig device parameters
 *
 * Displays a rotary knob with:
 * - Value arc with configurable origin (for bipolar parameters like pan)
 * - Modulation overlay showing LFO/automation offset
 * - Parameter name label below
 *
 * Used for ParameterType::KNOB in RemoteControlsView.
 *
 * @see BaseParameterWidget for shared layout infrastructure
 * @see ParameterListWidget for discrete parameter display
 */

#include <memory>

#include <oc/ui/lvgl/widget/KnobWidget.hpp>
#include "BaseParameterWidget.hpp"

namespace bitwig::ui {

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

}  // namespace bitwig::ui
