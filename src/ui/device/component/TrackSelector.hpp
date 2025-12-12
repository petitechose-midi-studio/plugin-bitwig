#pragma once

#include "TrackTitleItem.hpp"

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

#include "ui/widget/HintBar.hpp"
#include "ui/widget/BaseSelector.hpp"

namespace Bitwig {

struct TrackSelectorProps {
    const std::vector<std::string> *names = nullptr;
    const std::vector<bool> *muteStates = nullptr;
    const std::vector<bool> *soloStates = nullptr;
    const std::vector<uint8_t> *trackTypes = nullptr;
    const std::vector<uint32_t> *trackColors = nullptr;
    int selectedIndex = 0;
    bool visible = false;
};

/**
 * Track list selector with mute/solo indicators.
 * Stateless - all data comes from props.
 */
class TrackSelector : public BaseSelector {
public:
    explicit TrackSelector(lv_obj_t *parent);
    ~TrackSelector() override;

    void render(const TrackSelectorProps &props);
private:
    void createFooter();
    void createTrackItems(const std::vector<std::string> &names);
    void clearTrackItems();
    void renderTrackItems(const TrackSelectorProps &props);
    void renderFooter(const TrackSelectorProps &props);

    std::vector<std::string> prev_items_;  // Cache for itemsChanged detection
    std::vector<std::unique_ptr<TrackTitleItem>> track_items_;

    std::unique_ptr<HintBar> footer_;
    lv_obj_t *footer_mute_ = nullptr;
    lv_obj_t *footer_solo_ = nullptr;
};

}  // namespace Bitwig
