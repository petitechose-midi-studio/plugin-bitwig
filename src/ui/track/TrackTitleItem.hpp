#pragma once

#include "ui/device/LevelBar.hpp"

#include <cstdint>

#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/widget/Label.hpp>

namespace bitwig {

struct TrackTitleItemProps {
    const char *name = "";
    uint32_t color = 0xFFFFFF;
    uint8_t trackType = 0;
    bool isMuted = false;
    bool isSoloed = false;
    float level = 0.0f;
    bool highlighted = false;
    bool hideIndicators = false;
};

class TrackTitleItem {
public:
    explicit TrackTitleItem(lv_obj_t *parent, bool withMuteSolo = false,
                            lv_coord_t barHeight = LV_PCT(100));
    ~TrackTitleItem();

    void render(const TrackTitleItemProps &props);

    static const char *getTrackTypeIcon(uint8_t trackType);
private:
    void updateIndicatorOpacity(bool isMuted, bool isSoloed, bool highlighted);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *color_bar_ = nullptr;
    lv_obj_t *mute_icon_ = nullptr;
    lv_obj_t *solo_icon_ = nullptr;
    lv_obj_t *type_icon_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::Label> label_;
    std::unique_ptr<LevelBar> level_bar_;

    lv_coord_t bar_height_ = 12;
    bool has_mute_solo_ = false;
};

}  // namespace bitwig
