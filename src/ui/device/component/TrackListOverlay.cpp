#include "TrackListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "font/binary_font_buffer.hpp"
#include <algorithm>

using namespace Theme;

namespace Bitwig
{

    TrackListOverlay::TrackListOverlay(lv_obj_t *parent)
        : list_(parent)
    {
    }

    TrackListOverlay::~TrackListOverlay()
    {
        clearIndicators();
    }

    void TrackListOverlay::setTitle(const std::string &title)
    {
        list_.setTitle(title);
    }

    void TrackListOverlay::setItems(const std::vector<std::string> &items)
    {
        item_names_ = items;
        current_track_index_ = -1;
        mute_states_.clear();
        solo_states_.clear();

        list_.setItems(items);
        clearIndicators();
    }

    void TrackListOverlay::setTrackItems(const std::vector<std::string> &names,
                                         int currentIndex,
                                         const std::vector<bool> &muteStates,
                                         const std::vector<bool> &soloStates,
                                         const std::vector<bool> &groupStates)
    {
        item_names_ = names;
        current_track_index_ = currentIndex;
        mute_states_ = muteStates;
        solo_states_ = soloStates;
        group_states_ = groupStates;

        mute_states_.resize(names.size(), false);
        solo_states_.resize(names.size(), false);
        group_states_.resize(names.size(), false);

        list_.setItems(names);
        list_.setSelectedIndex(currentIndex);

        clearIndicators();
        createIndicators();

        updateIndicatorStates();
    }

    void TrackListOverlay::setSelectedIndex(int index)
    {
        list_.setSelectedIndex(index);
        updateIndicatorStates();
    }

    void TrackListOverlay::show()
    {
        list_.show();
        updateIndicatorStates();
    }

    void TrackListOverlay::hide()
    {
        list_.hide();
    }

    bool TrackListOverlay::isVisible() const
    {
        return list_.isVisible();
    }

    int TrackListOverlay::getSelectedIndex() const
    {
        return list_.getSelectedIndex();
    }

    int TrackListOverlay::getItemCount() const
    {
        return list_.getItemCount();
    }

