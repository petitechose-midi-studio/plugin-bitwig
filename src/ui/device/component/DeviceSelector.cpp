#include "DeviceSelector.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/device/DeviceTypeHelper.hpp"
#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;
namespace style = oc::ui::lvgl::style;

namespace bitwig {

DeviceSelector::DeviceSelector(lv_obj_t *parent) : BaseSelector(parent) {
    createHeader();  // Create header first, before list
    createFooter();  // Create footer once, after list - ensures correct flex order
}

DeviceSelector::~DeviceSelector() {
    track_header_.reset();
    footer_.reset();
}

void DeviceSelector::render(const DeviceSelectorProps &props) {
    if (!props.visible) {
        hide();
        if (footer_) footer_->hide();
        return;
    }

    if (props.showingChildren) {
        renderChildren(props);
    } else {
        renderDeviceList(props);
    }
}

void DeviceSelector::renderDeviceList(const DeviceSelectorProps &props) {
    if (props.names.empty()) return;

    const auto &names = props.names;

    // Check if items changed
    bool items_changed = (prev_items_ != names);

    if (items_changed) {
        clearIndicators();
        prev_items_ = names;
    }

    overlay().setItems(names);
    overlay().setSelectedIndex(props.selectedIndex);

    if (items_changed) {
        createIndicators(props);
    } else {
        // Update states for existing indicators (names didn't change but states might have)
        updateIndicatorStates(props);
    }

    renderHeader(props);

    if (!isVisible()) show();

    if (props.showFooter) {
        renderFooter(props);
    } else if (footer_) {
        footer_->hide();
    }
}

void DeviceSelector::renderChildren(const DeviceSelectorProps &props) {
    if (props.childrenNames.empty()) return;

    const auto &names = props.childrenNames;

    // Check if items changed
    bool items_changed = (prev_items_ != names);

    if (items_changed) {
        clearIndicators();
        prev_items_ = names;
    }

    overlay().setItems(names);

    if (items_changed) {
        // Apply icon font to Back button (index 0)
        if (!names.empty() && names[0] == Icon::ARROW_LEFT) {
            overlay().setItemFont(0, bitwig_fonts.icons_14);
        }

        // Add type icons for children items
        if (!props.childrenTypes.empty()) {
            const auto &types = props.childrenTypes;
            for (size_t i = 0; i < names.size(); i++) {
                if (i == 0 || i >= types.size()) continue;

                lv_obj_t *btn = overlay().getButton(i);
                if (!btn) continue;

                const char *icon_symbol = nullptr;
                switch (types[i]) {
                    case 0: icon_symbol = Icon::SLIDER; break;
                    case 1: icon_symbol = Icon::LAYER; break;
                    case 2: icon_symbol = Icon::DRUM_PAD_MATRIX; break;
                }

                if (icon_symbol) {
                    lv_obj_t *icon_lbl = lv_label_create(btn);
                    Icon::set(icon_lbl, icon_symbol);
                    lv_obj_set_style_text_color(icon_lbl, lv_color_hex(Color::INACTIVE_LIGHTER),
                                                LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(icon_lbl, lv_color_hex(Color::TEXT_PRIMARY),
                                                LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(icon_lbl, lv_color_hex(Color::TEXT_PRIMARY),
                                                LV_STATE_PRESSED);
                    lv_obj_set_style_text_color(icon_lbl, lv_color_hex(Color::INACTIVE_LIGHTER),
                                                LV_STATE_DISABLED);
                    lv_obj_set_style_text_opa(icon_lbl, LV_OPA_50, LV_STATE_DISABLED);
                    lv_obj_move_to_index(icon_lbl, 0);
                }
            }
        }
    }

    overlay().setSelectedIndex(props.selectedIndex);

    renderHeader(props);

    if (!isVisible()) show();

    if (footer_) footer_->hide();
}

void DeviceSelector::updateDeviceState(int displayIndex, bool enabled) {
    size_t index = static_cast<size_t>(displayIndex);
    if (index >= state_icons_.size() || !state_icons_[index]) return;

    // Update state icon in the list
    lv_obj_t *state_icon = state_icons_[index];
    lv_label_set_text(state_icon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
    style::apply(state_icon).textColor(enabled ? Color::DEVICE_STATE_ENABLED
                                               : Color::DEVICE_STATE_DISABLED);

    // Update footer if this is the selected device
    if (footer_state_ && displayIndex == overlay().getSelectedIndex()) {
        Icon::set(footer_state_, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF, Icon::Size::L);
        style::apply(footer_state_).textColor(enabled ? Color::DEVICE_STATE_ENABLED
                                                      : Color::DEVICE_STATE_DISABLED);
    }
}

void DeviceSelector::updateIndicatorStates(const DeviceSelectorProps &props) {
    if (props.deviceStates.empty()) return;

    const auto &states = props.deviceStates;
    for (size_t i = 0; i < state_icons_.size() && i < states.size(); i++) {
        if (!state_icons_[i]) continue;

        bool enabled = states[i];
        lv_label_set_text(state_icons_[i], enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
        style::apply(state_icons_[i]).textColor(enabled ? Color::DEVICE_STATE_ENABLED
                                                        : Color::DEVICE_STATE_DISABLED);
    }
}

void DeviceSelector::createHeader() {
    // Create header container inside overlay container (participates in flex layout)
    header_ = lv_obj_create(overlay().getContainer());
    lv_obj_set_size(header_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(header_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(header_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(header_, 16, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(header_, 16, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(header_, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(header_, 4, LV_STATE_DEFAULT);
    lv_obj_clear_flag(header_, LV_OBJ_FLAG_SCROLLABLE);

    // Flex row for track title items
    lv_obj_set_flex_flow(header_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(header_, 8, LV_STATE_DEFAULT);

    // Move header to position 1 (after hidden title, before list)
    lv_obj_move_to_index(header_, 1);

    // Create TrackTitleItem
    track_header_ = std::make_unique<TrackTitleItem>(header_, false, 16);
}

void DeviceSelector::renderHeader(const DeviceSelectorProps &props) {
    if (!track_header_) return;

    track_header_->render({.name = props.trackName ? props.trackName : "",
                           .color = props.trackColor,
                           .trackType = props.trackType,
                           .highlighted = true,
                           .hideIndicators = false});
}

void DeviceSelector::createFooter() {
    // Create footer inside overlay container (participates in flex layout)
    footer_ = std::make_unique<HintBar>(overlay().getContainer(), HintBarPosition::BOTTOM);
    footer_->setSize(Layout::HINT_BAR_HEIGHT);

    // Create track icon (cell 0 = left)
    footer_track_ = lv_label_create(footer_->getElement());
    Icon::set(footer_track_, Icon::CHANNEL_LIST, Icon::Size::L);
    style::apply(footer_track_).textColor(Color::TEXT_LIGHT);
    footer_->setCell(0, footer_track_);

    // Create state icon (cell 1 = center) - will be updated in renderFooter
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

    // Update icon and color based on state
    if (footer_state_) {
        Icon::set(footer_state_, is_enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF, Icon::Size::L);
        style::apply(footer_state_).textColor(is_enabled ? Color::DEVICE_STATE_ENABLED
                                                         : Color::DEVICE_STATE_DISABLED);
    }

    footer_->show();
}

void DeviceSelector::clearIndicators() {
    for (auto *icon : type_icons_) {
        if (icon) lv_obj_delete(icon);
    }
    type_icons_.clear();

    for (auto *icon : state_icons_) {
        if (icon) lv_obj_delete(icon);
    }
    state_icons_.clear();

    for (auto *icon : folder_icons_) {
        if (icon) lv_obj_delete(icon);
    }
    folder_icons_.clear();
}

bool DeviceSelector::isNonDeviceItem(const std::string &name) {
    return name == Icon::ARROW_LEFT || (!name.empty() && name[0] == '[');
}

bool DeviceSelector::hasChildren(const DeviceSelectorProps &props, size_t index) {
    bool has_slots = index < props.hasSlots.size() && props.hasSlots[index];
    bool has_layers = index < props.hasLayers.size() && props.hasLayers[index];
    bool has_drums = index < props.hasDrums.size() && props.hasDrums[index];
    return has_slots || has_layers || has_drums;
}

lv_obj_t *DeviceSelector::createDeviceTypeIcon(lv_obj_t *parent, uint8_t deviceType) {
    auto info = DeviceType::get(deviceType);
    if (!info.visible) return nullptr;

    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, info.icon);
    style::apply(icon).textColor(info.color);
    return icon;
}

lv_obj_t *DeviceSelector::createDeviceStateIcon(lv_obj_t *parent, bool enabled) {
    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
    style::apply(icon).textColor(enabled ? Color::DEVICE_STATE_ENABLED
                                         : Color::DEVICE_STATE_DISABLED);
    return icon;
}

lv_obj_t *DeviceSelector::createFolderIcon(lv_obj_t *parent) {
    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, Icon::DIRECTORY);
    style::apply(icon).textColor(Color::INACTIVE_LIGHTER);
    lv_obj_set_style_text_opa(icon, LV_OPA_70, LV_STATE_DEFAULT);
    return icon;
}

void DeviceSelector::createIndicators(const DeviceSelectorProps &props) {
    if (props.names.empty()) return;

    const auto &names = props.names;
    size_t count = names.size();

    type_icons_.clear();
    type_icons_.resize(count, nullptr);
    state_icons_.clear();
    state_icons_.resize(count, nullptr);
    folder_icons_.clear();
    folder_icons_.resize(count, nullptr);

    for (size_t i = 0; i < count; i++) {
        lv_obj_t *button = overlay().getButton(i);
        if (!button) continue;

        bool is_back_item = (i == 0) && (names[i] == Icon::ARROW_LEFT);

        if (is_back_item) {
            overlay().setItemFont(i, bitwig_fonts.icons_14);
            continue;
        }

        bool is_device = !isNonDeviceItem(names[i]);

        if (is_device) {
            // Create type icon first (index 0)
            uint8_t device_type = i < props.deviceTypes.size() ? props.deviceTypes[i] : 0;
            lv_obj_t *type_icon = createDeviceTypeIcon(button, device_type);
            if (type_icon) {
                lv_obj_move_to_index(type_icon, 0);
                type_icons_[i] = type_icon;
            }

            // Create state icon (index 1)
            bool is_enabled = i < props.deviceStates.size() && props.deviceStates[i];
            lv_obj_t *state_icon = createDeviceStateIcon(button, is_enabled);
            lv_obj_move_to_index(state_icon, type_icon ? 1 : 0);
            state_icons_[i] = state_icon;
        }

        // Create folder icon (index 2 or less if no type/state)
        bool is_folder = hasChildren(props, i);
        lv_obj_t *folder_icon = createFolderIcon(button);
        int folder_index = (type_icons_[i] ? 1 : 0) + (state_icons_[i] ? 1 : 0);
        lv_obj_move_to_index(folder_icon, folder_index);
        folder_icons_[i] = folder_icon;

        if (!is_folder) lv_obj_add_flag(folder_icon, LV_OBJ_FLAG_HIDDEN);
    }
}

}  // namespace bitwig
