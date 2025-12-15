#pragma once

#include <memory>
#include <string>

#include <lvgl.h>

#include <oc/ui/lvgl/widget/KnobWidget.hpp>

#include "IParameterWidget.hpp"

namespace bitwig {

/**
 * @brief Parameter widget with KnobWidget + name label (no value text display)
 */
class ParameterKnobWidget : public IParameterWidget {
public:
    ParameterKnobWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, uint8_t index,
                        bool centered = false);
    ~ParameterKnobWidget() override;

    void setName(const std::string& name) override;
    void setValue(float value) override;
    void setValueWithDisplay(float value, const char* displayValue) override;
    void setVisible(bool visible) override;
    lv_obj_t* getElement() const override { return container_; }

    void setOrigin(float origin);

private:
    void createUI(lv_coord_t width, lv_coord_t height, bool centered);

    lv_obj_t* parent_ = nullptr;
    lv_obj_t* container_ = nullptr;
    lv_obj_t* name_label_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::KnobWidget> knob_;
    uint8_t index_ = 0;
};

}  // namespace bitwig