    void TrackListOverlay::createIndicators()
    {
        indicator_circles_.clear();
        indicator_circles_.resize(item_names_.size());
        folder_icons_.clear();
        folder_icons_.resize(item_names_.size(), nullptr);

        for (size_t i = 0; i < item_names_.size(); i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            std::array<lv_obj_t *, 2> labels = {nullptr, nullptr};

            // Create "M" label for mute
            lv_obj_t *mute_label = lv_label_create(btn);
            lv_label_set_text(mute_label, "M");
            if (fonts.parameter_label)
            {
                lv_obj_set_style_text_font(mute_label, fonts.parameter_label, 0);
            }
            lv_obj_set_style_text_color(mute_label, lv_color_hex(Color::TRACK_MUTE), 0);
            lv_obj_move_to_index(mute_label, 0);
            labels[0] = mute_label;

            // Create "S" label for solo
            lv_obj_t *solo_label = lv_label_create(btn);
            lv_label_set_text(solo_label, "S");
            if (fonts.parameter_label)
            {
                lv_obj_set_style_text_font(solo_label, fonts.parameter_label, 0);
            }
            lv_obj_set_style_text_color(solo_label, lv_color_hex(Color::TRACK_SOLO), 0);
            lv_obj_move_to_index(solo_label, 1);
            labels[1] = solo_label;

            // Create folder icon if this is a group
            bool isGroup = (i < group_states_.size()) ? group_states_[i] : false;
            if (isGroup)
            {
                lv_obj_t *folder_icon = lv_label_create(btn);
                lv_label_set_text(folder_icon, LV_SYMBOL_DIRECTORY);
                if (fonts.lvgl_symbols)
                {
                    lv_obj_set_style_text_font(folder_icon, fonts.lvgl_symbols, 0);
                }
                lv_obj_set_style_text_color(folder_icon, lv_color_hex(Color::TEXT_PRIMARY), 0);
                lv_obj_set_style_text_opa(folder_icon, LV_OPA_70, 0);
                lv_obj_clear_flag(folder_icon, LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_move_to_index(folder_icon, 2);
                folder_icons_[i] = folder_icon;
            }

            indicator_circles_[i] = labels;
        }
    }

    void TrackListOverlay::updateIndicatorStates()
    {
        int itemCount = list_.getItemCount();
        int selectedIndex = list_.getSelectedIndex();

        for (int i = 0; i < itemCount; i++)
        {
            if (i >= static_cast<int>(indicator_circles_.size()))
                continue;

            lv_obj_t *mute_label = indicator_circles_[i][0];
            lv_obj_t *solo_label = indicator_circles_[i][1];

            if (!mute_label || !solo_label)
                continue;

            // Check if this is a special item (like "Back to parent")
            bool isSpecialItem = false;
            if (i < static_cast<int>(item_names_.size()))
            {
                const std::string &itemName = item_names_[i];
                isSpecialItem = (!itemName.empty() && itemName[0] == '[') ||
                                (itemName == "Back to parent");
            }

            bool isHighlighted = (i == selectedIndex);

            if (isSpecialItem)
            {
                // Hide indicators for special items (like "Back to parent")
                lv_obj_set_style_text_opa(mute_label, LV_OPA_TRANSP, 0);
                lv_obj_set_style_text_opa(solo_label, LV_OPA_TRANSP, 0);
                // Don't update mute/solo indicators below, but continue to update label highlighting
            }
            else
            {
                // Get mute/solo states
                bool isMuted = (i < static_cast<int>(mute_states_.size())) ? mute_states_[i] : false;
                bool isSoloed = (i < static_cast<int>(solo_states_.size())) ? solo_states_[i] : false;

                // Update mute label color and opacity
                lv_obj_set_style_text_color(mute_label, lv_color_hex(Color::TRACK_MUTE), 0);
                lv_opa_t mute_opa = isMuted ? LV_OPA_COVER : (isHighlighted ? LV_OPA_40 : LV_OPA_20);
                lv_obj_set_style_text_opa(mute_label, mute_opa, 0);

                // Update solo label color and opacity
                lv_obj_set_style_text_color(solo_label, lv_color_hex(Color::TRACK_SOLO), 0);
                lv_opa_t solo_opa = isSoloed ? LV_OPA_COVER : (isHighlighted ? LV_OPA_40 : LV_OPA_20);
                lv_obj_set_style_text_opa(solo_label, solo_opa, 0);
            }

            // Update track name label color based on highlight state
            lv_obj_t *btn = list_.getButton(i);
            if (btn)
            {
                bool hasFolder = (i < static_cast<int>(folder_icons_.size())) && folder_icons_[i];
                uint32_t labelIndex = 2 + (hasFolder ? 1 : 0);
                uint32_t childCount = lv_obj_get_child_cnt(btn);

                if (labelIndex < childCount)
                {
                    lv_obj_t *label = lv_obj_get_child(btn, labelIndex);
                    lv_color_t labelColor = isHighlighted ? lv_color_hex(Color::TEXT_PRIMARY)
                                                          : lv_color_hex(Color::INACTIVE_LIGHTER);
                    lv_obj_set_style_text_color(label, labelColor, 0);
                    lv_obj_set_style_text_opa(label, LV_OPA_COVER, 0);
                }
            }
        }
    }

    void TrackListOverlay::clearIndicators()
    {
        indicator_circles_.clear();
        folder_icons_.clear();
    }

    void TrackListOverlay::setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted)
    {
        // Update the cached state at the track index
        if (trackIndex >= 0 && trackIndex < static_cast<int>(mute_states_.size()))
        {
            mute_states_[trackIndex] = isMuted;
            updateIndicatorStates();
        }
    }

    void TrackListOverlay::setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed)
    {
        // Update the cached state at the track index
        if (trackIndex >= 0 && trackIndex < static_cast<int>(solo_states_.size()))
        {
            solo_states_[trackIndex] = isSoloed;
            updateIndicatorStates();
        }
    }

} // namespace Bitwig
