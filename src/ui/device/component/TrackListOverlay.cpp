#include "TrackListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/icon.hpp"

using namespace Theme;

namespace Bitwig
{

    TrackListOverlay::TrackListOverlay(lv_obj_t *parent)
        : list_(parent)
    {
    }

    TrackListOverlay::~TrackListOverlay()
    {
        clearTrackItems();
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
        clearTrackItems();
    }

    void TrackListOverlay::setTrackItems(const std::vector<std::string> &names,
                                         int currentIndex,
                                         const std::vector<bool> &muteStates,
                                         const std::vector<bool> &soloStates,
                                         const std::vector<uint8_t> &trackTypes,
                                         const std::vector<uint32_t> &trackColors)
    {
        item_names_ = names;
        current_track_index_ = currentIndex;
        mute_states_ = muteStates;
        solo_states_ = soloStates;
        track_types_ = trackTypes;
        track_colors_ = trackColors;

        mute_states_.resize(names.size(), false);
        solo_states_.resize(names.size(), false);
        track_types_.resize(names.size(), 0);
        track_colors_.resize(names.size(), 0xFFFFFF);

        list_.setItems(names);
        list_.setSelectedIndex(currentIndex);

        clearTrackItems();
        createTrackItems();
        updateHighlightStates();
    }

    void TrackListOverlay::setSelectedIndex(int index)
    {
        list_.setSelectedIndex(index);
        updateHighlightStates();
    }

    void TrackListOverlay::show()
    {
        list_.show();
        updateHighlightStates();
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

    void TrackListOverlay::createTrackItems()
    {
        track_items_.clear();

        for (size_t i = 0; i < item_names_.size(); i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            // Check if this is the "Back" item
            bool isBackItem = (i == 0) && !item_names_.empty() && (item_names_[0] == Icon::ARROW_LEFT);

            // Remove the default label created by ListOverlay
            uint32_t childCount = lv_obj_get_child_cnt(btn);
            for (uint32_t j = 0; j < childCount; j++)
            {
                lv_obj_t *child = lv_obj_get_child(btn, j);
                if (child && lv_obj_check_type(child, &lv_label_class))
                {
                    // For back item, keep the label but set icon font
                    if (isBackItem)
                    {
                        if (bitwig_fonts.icons_14)
                        {
                            lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
                        }
                        // Apply state-based colors like other items
                        lv_obj_set_style_text_color(child, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DEFAULT);
                        lv_obj_set_style_text_color(child, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_FOCUSED);
                    }
                    else
                    {
                        lv_obj_delete(child);
                    }
                    break;
                }
            }

            // Create TrackTitleItem for non-back items
            if (!isBackItem)
            {
                bool showMuteSolo = true;  // List items show mute/solo
                lv_coord_t barHeight = 12; // Fixed height for list items (parent has LV_SIZE_CONTENT)
                auto item = std::make_unique<TrackTitleItem>(btn, showMuteSolo, barHeight);

                item->setName(item_names_[i]);
                item->setColor(track_colors_[i]);
                item->setType(track_types_[i]);
                item->setMuteState(mute_states_[i]);
                item->setSoloState(solo_states_[i]);

                track_items_.push_back(std::move(item));
            }
            else
            {
                // Back item has no TrackTitleItem, push nullptr to keep indices aligned
                track_items_.push_back(nullptr);
            }
        }
    }

    void TrackListOverlay::clearTrackItems()
    {
        track_items_.clear();
    }

    void TrackListOverlay::updateHighlightStates()
    {
        int selectedIndex = list_.getSelectedIndex();

        for (size_t i = 0; i < track_items_.size(); i++)
        {
            if (track_items_[i])
            {
                bool isHighlighted = (static_cast<int>(i) == selectedIndex);
                track_items_[i]->setHighlighted(isHighlighted);
            }
        }
    }

    void TrackListOverlay::setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted)
    {
        if (trackIndex < mute_states_.size())
        {
            mute_states_[trackIndex] = isMuted;

            if (trackIndex < track_items_.size() && track_items_[trackIndex])
            {
                track_items_[trackIndex]->setMuteState(isMuted);
            }
        }
    }

    void TrackListOverlay::setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed)
    {
        if (trackIndex < solo_states_.size())
        {
            solo_states_[trackIndex] = isSoloed;

            if (trackIndex < track_items_.size() && track_items_[trackIndex])
            {
                track_items_[trackIndex]->setSoloState(isSoloed);
            }
        }
    }

    bool TrackListOverlay::getTrackMuteStateAtIndex(uint8_t trackIndex) const
    {
        if (trackIndex < mute_states_.size())
        {
            return mute_states_[trackIndex];
        }
        return false;
    }

    bool TrackListOverlay::getTrackSoloStateAtIndex(uint8_t trackIndex) const
    {
        if (trackIndex < solo_states_.size())
        {
            return solo_states_[trackIndex];
        }
        return false;
    }

} // namespace Bitwig
