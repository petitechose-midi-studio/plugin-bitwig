#pragma once

#include "TrackListOverlay.hpp"
#include "ButtonHintBar.hpp"
#include "interface/IComponent.hpp"
#include <string>
#include <vector>
#include <memory>

namespace Bitwig
{

    /**
     * @brief Track list selector component
     *
     * Displays a list of tracks for quick navigation.
     * Simpler than DeviceSelector (no hierarchical navigation).
     *
     * PURE UI COMPONENT - No logic, no callbacks, no protocol dependencies.
     * All data is pre-formatted by Controller.
     *
     * Usage (from DeviceController):
     *   trackSelector.setItems({"Track 1", "Track 2", "Track 3"});
     *   trackSelector.setCurrentItemIndex(0);
     *   trackSelector.show();
     */
    class TrackListSelector : public UI::IComponent
    {
    public:
        /**
         * @brief Construct track list selector
         * @param parent Parent LVGL object (typically screen)
         */
        explicit TrackListSelector(lv_obj_t *parent);

        /**
         * @brief Destructor
         */
        ~TrackListSelector();

        /**
         * @brief Set selector title
         * @param title Title text (default: "Select Track")
         */
        void setTitle(const std::string &title);

        /**
         * @brief Set list items
         * @param items Vector of track names
         */
        void setItems(const std::vector<std::string> &items);

        /**
         * @brief Set track items with mute/solo indicators
         * @param items Vector of track names
         * @param currentIndex Current track index (0-based)
         * @param muteStates Mute state per track
         * @param soloStates Solo state per track
         * @param trackTypes Track type per track (0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master)
         * @param trackColors Track colors (RGB hex values)
         */
        void setTrackItems(const std::vector<std::string> &items,
                           int currentIndex,
                           const std::vector<bool> &muteStates,
                           const std::vector<bool> &soloStates,
                           const std::vector<uint8_t> &trackTypes = {},
                           const std::vector<uint32_t> &trackColors = {});

        /**
         * @brief Set current item index (initial selection when showing)
         * @param index Current item index (0-based)
         */
        void setCurrentItemIndex(int index);

        /**
         * @brief Set selected item index (navigation)
         * @param index Selected index (0-based), clamped to valid range
         */
        void setSelectedIndex(int index);

        /**
         * @brief Show track selector overlay (from IComponent)
         */
        void show() override;

        /**
         * @brief Hide track selector overlay (from IComponent)
         */
        void hide() override;

        /**
         * @brief Check if selector is visible (from IComponent)
         * @return true if visible
         */
        bool isVisible() const override;

        /**
         * @brief Get currently selected item index
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
        lv_obj_t *getElement() const override { return overlay_.getElement(); }

        /**
         * @brief Set track mute state at specific index
         * @param trackIndex Track index
         * @param isMuted Mute state
         */
        void setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted);

        /**
         * @brief Set track solo state at specific index
         * @param trackIndex Track index
         * @param isSoloed Solo state
         */
        void setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed);

        /**
         * @brief Get current mute state at specific index
         * @param trackIndex Track index
         * @return Current mute state
         */
        bool getTrackMuteStateAtIndex(uint8_t trackIndex) const;

        /**
         * @brief Get current solo state at specific index
         * @param trackIndex Track index
         * @return Current solo state
         */
        bool getTrackSoloStateAtIndex(uint8_t trackIndex) const;

    private:
        void createFooter();
        void destroyFooter();
        void updateFooterStates();

        lv_obj_t *parent_;
        TrackListOverlay overlay_;
        std::vector<std::string> items_;
        int current_item_index_ = 0;

        std::unique_ptr<ButtonHintBar> footer_;
    };

} // namespace Bitwig
