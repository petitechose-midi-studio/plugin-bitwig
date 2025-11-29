#pragma once

#include "widget/BaseSelector.hpp"
#include "ButtonHintBar.hpp"
#include "TrackTitleItem.hpp"
#include <string>
#include <vector>
#include <memory>

namespace Bitwig
{

/**
 * Track list selector with mute/solo indicators.
 */
class TrackListSelector : public UI::BaseSelector
{
public:
    explicit TrackListSelector(lv_obj_t *parent);
    ~TrackListSelector() override;

    void setItems(const std::vector<std::string> &items);

    void setTrackItems(const std::vector<std::string> &items,
                       int currentIndex,
                       const std::vector<bool> &muteStates,
                       const std::vector<bool> &soloStates,
                       const std::vector<uint8_t> &trackTypes,
                       const std::vector<uint32_t> &trackColors);

    void setCurrentItemIndex(int index);
    void setSelectedIndex(int index) override;

    void show() override;
    void hide() override;

    void setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted);
    void setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed);
    bool getTrackMuteStateAtIndex(uint8_t trackIndex) const;
    bool getTrackSoloStateAtIndex(uint8_t trackIndex) const;

private:
    void createFooter();
    void updateFooterStates();
    void createTrackItems();
    void clearTrackItems();
    void updateHighlightStates();

    std::vector<std::string> items_;
    std::vector<bool> mute_states_;
    std::vector<bool> solo_states_;
    std::vector<uint8_t> track_types_;
    std::vector<uint32_t> track_colors_;
    std::vector<std::unique_ptr<TrackTitleItem>> track_items_;
    int current_item_index_ = 0;

    std::unique_ptr<ButtonHintBar> footer_;
};

} // namespace Bitwig
