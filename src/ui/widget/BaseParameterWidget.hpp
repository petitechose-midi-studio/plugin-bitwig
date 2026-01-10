#pragma once

/**
 * @file BaseParameterWidget.hpp
 * @brief Abstract base class for Bitwig parameter widgets
 *
 * Provides shared infrastructure for all parameter widget types:
 * - 2-row grid layout (widget + name label)
 * - Automation indicator overlay
 * - Common setName/setVisible implementations
 *
 * Concrete implementations:
 * - ParameterKnobWidget: continuous/bipolar knobs
 * - ParameterButtonWidget: toggle buttons
 * - ParameterListWidget: discrete value selectors
 *
 * @see IParameterWidget for the polymorphic interface
 * @see RemoteControlsView for widget instantiation based on ParameterType
 */

#include <memory>
#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/widget/Label.hpp>
#include <oc/ui/lvgl/widget/StateIndicator.hpp>

#include "IParameterWidget.hpp"

namespace bitwig::ui {

/**
 * @brief Base class for parameter widgets with common grid layout and name label
 *
 * Provides:
 * - Container with 2-row grid layout (widget row + label row)
 * - Name label creation and management
 * - Common setName(), setVisible(), getElement() implementations
 *
 * Subclasses must:
 * - Call createContainerWithGrid() and createNameLabel() in their createUI()
 * - Implement setValue(), setValueWithDisplay()
 * - Create their specific widget (knob, button, enum) in the first grid row
 */
class BaseParameterWidget : public IParameterWidget {
public:
    ~BaseParameterWidget() override;

    // Non-copyable
    BaseParameterWidget(const BaseParameterWidget&) = delete;
    BaseParameterWidget& operator=(const BaseParameterWidget&) = delete;

    // Common implementations
    void setName(const std::string& name) override;
    void setVisible(bool visible) override;
    lv_obj_t* getElement() const override { return container_; }

    // Automation indicator support
    void setHasAutomation(bool hasAutomation) override;

    /**
     * @brief Set discrete metadata for button/list widgets
     *
     * Default no-op. Override in ParameterListWidget for actual implementation.
     * Called via static_cast<BaseParameterWidget*> from DeviceView for BUTTON/LIST types.
     */
    virtual void setDiscreteMetadata(int16_t discreteCount,
                                     const std::vector<std::string>& valueNames,
                                     uint8_t currentIndex) {
        (void)discreteCount;
        (void)valueNames;
        (void)currentIndex;
    }

protected:
    explicit BaseParameterWidget(lv_obj_t* parent, uint8_t index);

    /**
     * @brief Create container with standard 2-row grid layout
     *
     * Grid: 1 column, 2 rows
     * - Row 0: FR(1) - flexible space for widget
     * - Row 1: CONTENT - label height
     */
    void createContainerWithGrid(lv_coord_t width, lv_coord_t height);

    /**
     * @brief Create name label in second grid row
     *
     * Must be called after createContainerWithGrid()
     */
    void createNameLabel();

    /**
     * @brief Create automation indicator as overlay inside the given parent widget
     *
     * @param parent The widget element to overlay (knob, button, or enum widget)
     */
    void createAutomationIndicator(lv_obj_t* parent);

    lv_obj_t* parent_ = nullptr;
    lv_obj_t* container_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::Label> name_label_;
    std::unique_ptr<oc::ui::lvgl::StateIndicator> automation_indicator_;
    uint8_t index_ = 0;
    bool has_automation_ = false;
};

}  // namespace bitwig::ui
