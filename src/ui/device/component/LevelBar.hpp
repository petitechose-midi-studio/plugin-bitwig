#pragma once

#include <cstdint>

#include <lvgl.h>

namespace bitwig {

struct LevelBarProps {
    float value = 0.0f;
    uint32_t color = 0xFFFFFF;
    lv_opa_t opacity = LV_OPA_50;
};

/**
 * @brief Simple horizontal level bar displaying a normalized value (0.0-1.0)
 *
 * Displays a colored bar with configurable opacity representing a value.
 * Used for track volume/level display in TrackListOverlay.
 */
class LevelBar {
public:
    LevelBar(lv_obj_t *parent, lv_coord_t width = 60, lv_coord_t height = LV_PCT(100));
    ~LevelBar();

    void render(const LevelBarProps &props);
private:
    lv_obj_t *parent_ = nullptr;
    lv_obj_t *bar_ = nullptr;
    lv_coord_t width_ = 60;
    lv_coord_t height_ = 12;
};

}  // namespace bitwig
