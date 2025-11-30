#pragma once

#include <lvgl.h>
#include <cstdint>
#include "ui/font/icon.hpp"

namespace Bitwig
{

struct ButtonHintItem
{
    const char *text = nullptr;   // For text mode
    const char *icon = nullptr;   // For icon mode (takes precedence over text)
    uint32_t color = 0xFFFFFF;
    bool active = true;           // Controls opacity (true = full, false = dimmed)
};

struct ButtonHintBarProps
{
    ButtonHintItem left;
    ButtonHintItem center;
    ButtonHintItem right;
    bool visible = true;
};

/**
 * @brief A 3-cell footer bar showing button action hints
 *
 * Displays labels aligned with physical buttons below the screen.
 * Each cell corresponds to a button position (left, center, right).
 * Anchored at bottom-center of parent by default.
 * Uses lazy initialization - LVGL widgets created on first use.
 */
class ButtonHintBar
{
public:
    explicit ButtonHintBar(lv_obj_t *parent);
    ~ButtonHintBar();

    void render(const ButtonHintBarProps &props);

    lv_obj_t *getElement() const { return container_; }

private:
    void ensureCreated();
    void applyStyle(lv_obj_t *label);
    void renderItem(lv_obj_t *label, const ButtonHintItem &item, lv_grid_align_t align, int col);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *container_ = nullptr;
    lv_obj_t *left_label_ = nullptr;
    lv_obj_t *center_label_ = nullptr;
    lv_obj_t *right_label_ = nullptr;
};

} // namespace Bitwig
