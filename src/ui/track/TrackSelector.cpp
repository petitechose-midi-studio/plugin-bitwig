#include "TrackSelector.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>
#include <oc/ui/lvgl/theme/BaseTheme.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

namespace bitwig {

using namespace oc::ui::lvgl;
using namespace theme;
namespace style = oc::ui::lvgl::style;

// ══════════════════════════════════════════════════════════════════
// Constants
// ══════════════════════════════════════════════════════════════════

constexpr int VISIBLE_SLOTS = 5;
constexpr int ITEM_HEIGHT = 32;

// ══════════════════════════════════════════════════════════════════
// Construction
// ══════════════════════════════════════════════════════════════════

TrackSelector::TrackSelector(lv_obj_t *parent) : parent_(parent) {
    createOverlay();
    createHeader();

    // Create virtual list with fluent API
    list_ = std::make_unique<VirtualList>(container_);
    list_->visibleCount(VISIBLE_SLOTS)
        .itemHeight(ITEM_HEIGHT)
        .onBindSlot([this](VirtualSlot &slot, int index, bool isSelected) {
            bindSlot(slot, index, isSelected);
        })
        .onUpdateHighlight([this](VirtualSlot &slot, bool isSelected) {
            updateSlotHighlight(slot, isSelected);
        });

    // Pre-allocate slot items and back buttons
    slotItems_.resize(VISIBLE_SLOTS);
    backButtons_.resize(VISIBLE_SLOTS);

    createFooter();
}

TrackSelector::~TrackSelector() {
    slotItems_.clear();
    backButtons_.clear();
    list_.reset();
    footer_.reset();

    if (overlay_) {
        lv_obj_delete(overlay_);
        overlay_ = nullptr;
    }
}

// ══════════════════════════════════════════════════════════════════
// Render
// ══════════════════════════════════════════════════════════════════

void TrackSelector::render(const TrackSelectorProps &props) {
    if (!props.visible) {
        hide();
        return;
    }

    if (props.names.empty()) return;

    // Store current props for access in callbacks
    currentProps_ = props;

    // Update list - invalidate only if count didn't change (avoid double-rebind)
    bool countChanged = list_->setTotalCount(static_cast<int>(props.names.size()));
    list_->setSelectedIndex(props.selectedIndex);
    if (!countChanged) {
        list_->invalidate();
    }

    // Show overlay and list
    if (!visible_) show();

    renderFooter(props);
}

// ══════════════════════════════════════════════════════════════════
// IComponent
// ══════════════════════════════════════════════════════════════════

void TrackSelector::show() {
    if (overlay_) lv_obj_clear_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    if (list_) list_->show();
    if (footer_) footer_->show();
    visible_ = true;
}

void TrackSelector::hide() {
    if (overlay_) lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    if (list_) list_->hide();
    if (footer_) footer_->hide();
    visible_ = false;
}

// ══════════════════════════════════════════════════════════════════
// Overlay Structure
// ══════════════════════════════════════════════════════════════════

void TrackSelector::createOverlay() {
    // Full-screen overlay with semi-transparent background
    overlay_ = lv_obj_create(parent_);
    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_FLOATING);
    style::apply(overlay_).fullSize().bgColor(BaseTheme::Color::BACKGROUND, Opacity::OVERLAY_BG).noScroll();
    lv_obj_align(overlay_, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(overlay_, 0, LV_STATE_DEFAULT);
    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);

