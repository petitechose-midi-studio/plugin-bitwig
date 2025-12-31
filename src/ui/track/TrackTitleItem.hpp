#pragma once

#include "ui/device/LevelBar.hpp"

#include <cstdint>
#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IListItem.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include "protocol/TrackType.hpp"

namespace bitwig {

struct TrackTitleItemProps {
    const char *name = "";
    uint32_t color = 0xFFFFFF;
    TrackType trackType = TrackType::AUDIO;
    bool isMuted = false;
    bool isSoloed = false;
    float level = 0.0f;
    bool highlighted = false;
    bool hideIndicators = false;
};

/**
 * @brief Track title item for track selector lists
 *
 * Displays track name, type icon, color bar, mute/solo indicators and level.
 * Implements IListItem for consistent handling in VirtualList.
 */
class TrackTitleItem : public oc::ui::lvgl::IListItem {
public:
    explicit TrackTitleItem(lv_obj_t *parent, bool withMuteSolo = false,
                            lv_coord_t barHeight = LV_PCT(100));
    ~TrackTitleItem() override;

    TrackTitleItem(const TrackTitleItem&) = delete;
    TrackTitleItem& operator=(const TrackTitleItem&) = delete;

    void render(const TrackTitleItemProps &props);

    // IListItem
    void setHighlighted(bool highlighted) override;

    // IComponent
    void show() override;
    void hide() override;
    bool isVisible() const override { return visible_; }
    lv_obj_t* getElement() const override { return container_; }

    static const char *getTrackTypeIcon(TrackType trackType);

private:
    void updateIndicatorOpacity(bool isMuted, bool isSoloed, bool highlighted);

    lv_obj_t *container_ = nullptr;
    lv_obj_t *color_bar_ = nullptr;
    lv_obj_t *mute_icon_ = nullptr;
    lv_obj_t *solo_icon_ = nullptr;
    lv_obj_t *type_icon_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::Label> label_;
    std::unique_ptr<LevelBar> level_bar_;

    lv_coord_t bar_height_ = 12;
    bool has_mute_solo_ = false;
    bool visible_ = true;
    bool highlighted_ = false;
};

}  // namespace bitwig
