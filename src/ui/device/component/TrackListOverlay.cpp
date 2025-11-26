#include "TrackListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"
#include "../../LVGLSymbol.hpp"
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
                                         const std::vector<bool> &groupStates,
                                         const std::vector<uint32_t> &trackColors)
    {
        item_names_ = names;
        current_track_index_ = currentIndex;
        mute_states_ = muteStates;
        solo_states_ = soloStates;
        group_states_ = groupStates;
        track_colors_ = trackColors;

        mute_states_.resize(names.size(), false);
        solo_states_.resize(names.size(), false);
        group_states_.resize(names.size(), false);
        track_colors_.resize(names.size(), 0xFFFFFF);

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

    lv_obj_t *TrackListOverlay::createVerticalBar(lv_obj_t *parent, uint32_t color)
    {
        lv_obj_t *bar = lv_obj_create(parent);
        lv_obj_set_size(bar, 4, 12);
        lv_obj_set_style_bg_color(bar, lv_color_hex(color), 0);
        lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(bar, 0, 0);
        lv_obj_set_style_border_width(bar, 0, 0);
        lv_obj_set_style_pad_all(bar, 0, 0);
        lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);
        return bar;
    }

    void TrackListOverlay::createIndicators()
    {
        indicator_circles_.clear();
        indicator_circles_.resize(item_names_.size());
        folder_icons_.clear();
        folder_icons_.resize(item_names_.size(), nullptr);
        vertical_bars_.clear();
        vertical_bars_.resize(item_names_.size(), nullptr);

        for (size_t i = 0; i < item_names_.size(); i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            // Check if this is the "Back" item (first item with the Back LVGLSymbol)
            bool isBackItem = (i == 0) && (i < item_names_.size()) && (item_names_[i] == LVGLSymbol::BACK);

            // Apply lvgl_symbols font to text label if this is the Back item
            if (isBackItem)
            {
                // Find the text label (first label child)
                uint32_t childCount = lv_obj_get_child_cnt(btn);
                for (uint32_t j = 0; j < childCount; j++)
                {
                    lv_obj_t *child = lv_obj_get_child(btn, j);
                    if (child && lv_obj_check_type(child, &lv_label_class))
                    {
                        if (bitwig_fonts.icons)
                        {
                            lv_obj_set_style_text_font(child, bitwig_fonts.icons, 0);
                        }
                        break;
                    }
                }
            }

            // Create vertical bar at the left edge with track color
            uint32_t barColor = (i < track_colors_.size()) ? track_colors_[i] : 0xFFFFFF;
            lv_obj_t *vertical_bar = createVerticalBar(btn, barColor);
            lv_obj_move_to_index(vertical_bar, 0);
            vertical_bars_[i] = vertical_bar;

            std::array<lv_obj_t *, 2> labels = {nullptr, nullptr};

            // Don't create mute/solo indicators for Back item
            if (!isBackItem)
            {
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
            }

            // Create folder icon if this is a group
            bool isGroup = (i < group_states_.size()) ? group_states_[i] : false;
            if (isGroup)
            {
                lv_obj_t *folder_icon = lv_label_create(btn);
                lv_label_set_text(folder_icon, LVGLSymbol::FOLDER);
                if (bitwig_fonts.icons)
                {
                    lv_obj_set_style_text_font(folder_icon, bitwig_fonts.icons, 0);
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

            // Check if this is a special item (Back or device children like "[S] Slots")
            bool isSpecialItem = false;
            if (i < static_cast<int>(item_names_.size()))
            {
                const std::string &itemName = item_names_[i];
                isSpecialItem = (itemName == LVGLSymbol::BACK) ||
                                (!itemName.empty() && itemName[0] == '[');
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
        vertical_bars_.clear();
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

    bool TrackListOverlay::getTrackMuteStateAtIndex(uint8_t trackIndex) const
    {
        if (trackIndex >= 0 && trackIndex < static_cast<int>(mute_states_.size()))
        {
            return mute_states_[trackIndex];
        }
        return false;
    }

    bool TrackListOverlay::getTrackSoloStateAtIndex(uint8_t trackIndex) const
    {
        if (trackIndex >= 0 && trackIndex < static_cast<int>(solo_states_.size()))
        {
            return solo_states_[trackIndex];
        }
        return false;
    }

} // namespace Bitwig
