#pragma once

#include <memory>
#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/widget/EnumWidget.hpp>

#include "IParameterWidget.hpp"

namespace Bitwig {

/**
 * @brief Parameter widget with EnumWidget + name label + value display
 */
class ParameterListWidget : public IParameterWidget {
public:
    ParameterListWidget(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, uint8_t index,
                        int16_t discreteCount);
    ~ParameterListWidget() override;

    void setName(const std::string& name) override;
    void setValue(float value) override;
    void setValueWithDisplay(float value, const char* displayValue) override;
    void setDiscreteMetadata(int16_t discreteCount, const std::vector<std::string>& valueNames,
                             uint8_t currentIndex) override;
    void setVisible(bool visible) override;
    lv_obj_t* getContainer() const override { return container_; }

private:
    void createUI(lv_coord_t width, lv_coord_t height);
    void updateValueDisplay();

    lv_obj_t* parent_ = nullptr;  // Parent for widget creation
    lv_obj_t* container_ = nullptr;
    lv_obj_t* name_label_ = nullptr;
    lv_obj_t* value_label_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::EnumWidget> enum_widget_;
    uint8_t index_ = 0;
    int16_t discrete_count_ = 0;
    std::vector<std::string> value_names_;
    uint8_t current_index_ = 0;
};

}  // namespace Bitwig
