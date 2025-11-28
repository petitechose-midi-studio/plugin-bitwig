#pragma once

#include "widget/ListOverlay.hpp"
#include "interface/IComponent.hpp"
#include <vector>
#include <string>
#include <array>
#include <cstdint>

namespace Bitwig
{

    /**
     * @brief Bitwig-specific list overlay with track mute/solo indicators
     *
     * Wraps ListOverlay and adds (M) and (S) indicators before labels to indicate
     * track mute and solo states.
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
        /**
         * @brief Construct track list overlay
         * @param parent Parent LVGL object (typically screen)
         */
        explicit TrackListOverlay(lv_obj_t *parent);

        /**
         * @brief Destructor
         */
        ~TrackListOverlay();

        /**
         * @brief Set overlay title
         * @param title Title text (empty to hide title)
         */
        void setTitle(const std::string &title);

        /**
         * @brief Set items without indicators (for simple lists)
         * @param items Item names
         *
         * Use this for lists that don't need mute/solo indicators.
         */
        void setItems(const std::vector<std::string> &items);

        /**
         * @brief Set track items with mute/solo indicators
         * @param names Track names
         * @param currentIndex Index of current track with Bitwig focus
         * @param muteStates Mute state per track
         * @param soloStates Solo state per track
         * @param trackTypes Track type per track (0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master)
         * @param trackColors Track colors (RGB hex values)
         *
         * Note: All vectors should have the same size. Missing entries = false/0.
         */
        void setTrackItems(const std::vector<std::string> &names,
                           int currentIndex,
                           const std::vector<bool> &muteStates = {},
                           const std::vector<bool> &soloStates = {},
                           const std::vector<uint8_t> &trackTypes = {},
                           const std::vector<uint32_t> &trackColors = {});

        /**
         * @brief Set selected item index
         * @param index Item index (0-based), wrapped modulo
         */
        void setSelectedIndex(int index);

        /**
         * @brief Show overlay (from IComponent)
         */
        void show() override;

        /**
         * @brief Hide overlay (from IComponent)
         */
        void hide() override;

        /**
         * @brief Check if overlay is currently visible (from IComponent)
         * @return true if visible
         */
        bool isVisible() const override;

        /**
         * @brief Get currently selected index
         * @return Selected index (0-based), -1 if no items
         */
        int getSelectedIndex() const;

        /**
         * @brief Get number of items in list
         * @return Item count
         */
        int getItemCount() const;

        /**
         * @brief Get underlying LVGL element (from IElement)
         * @return Overlay object (nullptr if not created)
         */
        lv_obj_t *getElement() const override { return list_.getElement(); }

        /**
         * @brief Set track mute state at specific index (updates indicator visibility)
         * @param trackIndex Track index
         * @param isMuted Mute state
         */
        void setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted);

        /**
         * @brief Set track solo state at specific index (updates indicator visibility)
         * @param trackIndex Track index
         * @param isSoloed Solo state
         */
        void setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed);

        /**
         * @brief Get current mute state at specific index
         * @param trackIndex Track index
         * @return Current mute state (false if index out of bounds)
         */
        bool getTrackMuteStateAtIndex(uint8_t trackIndex) const;

        /**
         * @brief Get current solo state at specific index
         * @param trackIndex Track index
         * @return Current solo state (false if index out of bounds)
         */
        bool getTrackSoloStateAtIndex(uint8_t trackIndex) const;

    private:
        void createIndicators();
        void clearIndicators();
        void updateIndicatorStates();
        lv_obj_t *createVerticalBar(lv_obj_t *parent, uint32_t color);

        ListOverlay list_;

        std::vector<std::string> item_names_;
        int current_track_index_ = -1;
        std::vector<bool> mute_states_;
        std::vector<bool> solo_states_;
        std::vector<uint8_t> track_types_;
        std::vector<uint32_t> track_colors_;

        // Store indicator objects: [track_index][0=mute, 1=solo]
        std::vector<std::array<lv_obj_t *, 2>> indicator_circles_;

        // Store track type icons: [track_index]
        std::vector<lv_obj_t *> track_icons_;

        // Store vertical bars: [track_index]
        std::vector<lv_obj_t *> vertical_bars_;
    };

} // namespace Bitwig