    // Container with flex column layout
    container_ = lv_obj_create(overlay_);
    style::apply(container_).fullSize().transparent().noScroll();
    lv_obj_align(container_, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(container_, BaseTheme::Layout::ROW_GAP_MD, LV_STATE_DEFAULT);
}

void TrackSelector::createHeader() {
    header_ = lv_obj_create(container_);
    lv_obj_set_size(header_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(header_, Opacity::HIDDEN, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(header_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(header_, Layout::OVERLAY_PAD_H, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(header_, Layout::OVERLAY_PAD_H, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(header_, Layout::OVERLAY_PAD_TOP, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(header_, Layout::OVERLAY_PAD_BOTTOM, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(header_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(header_, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_move_to_index(header_, 0);

    header_label_ = lv_label_create(header_);
    lv_label_set_text(header_label_, "Tracks");
    if (bitwig_fonts.device_label) {
        lv_obj_set_style_text_font(header_label_, bitwig_fonts.device_label, LV_STATE_DEFAULT);
    }
    style::apply(header_label_).textColor(Color::TEXT_LIGHT);
}

void TrackSelector::createFooter() {
    footer_ = std::make_unique<HintBar>(container_, HintBarPosition::BOTTOM);
    footer_->setSize(Layout::HINT_BAR_HEIGHT);

    // Mute icon (cell 1 = center)
    footer_mute_ = lv_label_create(footer_->getElement());
    Icon::set(footer_mute_, Icon::CHANNEL_MUTE, Icon::Size::L);
    style::apply(footer_mute_).textColor(Color::TRACK_MUTE);
    footer_->setCell(1, footer_mute_);

    // Solo icon (cell 2 = right)
    footer_solo_ = lv_label_create(footer_->getElement());
    Icon::set(footer_solo_, Icon::CHANNEL_SOLO, Icon::Size::L);
    style::apply(footer_solo_).textColor(Color::TRACK_SOLO);
    footer_->setCell(2, footer_solo_);
}

void TrackSelector::renderFooter(const TrackSelectorProps &props) {
    if (!footer_) return;

    int idx = props.selectedIndex;
    bool is_muted = idx >= 0 && idx < static_cast<int>(props.muteStates.size())
                        ? props.muteStates[idx]
                        : false;
    bool is_soloed = idx >= 0 && idx < static_cast<int>(props.soloStates.size())
                         ? props.soloStates[idx]
                         : false;

    if (footer_mute_)
        lv_obj_set_style_text_opa(footer_mute_, is_muted ? Opacity::FULL : Opacity::FADED,
                                  LV_STATE_DEFAULT);
    if (footer_solo_)
        lv_obj_set_style_text_opa(footer_solo_, is_soloed ? Opacity::FULL : Opacity::FADED,
                                  LV_STATE_DEFAULT);

    footer_->show();
}

// ══════════════════════════════════════════════════════════════════
// VirtualList Callbacks
// ══════════════════════════════════════════════════════════════════

void TrackSelector::bindSlot(VirtualSlot &slot, int index, bool isSelected) {
    int slotIndex = index - list_->getWindowStart();
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    // Ensure widgets exist for this slot
    ensureSlotWidgets(slotIndex);

    // Get data for this index
    const auto &props = currentProps_;
    const char *name = index < static_cast<int>(props.names.size())
                           ? props.names[index].c_str()
                           : "";

    // Check if this is the back button
    bool isBack = (index == 0) && (std::string(name) == Icon::UI_ARROW_LEFT);

    if (isBack) {
        // Show back button, hide TrackTitleItem
        if (backButtons_[slotIndex]) {
            backButtons_[slotIndex]->show();
            backButtons_[slotIndex]->setHighlighted(isSelected);
        }
        if (slotItems_[slotIndex]) {
            slotItems_[slotIndex]->hide();
        }
    } else {
        // Hide back button, show TrackTitleItem
        if (backButtons_[slotIndex]) {
            backButtons_[slotIndex]->hide();
        }
        if (slotItems_[slotIndex]) {
            slotItems_[slotIndex]->show();
        }

        uint32_t color = index < static_cast<int>(props.trackColors.size())
                             ? props.trackColors[index]
                             : 0xFFFFFF;
        uint8_t trackType = index < static_cast<int>(props.trackTypes.size())
                                ? props.trackTypes[index]
                                : 0;
        bool isMuted = index < static_cast<int>(props.muteStates.size())
                           ? props.muteStates[index]
                           : false;
        bool isSoloed = index < static_cast<int>(props.soloStates.size())
                            ? props.soloStates[index]
                            : false;

        slotItems_[slotIndex]->render({
            .name = name,
            .color = color,
            .trackType = trackType,
            .isMuted = isMuted,
            .isSoloed = isSoloed,
            .level = 0.0f,
            .highlighted = isSelected,
            .hideIndicators = false
        });
    }
}

void TrackSelector::updateSlotHighlight(VirtualSlot &slot, bool isSelected) {
    int slotIndex = slot.boundIndex - list_->getWindowStart();
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    applyHighlightStyle(slotIndex, isSelected);
}

void TrackSelector::ensureSlotWidgets(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    const auto &slots = list_->getSlots();
    if (slotIndex >= static_cast<int>(slots.size())) return;

    lv_obj_t *container = slots[slotIndex].container;

    // Create BackButton if not exists (hidden by default)
    if (!backButtons_[slotIndex]) {
        backButtons_[slotIndex] = std::make_unique<BackButton>(container);
    }

    // Create TrackTitleItem if not exists
    if (!slotItems_[slotIndex]) {
        slotItems_[slotIndex] = std::make_unique<TrackTitleItem>(container, true, 12);
    }
}

void TrackSelector::applyHighlightStyle(int slotIndex, bool isSelected) {
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    // Get current data for this slot
    int logicalIndex = list_->getWindowStart() + slotIndex;
    const auto &props = currentProps_;

    const char *name = logicalIndex < static_cast<int>(props.names.size())
                           ? props.names[logicalIndex].c_str()
                           : "";

    // Check if this is the back button
    bool isBack = (logicalIndex == 0) && (std::string(name) == Icon::UI_ARROW_LEFT);

    if (isBack) {
        // Update back button highlight
        if (backButtons_[slotIndex]) {
            backButtons_[slotIndex]->setHighlighted(isSelected);
        }
    } else {
        // Update TrackTitleItem highlight
        if (!slotItems_[slotIndex]) return;

        uint32_t color = logicalIndex < static_cast<int>(props.trackColors.size())
                             ? props.trackColors[logicalIndex]
                             : 0xFFFFFF;
        uint8_t trackType = logicalIndex < static_cast<int>(props.trackTypes.size())
                                ? props.trackTypes[logicalIndex]
                                : 0;
        bool isMuted = logicalIndex < static_cast<int>(props.muteStates.size())
                           ? props.muteStates[logicalIndex]
                           : false;
        bool isSoloed = logicalIndex < static_cast<int>(props.soloStates.size())
                            ? props.soloStates[logicalIndex]
                            : false;

        slotItems_[slotIndex]->render({
            .name = name,
            .color = color,
            .trackType = trackType,
            .isMuted = isMuted,
            .isSoloed = isSoloed,
            .level = 0.0f,
            .highlighted = isSelected,
            .hideIndicators = false
        });
    }
}

}  // namespace bitwig
