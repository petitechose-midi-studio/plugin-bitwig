#include "RemoteControlsPageSelector.hpp"

#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/style/StyleBuilder.hpp>
#include <oc/ui/lvgl/theme/BaseTheme.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

namespace bitwig::ui {

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

RemoteControlsPageSelector::RemoteControlsPageSelector(lv_obj_t* parent)
    : parent_(parent) {
    createOverlay();
    createHeader();

    // Create virtual list with fluent API
    list_ = std::make_unique<VirtualList>(container_);
    list_->visibleCount(VISIBLE_SLOTS)
        .itemHeight(ITEM_HEIGHT)
        .scrollMode(ScrollMode::CenterLocked)
        .onBindSlot([this](VirtualSlot& slot, int index, bool isSelected) {
            bindSlot(slot, index, isSelected);
        })
        .onUpdateHighlight([this](VirtualSlot& slot, bool isSelected) {
            updateSlotHighlight(slot, isSelected);
        });

    // Pre-allocate slot widgets
    slotWidgets_.resize(VISIBLE_SLOTS);

    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
}

RemoteControlsPageSelector::~RemoteControlsPageSelector() {
    list_.reset();

    if (overlay_) {
        lv_obj_delete(overlay_);
        overlay_ = nullptr;
    }
}

// ══════════════════════════════════════════════════════════════════
// Render
// ══════════════════════════════════════════════════════════════════

void RemoteControlsPageSelector::render(const RemoteControlsPageSelectorProps& props) {
    OC_LOG_DEBUG("[PageSelector] render() visible={} names.size()={} totalCount={} selectedIdx={}",
                 props.visible, props.names.size(), props.totalCount, props.selectedIndex);

    if (!props.visible) {
        hide();
        return;
    }

    // Store current props for access in callbacks
    currentProps_ = props;

    // Use totalCount if available (windowed loading), else fall back to names.size()
    int totalCount = props.totalCount > 0 ? props.totalCount : static_cast<int>(props.names.size());

    // Update list (even if empty - data may arrive later)
    bool countChanged = list_->setTotalCount(totalCount);
    list_->setSelectedIndex(props.selectedIndex);
    if (!countChanged) {
        list_->invalidate();
    }

    // Show overlay and list
    if (!visible_) show();
}

// ══════════════════════════════════════════════════════════════════
// IComponent
// ══════════════════════════════════════════════════════════════════

void RemoteControlsPageSelector::show() {
    if (overlay_) lv_obj_clear_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    if (list_) list_->show();
    visible_ = true;
}

void RemoteControlsPageSelector::hide() {
    if (overlay_) lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
    if (list_) list_->hide();
    visible_ = false;
}

// ══════════════════════════════════════════════════════════════════
// Overlay Structure
// ══════════════════════════════════════════════════════════════════

void RemoteControlsPageSelector::createOverlay() {
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

void RemoteControlsPageSelector::createHeader() {
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
    lv_label_set_text(header_label_, "Pages");
    if (bitwig_fonts.device_label) {
        lv_obj_set_style_text_font(header_label_, bitwig_fonts.device_label, LV_STATE_DEFAULT);
    }
    style::apply(header_label_).textColor(Color::TEXT_LIGHT);
}

// ══════════════════════════════════════════════════════════════════
// VirtualList Callbacks
// ══════════════════════════════════════════════════════════════════

void RemoteControlsPageSelector::bindSlot(VirtualSlot& slot, int index, bool isSelected) {
    int slotIndex = index - list_->getWindowStart();
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    // Ensure widgets exist for this slot
    ensureSlotWidgets(slot, slotIndex);

    auto& widgets = slotWidgets_[slotIndex];

    // Get page name for this index
    const auto& props = currentProps_;
    const char* name = index < static_cast<int>(props.names.size())
                           ? props.names[index].c_str()
                           : "";

    // Update label
    if (widgets.label) {
        lv_label_set_text(widgets.label, name);
    }

    // Update index label (1-based display)
    if (widgets.indexLabel) {
        char indexStr[12];  // Enough for int32 + null
        snprintf(indexStr, sizeof(indexStr), "%d", index + 1);
        lv_label_set_text(widgets.indexLabel, indexStr);
    }

    // Apply highlight
    applyHighlightStyle(widgets, isSelected);
}

void RemoteControlsPageSelector::updateSlotHighlight(VirtualSlot& slot, bool isSelected) {
    int slotIndex = slot.boundIndex - list_->getWindowStart();
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    auto& widgets = slotWidgets_[slotIndex];
    applyHighlightStyle(widgets, isSelected);
}

void RemoteControlsPageSelector::ensureSlotWidgets(VirtualSlot& slot, int slotIndex) {
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    auto& widgets = slotWidgets_[slotIndex];
    if (widgets.created) return;

    lv_obj_t* container = slot.container;

    // Set up container as horizontal flex
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_left(container, Layout::OVERLAY_PAD_H, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(container, Layout::OVERLAY_PAD_H, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(container, 8, LV_STATE_DEFAULT);

    // Index label (right-aligned, fixed width)
    widgets.indexLabel = lv_label_create(container);
    lv_obj_set_width(widgets.indexLabel, 24);
    lv_obj_set_style_text_align(widgets.indexLabel, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
    if (bitwig_fonts.device_label) {
        lv_obj_set_style_text_font(widgets.indexLabel, bitwig_fonts.device_label, LV_STATE_DEFAULT);
    }
    style::apply(widgets.indexLabel).textColor(Color::DATA_INACTIVE);

    // Page name label (flex grow to fill remaining space)
    widgets.label = lv_label_create(container);
    lv_obj_set_flex_grow(widgets.label, 1);
    lv_label_set_long_mode(widgets.label, LV_LABEL_LONG_DOT);
    if (bitwig_fonts.device_label) {
        lv_obj_set_style_text_font(widgets.label, bitwig_fonts.device_label, LV_STATE_DEFAULT);
    }

    widgets.created = true;
}

void RemoteControlsPageSelector::applyHighlightStyle(PageSlotWidgets& widgets, bool isSelected) {
    if (widgets.label) {
        style::apply(widgets.label).textColor(isSelected ? Color::TEXT_LIGHT : Color::TEXT_DARK);
    }
    if (widgets.indexLabel) {
        style::apply(widgets.indexLabel).textColor(isSelected ? Color::DATA_ACTIVE : Color::DATA_INACTIVE);
    }
}

}  // namespace bitwig::ui
