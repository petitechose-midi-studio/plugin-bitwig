#pragma once

#include <lvgl.h>
#include <string>
#include <cstdint>
#include <memory>
#include "LevelBar.hpp"

namespace Bitwig
{

    /**
     * @brief Reusable track title component with color indicator, optional mute/solo/level, and type icon
     *
     * Displays: [color bar] [type icon] [track name] [mute?] [solo?] [level?]
     * Creates children directly in parent (no intermediate container).
     * Uses lazy initialization - LVGL widgets created on first use.
     *
     * @param withMuteSolo Set to true for list items that need mute/solo/level indicators
     */
    class TrackTitleItem
    {
    public:
        explicit TrackTitleItem(lv_obj_t *parent, bool withMuteSolo = false, lv_coord_t barHeight = LV_PCT(100));
        ~TrackTitleItem();

        void setName(const std::string &name);
        void setColor(uint32_t color);
        void setType(uint8_t trackType);

        // Mute/solo state (only effective if withMuteSolo=true)
        void setMuteState(bool isMuted);
        void setSoloState(bool isSoloed);

        // Level bar value (only effective if withMuteSolo=true)
        void setLevel(float normalizedValue);

        // Highlight state (affects ghost opacity and label color for list items)
        void setHighlighted(bool highlighted);

        // Hide all indicators (for special items like "Back")
        void hideIndicators();

        // Get icon for track type (public for potential reuse)
        static const char *getTrackTypeIcon(uint8_t trackType);

    private:
        void ensureCreated();
        void updateIndicatorOpacity();

        lv_obj_t *parent_ = nullptr;
        lv_obj_t *color_bar_ = nullptr;
        lv_obj_t *mute_icon_ = nullptr;
        lv_obj_t *solo_icon_ = nullptr;
        lv_obj_t *type_icon_ = nullptr;
        lv_obj_t *label_ = nullptr;
        std::unique_ptr<LevelBar> level_bar_;

        lv_coord_t bar_height_ = 12;
        uint32_t track_color_ = 0xFFFFFF;
        uint8_t track_type_ = 0;
        std::string pending_name_;
        float pending_level_ = 0.0f;
        bool has_mute_solo_ = false;
        bool is_muted_ = false;
        bool is_soloed_ = false;
        bool is_highlighted_ = false;
        bool indicators_hidden_ = false;
    };

} // namespace Bitwig
