#pragma once

/**
 * @file ParameterButtonWidget.hpp
 * @brief Toggle button widget for binary Bitwig device parameters
 *
 * Displays a simple on/off button with color indicator:
 * - Active state shown via button fill color
 * - Parameter name label below
 * - No text display (color-only feedback)
 *
 * Used for ParameterType::BUTTON in RemoteControlsView.
 *
 * @see BaseParameterWidget for shared layout infrastructure
 * @see ParameterKnobWidget for continuous parameters
 */

#include <memory>

#include <oc/ui/lvgl/widget/ButtonWidget.hpp>
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

private:
    void createUI(lv_coord_t width, lv_coord_t height);

    std::unique_ptr<oc::ui::lvgl::ButtonWidget> button_;
};

}  // namespace bitwig::ui
