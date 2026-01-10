#pragma once

/**
 * @file LevelBar.hpp
 * @brief Horizontal level/progress bar for value visualization
 *
 * Displays a colored horizontal bar representing a normalized value (0.0-1.0).
 * Used in TrackTitleItem to show track volume levels.
 *
 * Stateless - receives all data via render(LevelBarProps).
 *
 * @see TrackTitleItem for usage in track list items
 */

#include <cstdint>

#include <lvgl.h>

#include <oc/ui/lvgl/IWidget.hpp>

#include "ui/theme/BitwigTheme.hpp"

namespace bitwig::ui {

struct LevelBarProps {
    float value = 0.0f;
    uint32_t color = 0xFFFFFF;
    lv_opa_t opacity = theme::opacity::DIMMED;
};

/**
 * @brief Simple horizontal level bar displaying a normalized value (0.0-1.0)
 *
 * Displays a colored bar with configurable opacity representing a value.
 * Used for track volume/level display in TrackListOverlay.
 * Implements IWidget for consistent element access.
 */
class LevelBar : public oc::ui::lvgl::IWidget {
public:
    LevelBar(lv_obj_t *parent, lv_coord_t width = 60, lv_coord_t height = LV_PCT(100));
    ~LevelBar() override;

    LevelBar(const LevelBar&) = delete;
    LevelBar& operator=(const LevelBar&) = delete;

    void render(const LevelBarProps &props);

    // IWidget
    lv_obj_t* getElement() const override { return bar_; }

private:
    lv_obj_t *bar_ = nullptr;
    lv_coord_t width_ = 60;
    lv_coord_t height_ = 12;
};

}  // namespace bitwig::ui
