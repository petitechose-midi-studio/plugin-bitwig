#include "DeviceSelector.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>
#include <oc/ui/lvgl/theme/BaseTheme.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include "ui/device/DeviceTypeHelper.hpp"
#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;
using namespace oc::ui::lvgl;
namespace style = oc::ui::lvgl::style;

namespace bitwig {

namespace {
constexpr int VISIBLE_SLOTS = 5;
constexpr int ITEM_HEIGHT = 32;
}  // namespace

// ══════════════════════════════════════════════════════════════════
// Construction
// ══════════════════════════════════════════════════════════════════

DeviceSelector::DeviceSelector(lv_obj_t *parent) : parent_(parent) {
    createOverlay();

    list_ = std::make_unique<VirtualList>(container_);
    list_->visibleCount(VISIBLE_SLOTS)
        .itemHeight(ITEM_HEIGHT)
        .scrollMode(ScrollMode::CenterLocked)
        .onBindSlot([this](VirtualSlot &slot, int index, bool isSelected) {
            bindSlot(slot, index, isSelected);
        })
        .onUpdateHighlight([this](VirtualSlot &slot, bool isSelected) {
            updateSlotHighlight(slot, isSelected);
        });

    slotWidgets_.resize(VISIBLE_SLOTS);

    // Empty state label (floating, centered in parent, hidden by default)
    emptyLabel_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    emptyLabel_->alignment(LV_TEXT_ALIGN_CENTER)
        .font(bitwig_fonts.device_label)
        .color(Color::DATA_INACTIVE);
    emptyLabel_->setText("No Devices");
    lv_obj_add_flag(emptyLabel_->getElement(), LV_OBJ_FLAG_FLOATING);
    lv_obj_add_flag(emptyLabel_->getElement(), LV_OBJ_FLAG_HIDDEN);
    lv_obj_center(emptyLabel_->getElement());

    createHeader();
    createFooter();

    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
}

DeviceSelector::~DeviceSelector() {
    track_header_.reset();
    page_label_.reset();
    footer_.reset();
    emptyLabel_.reset();
    list_.reset();
}

// ══════════════════════════════════════════════════════════════════
// Public API
// ══════════════════════════════════════════════════════════════════

void DeviceSelector::render(const DeviceSelectorProps &props) {
    if (!props.visible) {
        hide();
        if (footer_) footer_->hide();
        return;
    }

    currentProps_ = props;
    showingChildren_ = props.showingChildren;

    if (props.showingChildren) {
        renderChildren(props);
    } else {
        renderDeviceList(props);
    }
}

void DeviceSelector::updateDeviceState(int displayIndex, bool enabled) {
    if (displayIndex >= 0 && displayIndex < static_cast<int>(currentProps_.deviceStates.size())) {
        currentProps_.deviceStates[displayIndex] = enabled;
    }

    list_->invalidateIndex(displayIndex);

    if (footer_state_ && displayIndex == currentProps_.selectedIndex) {
        Icon::set(footer_state_, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF, Icon::Size::L);
        style::apply(footer_state_).textColor(enabled ? Color::DEVICE_STATE_ENABLED
                                                       : Color::DEVICE_STATE_DISABLED);
    }
}

void DeviceSelector::show() {
    if (overlay_) {
        lv_obj_clear_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
        visible_ = true;
        list_->show();
    }
}

void DeviceSelector::hide() {
    if (overlay_) {
        lv_obj_add_flag(overlay_, LV_OBJ_FLAG_HIDDEN);
        visible_ = false;
        list_->hide();
    }
}

// ══════════════════════════════════════════════════════════════════
// Render Modes
// ══════════════════════════════════════════════════════════════════

void DeviceSelector::renderDeviceList(const DeviceSelectorProps &props) {
    renderHeader(props);

    // Loading state: hide both list and empty label, wait for host response
    if (props.loading) {
        list_->hide();
        if (emptyLabel_) {
            lv_obj_add_flag(emptyLabel_->getElement(), LV_OBJ_FLAG_HIDDEN);
        }
        if (footer_) footer_->hide();
        if (!visible_) show();
        return;
    }

    if (props.names.empty()) {
        // Empty state: show "No Device" label, hide list
        list_->hide();
        if (emptyLabel_) {
            lv_obj_clear_flag(emptyLabel_->getElement(), LV_OBJ_FLAG_HIDDEN);
        }
        if (footer_) footer_->hide();
        if (!visible_) show();
        return;
    }

    // Normal state: hide empty label, show list
    if (emptyLabel_) {
        lv_obj_add_flag(emptyLabel_->getElement(), LV_OBJ_FLAG_HIDDEN);
    }
    list_->show();

    list_->setTotalCount(static_cast<int>(props.names.size()));
    list_->setSelectedIndex(props.selectedIndex);
    list_->invalidate();  // Force rebind to pick up state changes

    if (!visible_) show();

    if (props.showFooter) {
        renderFooter(props);
    } else if (footer_) {
        footer_->hide();
    }
}

void DeviceSelector::renderChildren(const DeviceSelectorProps &props) {
    if (props.childrenNames.empty()) return;

    // Hide empty label when showing children
    if (emptyLabel_) {
        lv_obj_add_flag(emptyLabel_->getElement(), LV_OBJ_FLAG_HIDDEN);
    }
    list_->show();

    list_->setTotalCount(static_cast<int>(props.childrenNames.size()));
    list_->setSelectedIndex(props.selectedIndex);
    list_->invalidate();  // Force rebind to pick up any changes

    renderHeader(props);

    if (!visible_) show();

    if (footer_) footer_->hide();
}

// ══════════════════════════════════════════════════════════════════
// Overlay Structure
// ══════════════════════════════════════════════════════════════════

void DeviceSelector::createOverlay() {
    overlay_ = lv_obj_create(parent_);
    lv_obj_add_flag(overlay_, LV_OBJ_FLAG_FLOATING);
    style::apply(overlay_).fullSize().bgColor(BaseTheme::Color::BACKGROUND, Opacity::OVERLAY_BG).noScroll();
    lv_obj_align(overlay_, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(overlay_, 0, LV_STATE_DEFAULT);

    container_ = lv_obj_create(overlay_);
    style::apply(container_).fullSize().transparent().noScroll();
    lv_obj_align(container_, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(container_, BaseTheme::Layout::ROW_GAP_MD, LV_STATE_DEFAULT);
}

void DeviceSelector::createHeader() {
    // Fixed height header with grid layout - no padding (handled by cells)
    header_ = lv_obj_create(container_);
    lv_obj_set_size(header_, LV_PCT(100), Layout::OVERLAY_HEADER_HEIGHT);
    lv_obj_set_style_bg_opa(header_, Opacity::HIDDEN, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(header_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(header_, 0, LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(header_, LV_SCROLLBAR_MODE_OFF);

    // Grid: 2 columns (Title | Page indicator)
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_layout(header_, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(header_, col_dsc, row_dsc);

    lv_obj_move_to_index(header_, 0);

    // Left cell: Title container with TrackTitleItem (centered vertically)
    lv_obj_t* title_container = lv_obj_create(header_);
    lv_obj_set_grid_cell(title_container, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_style_pad_all(title_container, 0, LV_STATE_DEFAULT);
    style::apply(title_container).transparent().noScroll().flexRow(LV_FLEX_ALIGN_START, Layout::GAP_SM);
    track_header_ = std::make_unique<TrackTitleItem>(title_container, false, LV_PCT(100));

    // Right cell: Page indicator (with right padding)
    page_label_ = std::make_unique<oc::ui::lvgl::Label>(header_);
    page_label_->alignment(LV_TEXT_ALIGN_RIGHT)
               .gridCell(1, 1, 0, 1)
               .autoScroll(false)
               .color(Color::DATA_INACTIVE)
               .font(bitwig_fonts.device_label)
               .ownsLvglObjects(false);
    lv_obj_set_style_pad_right(page_label_->getElement(), Layout::OVERLAY_PAD_H, LV_STATE_DEFAULT);
    page_label_->setText("");
}

void DeviceSelector::renderHeader(const DeviceSelectorProps &props) {
    if (!track_header_) return;

    track_header_->render({.name = props.trackName ? props.trackName : "",
                           .color = props.trackColor,
                           .trackType = props.trackType,
                           .highlighted = true,
                           .hideIndicators = false});

    // Update total count indicator
    if (page_label_) {
        int total = props.totalCount;
        if (total > 0) {
            char buf[24];
            snprintf(buf, sizeof(buf), "%d items", total);
            page_label_->setText(buf);
        } else {
            page_label_->setText("");
        }
    }
}

void DeviceSelector::createFooter() {
    footer_ = std::make_unique<HintBar>(container_, HintBarPosition::BOTTOM);
    footer_->setSize(Layout::HINT_BAR_HEIGHT);

    footer_track_ = lv_label_create(footer_->getElement());
    Icon::set(footer_track_, Icon::CHANNEL_LIST, Icon::Size::L);
    style::apply(footer_track_).textColor(Color::TEXT_LIGHT);
    footer_->setCell(0, footer_track_);

    footer_state_ = lv_label_create(footer_->getElement());
    Icon::set(footer_state_, Icon::DEVICE_ON, Icon::Size::L);
    style::apply(footer_state_).textColor(Color::DEVICE_STATE_ENABLED);
    footer_->setCell(1, footer_state_);
}

void DeviceSelector::renderFooter(const DeviceSelectorProps &props) {
    if (!footer_) return;

    int idx = props.selectedIndex;
    bool is_enabled = idx >= 0 && idx < static_cast<int>(props.deviceStates.size())
                          ? props.deviceStates[idx]
                          : true;

    if (footer_state_) {
        Icon::set(footer_state_, is_enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF, Icon::Size::L);
        style::apply(footer_state_).textColor(is_enabled ? Color::DEVICE_STATE_ENABLED
                                                         : Color::DEVICE_STATE_DISABLED);
    }

    footer_->show();
}

// ══════════════════════════════════════════════════════════════════
// VirtualList Callbacks
// ══════════════════════════════════════════════════════════════════

void DeviceSelector::bindSlot(VirtualSlot &slot, int index, bool isSelected) {
    // slotIndex = position dans la fenêtre visible (0 à 5)
    int slotIndex = index - list_->getWindowStart();
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    ensureSlotWidgets(slot, slotIndex);

    auto &widgets = slotWidgets_[slotIndex];
    hideAllWidgets(widgets);

    if (showingChildren_) {
        populateSlotForChild(widgets, index);
    } else {
        populateSlotForDevice(widgets, index);
    }

    applyHighlightStyle(widgets, isSelected);
}

void DeviceSelector::updateSlotHighlight(VirtualSlot &slot, bool isSelected) {
    // Utilise boundIndex pour trouver le slotIndex
    int slotIndex = slot.boundIndex - list_->getWindowStart();
    if (slotIndex < 0 || slotIndex >= VISIBLE_SLOTS) return;

    auto &widgets = slotWidgets_[slotIndex];
    applyHighlightStyle(widgets, isSelected);
}

void DeviceSelector::ensureSlotWidgets(VirtualSlot &slot, int slotIndex) {
    auto &widgets = slotWidgets_[slotIndex];

    if (widgets.created) return;

    // Crée tous les widgets possibles (cachés par défaut)
    widgets.typeIcon = createIcon(slot.container);
    widgets.stateIcon = createIcon(slot.container);
    widgets.label = createLabel(slot.container);
    widgets.folderIcon = createIcon(slot.container);

    // Index label (fixed width, right-aligned, always last)
    widgets.indexLabel = lv_label_create(slot.container);
    lv_label_set_text(widgets.indexLabel, "");
    lv_obj_set_style_text_color(widgets.indexLabel, lv_color_hex(Color::DATA_INACTIVE), LV_STATE_DEFAULT);
    if (bitwig_fonts.device_label) {
        lv_obj_set_style_text_font(widgets.indexLabel, bitwig_fonts.device_label, LV_STATE_DEFAULT);
    }
    lv_obj_set_width(widgets.indexLabel, 24);
    lv_obj_set_style_text_align(widgets.indexLabel, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
    lv_obj_add_flag(widgets.indexLabel, LV_OBJ_FLAG_HIDDEN);

    widgets.created = true;
}

// ══════════════════════════════════════════════════════════════════
// Slot Population
// ══════════════════════════════════════════════════════════════════

void DeviceSelector::populateSlotForDevice(DeviceSlotWidgets &widgets, int index) {
    const auto &names = currentProps_.names;
    if (index < 0 || index >= static_cast<int>(names.size())) return;

    const std::string &name = names[index];
    bool isBack = (index == 0) && (name == Icon::UI_ARROW_LEFT);

    // Label (toujours visible) - will be moved to correct position below
    if (widgets.label) {
        lv_obj_clear_flag(widgets.label, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(widgets.label, name.c_str());
        lv_obj_set_style_text_font(widgets.label,
                                   isBack ? bitwig_fonts.icons_14 : bitwig_fonts.device_label,
                                   LV_STATE_DEFAULT);
    }

    if (isBack) return;

    bool isDevice = !isNonDeviceItem(name);
    bool hasFolder = hasChildren(currentProps_, index);

    // Order: type(0) → state(1) → folder(2) → label(3) → indexLabel(4)

    // Type icon (index 0)
    if (isDevice && widgets.typeIcon) {
        uint8_t deviceType = index < static_cast<int>(currentProps_.deviceTypes.size())
                                 ? currentProps_.deviceTypes[index]
                                 : 0;
        auto info = DeviceType::get(deviceType);
        if (info.visible) {
            Icon::set(widgets.typeIcon, info.icon);
            style::apply(widgets.typeIcon).textColor(info.color);
            lv_obj_clear_flag(widgets.typeIcon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_to_index(widgets.typeIcon, 0);
        }
    }

    // State icon (index 1)
    if (isDevice && widgets.stateIcon) {
        bool enabled = index < static_cast<int>(currentProps_.deviceStates.size())
                           ? currentProps_.deviceStates[index]
                           : true;
        Icon::set(widgets.stateIcon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
        style::apply(widgets.stateIcon).textColor(enabled ? Color::DEVICE_STATE_ENABLED
                                                           : Color::DEVICE_STATE_DISABLED);
        lv_obj_clear_flag(widgets.stateIcon, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_to_index(widgets.stateIcon, 1);
    }

    // Folder icon (index 2) - always reserve space for alignment
    if (widgets.folderIcon) {
        Icon::set(widgets.folderIcon, Icon::BROWSER_DIRECTORY);
        style::apply(widgets.folderIcon).textColor(Color::INACTIVE_LIGHTER);
        // Visible with opacity when has children, invisible (0 opacity) otherwise to reserve space
        lv_obj_set_style_text_opa(widgets.folderIcon, hasFolder ? Opacity::SUBTLE : Opacity::HIDDEN,
                                  LV_STATE_DEFAULT);
        lv_obj_clear_flag(widgets.folderIcon, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_to_index(widgets.folderIcon, 2);
    }

    // Label (index 3) - fixed position for alignment
    if (widgets.label) {
        lv_obj_move_to_index(widgets.label, 3);
    }

    // Index label (always last) - show 1-based device index
    if (widgets.indexLabel && isDevice) {
        // Calculate real device index (excluding back button)
        int deviceIndex = currentProps_.isNested ? index : index + 1;
        char buf[12];
        snprintf(buf, sizeof(buf), "%d", deviceIndex);
        lv_label_set_text(widgets.indexLabel, buf);
        lv_obj_clear_flag(widgets.indexLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_to_index(widgets.indexLabel, 4);
    }
}

void DeviceSelector::populateSlotForChild(DeviceSlotWidgets &widgets, int index) {
    const auto &names = currentProps_.childrenNames;
    if (index < 0 || index >= static_cast<int>(names.size())) return;

    const std::string &name = names[index];
    bool isBack = (index == 0) && (name == Icon::UI_ARROW_LEFT);

    // Label (toujours visible)
    if (widgets.label) {
        lv_obj_clear_flag(widgets.label, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(widgets.label, name.c_str());
        lv_obj_set_style_text_font(widgets.label,
                                   isBack ? bitwig_fonts.icons_14 : bitwig_fonts.device_label,
                                   LV_STATE_DEFAULT);
    }

    if (isBack) return;

    // Child type icon (position 0)
    bool hasTypeIcon = false;
    if (widgets.typeIcon && index < static_cast<int>(currentProps_.childrenTypes.size())) {
        uint8_t type = currentProps_.childrenTypes[index];
        const char *iconSymbol = nullptr;
        switch (type) {
            case 0: iconSymbol = Icon::UI_SLIDER; break;
            case 1: iconSymbol = Icon::BROWSER_LAYER; break;
            case 2: iconSymbol = Icon::DEVICE_DRUM_PAD; break;
        }
        if (iconSymbol) {
            Icon::set(widgets.typeIcon, iconSymbol);
            style::apply(widgets.typeIcon).textColor(Color::INACTIVE_LIGHTER);
            lv_obj_clear_flag(widgets.typeIcon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_to_index(widgets.typeIcon, 0);
            hasTypeIcon = true;
        }
    }

    // Label position
    int labelPosition = hasTypeIcon ? 1 : 0;
    if (widgets.label) {
        lv_obj_move_to_index(widgets.label, labelPosition);
    }

    // Index label (always last) - show 1-based child index
    if (widgets.indexLabel) {
        char buf[12];
        snprintf(buf, sizeof(buf), "%d", index);  // index 1 = child 1 (back button is 0)
        lv_label_set_text(widgets.indexLabel, buf);
        lv_obj_clear_flag(widgets.indexLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_to_index(widgets.indexLabel, labelPosition + 1);
    }
}

// ══════════════════════════════════════════════════════════════════
// Highlight
// ══════════════════════════════════════════════════════════════════

void DeviceSelector::applyHighlightStyle(DeviceSlotWidgets &widgets, bool isSelected) {
    uint32_t textColor = isSelected ? Color::TEXT_PRIMARY : Color::INACTIVE_LIGHTER;

    if (widgets.label) {
        lv_obj_set_style_text_color(widgets.label, lv_color_hex(textColor), LV_STATE_DEFAULT);
    }

    // Children type icons change color with selection
    if (showingChildren_ && widgets.typeIcon && !lv_obj_has_flag(widgets.typeIcon, LV_OBJ_FLAG_HIDDEN)) {
        lv_obj_set_style_text_color(widgets.typeIcon, lv_color_hex(textColor), LV_STATE_DEFAULT);
    }
}

void DeviceSelector::hideAllWidgets(DeviceSlotWidgets &widgets) {
    if (widgets.typeIcon) lv_obj_add_flag(widgets.typeIcon, LV_OBJ_FLAG_HIDDEN);
    if (widgets.stateIcon) lv_obj_add_flag(widgets.stateIcon, LV_OBJ_FLAG_HIDDEN);
    if (widgets.label) lv_obj_add_flag(widgets.label, LV_OBJ_FLAG_HIDDEN);
    if (widgets.folderIcon) lv_obj_add_flag(widgets.folderIcon, LV_OBJ_FLAG_HIDDEN);
    if (widgets.indexLabel) lv_obj_add_flag(widgets.indexLabel, LV_OBJ_FLAG_HIDDEN);
}

// ══════════════════════════════════════════════════════════════════
// Widget Creation
// ══════════════════════════════════════════════════════════════════

lv_obj_t *DeviceSelector::createIcon(lv_obj_t *parent) {
    lv_obj_t *icon = lv_label_create(parent);
    lv_obj_add_flag(icon, LV_OBJ_FLAG_HIDDEN);
    return icon;
}

lv_obj_t *DeviceSelector::createLabel(lv_obj_t *parent) {
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "");
    lv_obj_set_style_text_color(label, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DEFAULT);
    if (bitwig_fonts.device_label) {
        lv_obj_set_style_text_font(label, bitwig_fonts.device_label, LV_STATE_DEFAULT);
    }
    lv_obj_set_flex_grow(label, 1);
    return label;
}

// ══════════════════════════════════════════════════════════════════
// Static Helpers
// ══════════════════════════════════════════════════════════════════

bool DeviceSelector::isNonDeviceItem(const std::string &name) {
    return name == Icon::UI_ARROW_LEFT || (!name.empty() && name[0] == '[');
}

bool DeviceSelector::hasChildren(const DeviceSelectorProps &props, size_t index) {
    bool hasSlots = index < props.hasSlots.size() && props.hasSlots[index];
    bool hasLayers = index < props.hasLayers.size() && props.hasLayers[index];
    bool hasDrums = index < props.hasDrums.size() && props.hasDrums[index];
    return hasSlots || hasLayers || hasDrums;
}

}  // namespace bitwig
