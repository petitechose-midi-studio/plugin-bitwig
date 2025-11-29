#pragma once

#include <lvgl.h>
#include <string>
#include <cstdint>

namespace Bitwig
{

    /**
     * @brief Reusable track title component with color indicator, optional mute/solo, and type icon
     *
     * Displays: [color bar] [mute?] [solo?] [type icon] [track name]
     * Creates children directly in parent (no intermediate container).
     * Used in DeviceStateBar (top bar) and TrackListOverlay items.
     *
     * @param withMuteSolo Set to true for list items that need mute/solo indicators
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

        // Highlight state (affects ghost opacity and label color for list items)
        void setHighlighted(bool highlighted);

        // Hide all indicators (for special items like "Back")
        void hideIndicators();

        // Get icon for track type (public for potential reuse)
        static const char *getTrackTypeIcon(uint8_t trackType);

    private:
        void updateIndicatorOpacity();

        lv_obj_t *color_bar_ = nullptr;
        lv_obj_t *mute_icon_ = nullptr;   // nullptr if withMuteSolo=false
        lv_obj_t *solo_icon_ = nullptr;   // nullptr if withMuteSolo=false
        lv_obj_t *type_icon_ = nullptr;
        lv_obj_t *label_ = nullptr;

        bool has_mute_solo_ = false;
        bool is_muted_ = false;
        bool is_soloed_ = false;
        bool is_highlighted_ = false;
        bool indicators_hidden_ = false;
    };

} // namespace Bitwig
