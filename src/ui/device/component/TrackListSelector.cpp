#include "TrackListSelector.hpp"
#include "ui/theme/BitwigTheme.hpp"

namespace Bitwig
{

    TrackListSelector::TrackListSelector(lv_obj_t *parent)
        : parent_(parent), overlay_(parent)
    {
        overlay_.setTitle("Tracks");
    }

    TrackListSelector::~TrackListSelector()
    {
        destroyFooter();
    }

    void TrackListSelector::setTitle(const std::string &title)
    {
        overlay_.setTitle(title);
    }

    void TrackListSelector::setItems(const std::vector<std::string> &items)
    {
        items_ = items;
        overlay_.setItems(items_);
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
        overlay_.setTrackItems(items, currentIndex, muteStates, soloStates, trackTypes, trackColors);
    }

    void TrackListSelector::setCurrentItemIndex(int index)
    {
        current_item_index_ = index;
        overlay_.setSelectedIndex(index);
    }

    void TrackListSelector::setSelectedIndex(int index)
    {
        overlay_.setSelectedIndex(index);
        updateFooterStates();
    }

    void TrackListSelector::show()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();

        if (!footer_)
        {
            createFooter();
        }
        footer_->show();
        updateFooterStates();
    }

    void TrackListSelector::hide()
    {
        overlay_.hide();

        if (footer_)
        {
            footer_->hide();
        }
    }

    bool TrackListSelector::isVisible() const
    {
        return overlay_.isVisible();
    }

    int TrackListSelector::getSelectedIndex() const
    {
        return overlay_.getSelectedIndex();
    }

    int TrackListSelector::getItemCount() const
    {
        return overlay_.getItemCount();
    }

    void TrackListSelector::createFooter()
    {
        if (!parent_)
            return;

        footer_ = std::make_unique<ButtonHintBar>(parent_);
        footer_->setCenterIcon(Icon::MUTE, Theme::Color::TRACK_MUTE);
        footer_->setRightIcon(Icon::SOLO, Theme::Color::TRACK_SOLO);
    }

    void TrackListSelector::destroyFooter()
    {
        footer_.reset();
    }

    void TrackListSelector::updateFooterStates()
    {
        if (!footer_)
            return;

        int selectedIndex = overlay_.getSelectedIndex();
        bool isMuted = overlay_.getTrackMuteStateAtIndex(selectedIndex);
        bool isSoloed = overlay_.getTrackSoloStateAtIndex(selectedIndex);

        footer_->setCenterIconActive(isMuted);
        footer_->setRightIconActive(isSoloed);
    }

    void TrackListSelector::setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted)
    {
        overlay_.setTrackMuteStateAtIndex(trackIndex, isMuted);
        updateFooterStates();
    }

    void TrackListSelector::setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed)
    {
        overlay_.setTrackSoloStateAtIndex(trackIndex, isSoloed);
        updateFooterStates();
    }

    bool TrackListSelector::getTrackMuteStateAtIndex(uint8_t trackIndex) const
    {
        return overlay_.getTrackMuteStateAtIndex(trackIndex);
    }

    bool TrackListSelector::getTrackSoloStateAtIndex(uint8_t trackIndex) const
    {
        return overlay_.getTrackSoloStateAtIndex(trackIndex);
    }

} // namespace Bitwig
