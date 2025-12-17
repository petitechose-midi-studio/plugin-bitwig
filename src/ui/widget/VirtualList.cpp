#include "VirtualList.hpp"

#include <algorithm>

#include <oc/ui/lvgl/theme/BaseTheme.hpp>

namespace bitwig::ui {

using namespace oc::ui::lvgl;

VirtualList::VirtualList(lv_obj_t* parent, int visibleCount, int itemHeight)
    : parent_(parent)
    , visibleCount_(visibleCount)
    , itemHeight_(itemHeight) {
    createContainer();
    createSlots();
    initialized_ = true;
}

VirtualList::~VirtualList() {
    for (auto& slot : slots_) {
        slot.userData = nullptr;
    }
    slots_.clear();

    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void VirtualList::setOnBindSlot(BindSlotCallback callback) {
    onBindSlot_ = std::move(callback);
}

void VirtualList::setOnUpdateHighlight(UpdateHighlightCallback callback) {
    onUpdateHighlight_ = std::move(callback);
}

void VirtualList::setTotalCount(int count) {
    bool changed = (totalCount_ != count);
    totalCount_ = count;

    if (selectedIndex_ >= totalCount_) {
        selectedIndex_ = totalCount_ > 0 ? totalCount_ - 1 : 0;
    }

    if (changed) {
        windowStart_ = -1;  // Force recalculation
        previousSelectedIndex_ = -1;  // Reset highlight tracking
        rebindAllSlots();
    }
}

void VirtualList::setSelectedIndex(int index) {
    if (totalCount_ == 0) return;

    index = std::clamp(index, 0, totalCount_ - 1);
    if (selectedIndex_ == index) return;

    int oldIndex = selectedIndex_;
    selectedIndex_ = index;

    if (visible_ && onBindSlot_) {
        updateSelection(oldIndex, index);
    }
}

void VirtualList::invalidate() {
    rebindAllSlots();
}

void VirtualList::invalidateIndex(int logicalIndex) {
    int slotIdx = logicalIndexToSlotIndex(logicalIndex);
    if (slotIdx >= 0) {
        VirtualSlot& slot = slots_[slotIdx];
        bool isSelected = (logicalIndex == selectedIndex_);
        onBindSlot_(slot, logicalIndex, isSelected);
    }
}

VirtualSlot* VirtualList::getSlotForIndex(int logicalIndex) {
    int slotIdx = logicalIndexToSlotIndex(logicalIndex);
    return (slotIdx >= 0) ? &slots_[slotIdx] : nullptr;
}

void VirtualList::show() {
    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        visible_ = true;
        rebindAllSlots();
    }
}

void VirtualList::hide() {
    if (container_) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        visible_ = false;
    }
}

// ══════════════════════════════════════════════════════════════════
// Private: Container & Slots Creation
// ══════════════════════════════════════════════════════════════════

