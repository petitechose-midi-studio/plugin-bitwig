#pragma once

#include <memory>
#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/widget/ButtonWidget.hpp>

#include "IParameterWidget.hpp"

namespace Bitwig {

/**
 * @brief Parameter widget with ButtonWidget + name label
 */
class ParameterButtonWidget : public IParameterWidget {
public:
    ParameterButtonWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, uint8_t index);
    ~ParameterButtonWidget() override;

    void setName(const std::string& name) override;
    void setValue(float value) override;
    void setValueWithDisplay(float value, const char* displayValue) override;
    void setDiscreteMetadata(int16_t discreteCount, const std::vector<std::string>& valueNames,
                             uint8_t currentIndex) override;
    void setVisible(bool visible) override;
    lv_obj_t* getContainer() const override { return container_; }

private:
    void createUI(lv_coord_t width, lv_coord_t height);

    lv_obj_t* container_ = nullptr;
    lv_obj_t* name_label_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::ButtonWidget> button_;
    uint8_t index_ = 0;
    std::vector<std::string> value_names_;
};

}  // namespace Bitwig
