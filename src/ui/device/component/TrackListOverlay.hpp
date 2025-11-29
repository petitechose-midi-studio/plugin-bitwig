#pragma once

#include "widget/ListOverlay.hpp"
#include "TrackTitleItem.hpp"
#include "interface/IComponent.hpp"
#include <vector>
#include <string>
#include <memory>
#include <cstdint>

namespace Bitwig
{

    /**
     * @brief Bitwig-specific list overlay with track mute/solo indicators
     *
     * Wraps ListOverlay and uses TrackTitleItem components for consistent
     * track display with color bar, mute/solo indicators, type icon, and name.
     *
     * Usage:
     *   TrackListOverlay overlay(parent);
     *   overlay.setTitle("Select Track");
     *
     *   std::vector<std::string> names = {"Track 1", "Track 2"};
     *   std::vector<bool> muteStates = {false, true};
     *   std::vector<bool> soloStates = {true, false};
     *
     *   overlay.setTrackItems(names, 0, muteStates, soloStates);
     *   overlay.show();
     */
    class TrackListOverlay : public UI::IComponent
    {
    public:
        explicit TrackListOverlay(lv_obj_t *parent);
        ~TrackListOverlay();

        void setTitle(const std::string &title);
        void setItems(const std::vector<std::string> &items);

        /**
         * @brief Set track items with mute/solo indicators
         * @param names Track names
         * @param currentIndex Index of current track with Bitwig focus
         * @param muteStates Mute state per track
         * @param soloStates Solo state per track
         * @param trackTypes Track type per track (0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master)
         * @param trackColors Track colors (RGB hex values)
         */
        void setTrackItems(const std::vector<std::string> &names,
                           int currentIndex,
                           const std::vector<bool> &muteStates = {},
                           const std::vector<bool> &soloStates = {},
                           const std::vector<uint8_t> &trackTypes = {},
                           const std::vector<uint32_t> &trackColors = {});

        void setSelectedIndex(int index);
        void show() override;
        void hide() override;
        bool isVisible() const override;
        int getSelectedIndex() const;
        int getItemCount() const;
        lv_obj_t *getElement() const override { return list_.getElement(); }

        void setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted);
        void setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed);
        bool getTrackMuteStateAtIndex(uint8_t trackIndex) const;
        bool getTrackSoloStateAtIndex(uint8_t trackIndex) const;

    private:
        void createTrackItems();
        void clearTrackItems();
        void updateHighlightStates();

        ListOverlay list_;
        std::vector<std::string> item_names_;
        int current_track_index_ = -1;
        std::vector<bool> mute_states_;
        std::vector<bool> solo_states_;
        std::vector<uint8_t> track_types_;
        std::vector<uint32_t> track_colors_;

        // TrackTitleItem instances for each list item
        std::vector<std::unique_ptr<TrackTitleItem>> track_items_;
    };

} // namespace Bitwig
