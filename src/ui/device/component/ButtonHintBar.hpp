#pragma once

#include <lvgl.h>
#include "ui/font/icon.hpp"

namespace Bitwig
{

/**
 * @brief A 3-cell footer bar showing button action hints
 *
 * Displays labels aligned with physical buttons below the screen.
 * Each cell corresponds to a button position (left, center, right).
 * Anchored at bottom-center of parent by default.
 */
class ButtonHintBar
{
public:
    explicit ButtonHintBar(lv_obj_t *parent);
    ~ButtonHintBar();

    void setLeftLabel(const char *text);
    void setCenterLabel(const char *text);
    void setRightLabel(const char *text);

    void setLeftIcon(const char *icon, uint32_t color = 0xFFFFFF);
    void setCenterIcon(const char *icon, uint32_t color = 0xFFFFFF);
    void setRightIcon(const char *icon, uint32_t color = 0xFFFFFF);

    void setCenterIconActive(bool active);
    void setRightIconActive(bool active);

    void show();
    void hide();

    lv_obj_t *getElement() const { return container_; }

private:
    void createLayout();
    void applyStyle(lv_obj_t *label);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *container_ = nullptr;
    lv_obj_t *left_label_ = nullptr;
    lv_obj_t *center_label_ = nullptr;
    lv_obj_t *right_label_ = nullptr;
};

} // namespace Bitwig
