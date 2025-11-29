#include "TrackListSelector.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/icon.hpp"
#include "ui/font/FontLoader.hpp"

using namespace Theme;

namespace Bitwig
{

TrackListSelector::TrackListSelector(lv_obj_t *parent)
    : BaseSelector(parent)
{
    setTitle("Tracks");
}

TrackListSelector::~TrackListSelector()
{
    footer_.reset();
}

void TrackListSelector::setItems(const std::vector<std::string> &items)
{
    items_ = items;
    mute_states_.clear();
    solo_states_.clear();
    track_types_.clear();
    track_colors_.clear();

    overlay().setItems(items);
    clearTrackItems();
}

void TrackListSelector::setTrackItems(const std::vector<std::string> &items,
                                      int currentIndex,
                                      const std::vector<bool> &muteStates,
                                      const std::vector<bool> &soloStates,
                                      const std::vector<uint8_t> &trackTypes,
                                      const std::vector<uint32_t> &trackColors)
{
    items_ = items;
    current_item_index_ = currentIndex;
    mute_states_ = muteStates;
    solo_states_ = soloStates;
    track_types_ = trackTypes;
    track_colors_ = trackColors;

    mute_states_.resize(items.size(), false);
    solo_states_.resize(items.size(), false);
    track_types_.resize(items.size(), 0);
    track_colors_.resize(items.size(), 0xFFFFFF);

    overlay().setItems(items);
    overlay().setSelectedIndex(currentIndex);

    clearTrackItems();
    createTrackItems();
    updateHighlightStates();
}

void TrackListSelector::setCurrentItemIndex(int index)
{
    current_item_index_ = index;
    overlay().setSelectedIndex(index);
}

void TrackListSelector::setSelectedIndex(int index)
{
    BaseSelector::setSelectedIndex(index);
    updateHighlightStates();
    updateFooterStates();
}

void TrackListSelector::show()
{
    overlay().setSelectedIndex(current_item_index_);
    BaseSelector::show();
    updateHighlightStates();

    if (!footer_)
        createFooter();
    footer_->show();
    updateFooterStates();
}

void TrackListSelector::hide()
{
    BaseSelector::hide();
    if (footer_)
        footer_->hide();
}

void TrackListSelector::createFooter()
{
    footer_ = std::make_unique<ButtonHintBar>(parent_);
    footer_->setCenterIcon(Icon::MUTE, Color::TRACK_MUTE);
    footer_->setRightIcon(Icon::SOLO, Color::TRACK_SOLO);
}

void TrackListSelector::updateFooterStates()
{
    if (!footer_)
        return;

    int selectedIndex = getSelectedIndex();
    bool isMuted = getTrackMuteStateAtIndex(selectedIndex);
    bool isSoloed = getTrackSoloStateAtIndex(selectedIndex);

    footer_->setCenterIconActive(isMuted);
    footer_->setRightIconActive(isSoloed);
}

void TrackListSelector::createTrackItems()
{
    track_items_.clear();

    for (size_t i = 0; i < items_.size(); i++)
    {
        lv_obj_t *btn = overlay().getButton(i);
        if (!btn)
            continue;

        bool isBackItem = (i == 0) && !items_.empty() && (items_[0] == Icon::ARROW_LEFT);

        // Remove default label
        uint32_t childCount = lv_obj_get_child_cnt(btn);
        for (uint32_t j = 0; j < childCount; j++)
        {
            lv_obj_t *child = lv_obj_get_child(btn, j);
            if (child && lv_obj_check_type(child, &lv_label_class))
            {
                if (isBackItem)
                {
                    if (bitwig_fonts.icons_14)
                        lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
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

        if (!isBackItem)
        {
            auto item = std::make_unique<TrackTitleItem>(btn, true, 12);
            item->setName(items_[i]);
            item->setColor(track_colors_[i]);
            item->setType(track_types_[i]);
            item->setMuteState(mute_states_[i]);
            item->setSoloState(solo_states_[i]);
            track_items_.push_back(std::move(item));
        }
        else
        {
            track_items_.push_back(nullptr);
        }
    }
}

void TrackListSelector::clearTrackItems()
{
    track_items_.clear();
}

void TrackListSelector::updateHighlightStates()
{
    int selectedIndex = getSelectedIndex();
    for (size_t i = 0; i < track_items_.size(); i++)
    {
        if (track_items_[i])
            track_items_[i]->setHighlighted(static_cast<int>(i) == selectedIndex);
    }
}

void TrackListSelector::setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted)
{
    if (trackIndex < mute_states_.size())
    {
        mute_states_[trackIndex] = isMuted;
        if (trackIndex < track_items_.size() && track_items_[trackIndex])
            track_items_[trackIndex]->setMuteState(isMuted);
    }
    updateFooterStates();
}

void TrackListSelector::setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed)
{
    if (trackIndex < solo_states_.size())
    {
        solo_states_[trackIndex] = isSoloed;
        if (trackIndex < track_items_.size() && track_items_[trackIndex])
            track_items_[trackIndex]->setSoloState(isSoloed);
    }
    updateFooterStates();
}

bool TrackListSelector::getTrackMuteStateAtIndex(uint8_t trackIndex) const
{
    return trackIndex < mute_states_.size() ? mute_states_[trackIndex] : false;
}

bool TrackListSelector::getTrackSoloStateAtIndex(uint8_t trackIndex) const
{
    return trackIndex < solo_states_.size() ? solo_states_[trackIndex] : false;
}

} // namespace Bitwig