void VirtualList::createContainer() {
    container_ = lv_obj_create(parent_);
    lv_obj_set_size(container_, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_grow(container_, 1);

    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);

    lv_obj_set_style_pad_all(container_, BaseTheme::Layout::LIST_PAD, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(container_, BaseTheme::Layout::LIST_ITEM_GAP, LV_STATE_DEFAULT);
    lv_obj_set_style_margin_left(container_, BaseTheme::Layout::MARGIN_MD, LV_STATE_DEFAULT);
    lv_obj_set_style_margin_right(container_, BaseTheme::Layout::MARGIN_MD, LV_STATE_DEFAULT);

    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

void VirtualList::createSlots() {
    slots_.reserve(visibleCount_);

    for (int i = 0; i < visibleCount_; i++) {
        VirtualSlot slot;

        slot.container = lv_obj_create(container_);
        lv_obj_set_width(slot.container, LV_PCT(100));
        lv_obj_set_height(slot.container, itemHeight_);

        lv_obj_set_style_bg_opa(slot.container, LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(slot.container, 0, LV_STATE_DEFAULT);

        lv_obj_set_style_pad_left(slot.container, BaseTheme::Layout::PAD_BUTTON_H, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(slot.container, BaseTheme::Layout::MARGIN_LG, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(slot.container, BaseTheme::Layout::PAD_BUTTON_V, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(slot.container, BaseTheme::Layout::PAD_BUTTON_V, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(slot.container, BaseTheme::Layout::MARGIN_MD, LV_STATE_DEFAULT);

        lv_obj_set_flex_flow(slot.container, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(slot.container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_clear_flag(slot.container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(slot.container, LV_OBJ_FLAG_HIDDEN);

        slot.boundIndex = -1;
        slot.userData = nullptr;

        slots_.push_back(slot);
    }
}

// ══════════════════════════════════════════════════════════════════
// Private: Core Logic (simplified)
// ══════════════════════════════════════════════════════════════════

int VirtualList::calculateWindowStart() const {
    if (totalCount_ == 0) return 0;

    // Strict page-based: selectedIndex determines page
    // Page 0: items 0 to visibleCount-1
    // Page 1: items visibleCount to 2*visibleCount-1
    // Last page may have fewer items (handled by rebindAllSlots hiding unused slots)
    int pageIndex = selectedIndex_ / visibleCount_;
    return pageIndex * visibleCount_;
}

int VirtualList::logicalIndexToSlotIndex(int logicalIndex) const {
    if (logicalIndex < windowStart_ || logicalIndex >= windowStart_ + visibleCount_) {
        return -1;
    }
    return logicalIndex - windowStart_;
}

void VirtualList::rebindAllSlots() {
    if (!onBindSlot_ || totalCount_ == 0) return;

    int newWindowStart = calculateWindowStart();
    windowStart_ = newWindowStart;

    for (int slotIdx = 0; slotIdx < visibleCount_ && slotIdx < static_cast<int>(slots_.size()); slotIdx++) {
        int logicalIndex = windowStart_ + slotIdx;
        VirtualSlot& slot = slots_[slotIdx];

        if (logicalIndex < totalCount_) {
            slot.boundIndex = logicalIndex;
            bool isSelected = (logicalIndex == selectedIndex_);
            onBindSlot_(slot, logicalIndex, isSelected);
            lv_obj_clear_flag(slot.container, LV_OBJ_FLAG_HIDDEN);
        } else {
            slot.boundIndex = -1;
            lv_obj_add_flag(slot.container, LV_OBJ_FLAG_HIDDEN);
        }
    }

    previousSelectedIndex_ = selectedIndex_;
}

void VirtualList::updateSelection(int oldIndex, int newIndex) {
    int newWindowStart = calculateWindowStart();

    if (newWindowStart != windowStart_) {
        // Page changed: rebind everything
        rebindAllSlots();
    } else {
        // Same page: just update highlights
        updateHighlightOnly(oldIndex, newIndex);
    }
}

void VirtualList::updateHighlightOnly(int oldIndex, int newIndex) {
    // Désactiver l'ancien highlight (si visible)
    int oldSlotIdx = logicalIndexToSlotIndex(oldIndex);
    if (oldSlotIdx >= 0 && onUpdateHighlight_) {
        onUpdateHighlight_(slots_[oldSlotIdx], false);
    }

    // Activer le nouveau highlight (si visible)
    int newSlotIdx = logicalIndexToSlotIndex(newIndex);
    if (newSlotIdx >= 0 && onUpdateHighlight_) {
        onUpdateHighlight_(slots_[newSlotIdx], true);
    }

    previousSelectedIndex_ = newIndex;
}

void VirtualList::rebindSlot(VirtualSlot& slot, int newIndex) {
    slot.boundIndex = newIndex;
    bool isSelected = (newIndex == selectedIndex_);
    onBindSlot_(slot, newIndex, isSelected);
}

void VirtualList::updateSlotHighlight(VirtualSlot& slot, bool isSelected) {
    if (onUpdateHighlight_) {
        onUpdateHighlight_(slot, isSelected);
    } else if (onBindSlot_ && slot.boundIndex >= 0) {
        onBindSlot_(slot, slot.boundIndex, isSelected);
    }
}

}  // namespace bitwig::ui
