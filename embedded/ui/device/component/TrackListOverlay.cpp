#include "TrackListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "resource/common/ui/font/binary_font_buffer.hpp"
#include <algorithm>

using namespace Theme;

namespace Plugin::Bitwig {

TrackListOverlay::TrackListOverlay(lv_obj_t* parent)
    : list_(parent) {
}

TrackListOverlay::~TrackListOverlay() {
    clearIndicators();
}

void TrackListOverlay::setTitle(const std::string& title) {
    list_.setTitle(title);
}

void TrackListOverlay::setItems(const std::vector<std::string>& items) {
    item_names_ = items;
    current_track_index_ = -1;
    mute_states_.clear();
    solo_states_.clear();

    list_.setItems(items);
    clearIndicators();
}

void TrackListOverlay::setTrackItems(const std::vector<std::string>& names,
                                     int currentIndex,
                                     const std::vector<bool>& muteStates,
                                     const std::vector<bool>& soloStates) {
    item_names_ = names;
    current_track_index_ = currentIndex;
    mute_states_ = muteStates;
    solo_states_ = soloStates;

    mute_states_.resize(names.size(), false);
    solo_states_.resize(names.size(), false);

    list_.setItems(names);
    list_.setSelectedIndex(currentIndex);

    clearIndicators();
    createIndicators();

    updateIndicatorStates();
}

void TrackListOverlay::setSelectedIndex(int index) {
    list_.setSelectedIndex(index);
    updateIndicatorStates();
}

void TrackListOverlay::show() {
    list_.show();
    updateIndicatorStates();
}

void TrackListOverlay::hide() {
    list_.hide();
}

bool TrackListOverlay::isVisible() const {
    return list_.isVisible();
}

int TrackListOverlay::getSelectedIndex() const {
    return list_.getSelectedIndex();
}

int TrackListOverlay::getItemCount() const {
    return list_.getItemCount();
}

void TrackListOverlay::createIndicators() {
    indicator_circles_.clear();
    indicator_circles_.resize(item_names_.size());

    for (size_t i = 0; i < item_names_.size(); i++) {
        lv_obj_t* btn = list_.getButton(i);
        if (!btn) continue;

        std::array<lv_obj_t*, 2> circles = {nullptr, nullptr};

        // Create (M) indicator - Mute
        lv_obj_t* mute_circle = lv_obj_create(btn);
        lv_obj_set_size(mute_circle, 16, 16);  // Larger circles
        lv_obj_set_style_bg_color(mute_circle, lv_color_hex(Color::TRACK_MUTE), 0);
        lv_obj_set_style_bg_opa(mute_circle, LV_OPA_50, 0);
        lv_obj_set_style_radius(mute_circle, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(mute_circle, 0, 0);
        lv_obj_set_style_pad_all(mute_circle, 0, 0);
        lv_obj_clear_flag(mute_circle, LV_OBJ_FLAG_SCROLLABLE);

        // Add "M" label in mute circle - using flex for proper centering
        lv_obj_set_flex_flow(mute_circle, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(mute_circle, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t* mute_label = lv_label_create(mute_circle);
        lv_label_set_text(mute_label, "M");
        if (fonts.parameter_label) {
            lv_obj_set_style_text_font(mute_label, fonts.parameter_label, 0);
        }
        lv_obj_set_style_text_color(mute_label, lv_color_hex(Color::TEXT_PRIMARY), 0);
        lv_obj_set_style_translate_y(mute_label, -1, 0);  // Shift up to compensate for baseline

        // Move mute circle to index 0 (before label, no bullet anymore)
        lv_obj_move_to_index(mute_circle, 0);

        circles[0] = mute_circle;

        // Create (S) indicator - Solo
        lv_obj_t* solo_circle = lv_obj_create(btn);
        lv_obj_set_size(solo_circle, 16, 16);  // Larger circles
        lv_obj_set_style_bg_color(solo_circle, lv_color_hex(Color::TRACK_SOLO), 0);
        lv_obj_set_style_bg_opa(solo_circle, LV_OPA_50, 0);
        lv_obj_set_style_radius(solo_circle, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(solo_circle, 0, 0);
        lv_obj_set_style_pad_all(solo_circle, 0, 0);
        lv_obj_clear_flag(solo_circle, LV_OBJ_FLAG_SCROLLABLE);

        // Add "S" label in solo circle - using flex for proper centering
        lv_obj_set_flex_flow(solo_circle, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(solo_circle, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t* solo_label = lv_label_create(solo_circle);
        lv_label_set_text(solo_label, "S");
        if (fonts.parameter_label) {
            lv_obj_set_style_text_font(solo_label, fonts.parameter_label, 0);
        }
        lv_obj_set_style_text_color(solo_label, lv_color_hex(Color::TEXT_PRIMARY), 0);
        lv_obj_set_style_translate_y(solo_label, -1, 0);  // Shift up to compensate for baseline

        // Move solo circle to index 1 (after mute, before label)
        lv_obj_move_to_index(solo_circle, 1);

        circles[1] = solo_circle;

        indicator_circles_[i] = circles;
    }
}

void TrackListOverlay::updateIndicatorStates() {
    int itemCount = list_.getItemCount();
    int selectedIndex = list_.getSelectedIndex();

    for (int i = 0; i < itemCount; i++) {
        if (i >= static_cast<int>(indicator_circles_.size())) continue;

        lv_obj_t* mute_circle = indicator_circles_[i][0];
        lv_obj_t* solo_circle = indicator_circles_[i][1];

        if (!mute_circle || !solo_circle) continue;

        // Check if this is a special item (like "Back to parent")
        bool isSpecialItem = false;
        if (i < static_cast<int>(item_names_.size())) {
            const std::string& itemName = item_names_[i];
            isSpecialItem = (!itemName.empty() && itemName[0] == '[') ||
                           (itemName == "Back to parent");
        }

        if (isSpecialItem) {
            // Hide indicators for special items
            lv_obj_set_style_bg_opa(mute_circle, LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_opa(solo_circle, LV_OPA_TRANSP, 0);
            continue;
        }

        // Get mute/solo states
        bool isMuted = (i < static_cast<int>(mute_states_.size())) ? mute_states_[i] : false;
        bool isSoloed = (i < static_cast<int>(solo_states_.size())) ? solo_states_[i] : false;

        bool isHighlighted = (i == selectedIndex);

        // Update mute indicator opacity - always full opacity when active
        lv_opa_t mute_opa = LV_OPA_50;
        if (isMuted) {
            mute_opa = LV_OPA_COVER;  // Always 100% opacity when muted
        } else {
            mute_opa = isHighlighted ? LV_OPA_40 : LV_OPA_20;
        }
        lv_obj_set_style_bg_opa(mute_circle, mute_opa, 0);

        // Update solo indicator opacity - always full opacity when active
        lv_opa_t solo_opa = LV_OPA_50;
        if (isSoloed) {
            solo_opa = LV_OPA_COVER;  // Always 100% opacity when soloed
        } else {
            solo_opa = isHighlighted ? LV_OPA_40 : LV_OPA_20;
        }
        lv_obj_set_style_bg_opa(solo_circle, solo_opa, 0);

        // Update label color based on highlight state
        lv_obj_t* btn = list_.getButton(i);
        if (btn) {
            // Get label (child at index 2: mute=0, solo=1, label=2)
            lv_obj_t* label = lv_obj_get_child(btn, 2);
            if (label) {
                // Change color based on highlight (like device list)
                lv_color_t label_color = isHighlighted
                    ? lv_color_hex(Color::TEXT_PRIMARY)
                    : lv_color_hex(Color::INACTIVE_LIGHTER);
                lv_obj_set_style_text_color(label, label_color, 0);
            }
        }
    }
}

void TrackListOverlay::clearIndicators() {
    indicator_circles_.clear();
}

void TrackListOverlay::setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted) {
    // Update the cached state at the track index
    if (trackIndex >= 0 && trackIndex < static_cast<int>(mute_states_.size())) {
        mute_states_[trackIndex] = isMuted;
        updateIndicatorStates();
    }
}

void TrackListOverlay::setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed) {
    // Update the cached state at the track index
    if (trackIndex >= 0 && trackIndex < static_cast<int>(solo_states_.size())) {
        solo_states_[trackIndex] = isSoloed;
        updateIndicatorStates();
    }
}

} // namespace Plugin::Bitwig
