#include "DeviceSelector.hpp"
#include "ui/device/DeviceTypeHelper.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/icon.hpp"
#include "ui/font/FontLoader.hpp"

using namespace Theme;

namespace Bitwig
{

DeviceSelector::DeviceSelector(lv_obj_t *parent)
    : BaseSelector(parent)
{
    setTitle("Devices");
    createFooter();  // Create footer once, after list - ensures correct flex order
}

DeviceSelector::~DeviceSelector()
{
    footer_.reset();
}

void DeviceSelector::render(const DeviceSelectorProps &props)
{
    if (!props.visible)
    {
        hide();
        if (footer_)
            footer_->hide();
        return;
    }

    if (props.showingChildren)
    {
        renderChildren(props);
    }
    else
    {
        renderDeviceList(props);
    }
}

void DeviceSelector::renderDeviceList(const DeviceSelectorProps &props)
{
    if (!props.names)
        return;

    const auto &names = *props.names;

    // Check if items changed
    bool itemsChanged = (prev_items_ != names);

    if (itemsChanged)
    {
        clearIndicators();
        prev_items_ = names;
    }

    overlay().setItems(names);
    overlay().setSelectedIndex(props.selectedIndex);

    if (itemsChanged)
    {
        createIndicators(props);
    }
    else
    {
        // Update states for existing indicators (names didn't change but states might have)
        updateIndicatorStates(props);
    }

    if (!isVisible())
        show();

    if (props.showFooter)
    {
        renderFooter(props);
    }
    else if (footer_)
    {
        footer_->hide();
    }
}

void DeviceSelector::renderChildren(const DeviceSelectorProps &props)
{
    if (!props.childrenNames)
        return;

    const auto &names = *props.childrenNames;

    // Check if items changed
    bool itemsChanged = (prev_items_ != names);

    if (itemsChanged)
    {
        clearIndicators();
        prev_items_ = names;
    }

    overlay().setItems(names);

    if (itemsChanged)
    {
        // Apply icon font to Back button (index 0)
        if (!names.empty() && names[0] == Icon::ARROW_LEFT)
        {
            lv_obj_t *backBtn = overlay().getButton(0);
            if (backBtn)
            {
                uint32_t childCount = lv_obj_get_child_cnt(backBtn);
                for (uint32_t j = 0; j < childCount; j++)
                {
                    lv_obj_t *child = lv_obj_get_child(backBtn, j);
                    if (child && lv_obj_check_type(child, &lv_label_class))
                    {
                        if (bitwig_fonts.icons_14)
                            lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
                        break;
                    }
                }
            }
        }

        // Add type icons for children items
        if (props.childrenTypes)
        {
            const auto &types = *props.childrenTypes;
            for (size_t i = 0; i < names.size(); i++)
            {
                if (i == 0 || i >= types.size())
                    continue;

                lv_obj_t *btn = overlay().getButton(i);
                if (!btn)
                    continue;

                const char *iconSymbol = nullptr;
                switch (types[i])
                {
                case 0: iconSymbol = Icon::SLIDER; break;
                case 1: iconSymbol = Icon::LAYER; break;
                case 2: iconSymbol = Icon::DRUM_PAD_MATRIX; break;
                }

                if (iconSymbol)
                {
                    lv_obj_t *icon_label = lv_label_create(btn);
                    Icon::set(icon_label, iconSymbol);
                    lv_obj_set_style_text_color(icon_label, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(icon_label, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_FOCUSED);
                    lv_obj_set_style_text_color(icon_label, lv_color_white(), LV_STATE_PRESSED);
                    lv_obj_set_style_text_color(icon_label, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DISABLED);
                    lv_obj_set_style_text_opa(icon_label, LV_OPA_50, LV_STATE_DISABLED);
                    lv_obj_move_to_index(icon_label, 0);
                }
            }
        }
    }

    overlay().setSelectedIndex(props.selectedIndex);

    if (!isVisible())
        show();

    if (footer_)
        footer_->hide();
}

void DeviceSelector::updateDeviceState(int displayIndex, bool enabled)
{
    size_t index = static_cast<size_t>(displayIndex);
    if (index >= state_icons_.size() || !state_icons_[index])
        return;

    // Update state icon in the list
    lv_obj_t *stateIcon = state_icons_[index];
    lv_label_set_text(stateIcon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
    lv_obj_set_style_text_color(stateIcon,
        lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);

    // Update footer if this is the selected device
    if (footer_state_ && displayIndex == overlay().getSelectedIndex())
    {
        Icon::set(footer_state_, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF, Icon::L);
        lv_obj_set_style_text_color(footer_state_,
            lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
    }
}

void DeviceSelector::updateIndicatorStates(const DeviceSelectorProps &props)
{
    if (!props.deviceStates)
        return;

    const auto &states = *props.deviceStates;
    for (size_t i = 0; i < state_icons_.size() && i < states.size(); i++)
    {
        if (!state_icons_[i])
            continue;

        bool enabled = states[i];
        lv_label_set_text(state_icons_[i], enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
        lv_obj_set_style_text_color(state_icons_[i],
            lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
    }
}

void DeviceSelector::createFooter()
{
    // Create footer inside overlay container (participates in flex layout)
    footer_ = std::make_unique<UI::HintBar>(overlay().getContainer(), UI::HintBarPosition::Bottom);
    footer_->setSize(Layout::HINT_BAR_HEIGHT);

    // Create track icon (cell 0 = left)
    footer_track_ = lv_label_create(footer_->getElement());
    Icon::set(footer_track_, Icon::CHANNEL_LIST, Icon::L);
    lv_obj_set_style_text_color(footer_track_, lv_color_hex(Color::TEXT_LIGHT), 0);
    footer_->setCell(0, footer_track_);

    // Create state icon (cell 1 = center) - will be updated in renderFooter
    footer_state_ = lv_label_create(footer_->getElement());
    Icon::set(footer_state_, Icon::DEVICE_ON, Icon::L);
    lv_obj_set_style_text_color(footer_state_, lv_color_hex(Color::DEVICE_STATE_ENABLED), 0);
    footer_->setCell(1, footer_state_);
}

void DeviceSelector::renderFooter(const DeviceSelectorProps &props)
{
    if (!footer_)
        return;

    int idx = props.selectedIndex;
    bool isEnabled = props.deviceStates && idx >= 0 && idx < static_cast<int>(props.deviceStates->size())
                         ? (*props.deviceStates)[idx]
                         : true;

    // Update icon and color based on state
    if (footer_state_)
    {
        Icon::set(footer_state_, isEnabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF, Icon::L);
        lv_obj_set_style_text_color(footer_state_,
            lv_color_hex(isEnabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
    }

    footer_->show();
}

void DeviceSelector::clearIndicators()
{
    for (auto *icon : type_icons_)
    {
        if (icon)
            lv_obj_delete(icon);
    }
    type_icons_.clear();

    for (auto *icon : state_icons_)
    {
        if (icon)
            lv_obj_delete(icon);
    }
    state_icons_.clear();

    for (auto *icon : folder_icons_)
    {
        if (icon)
            lv_obj_delete(icon);
    }
    folder_icons_.clear();
}

bool DeviceSelector::isNonDeviceItem(const std::string &name)
{
    return name == Icon::ARROW_LEFT || (!name.empty() && name[0] == '[');
}

bool DeviceSelector::hasChildren(const DeviceSelectorProps &props, size_t index)
{
    bool hasSlots = props.hasSlots && index < props.hasSlots->size() && (*props.hasSlots)[index];
    bool hasLayers = props.hasLayers && index < props.hasLayers->size() && (*props.hasLayers)[index];
    bool hasDrums = props.hasDrums && index < props.hasDrums->size() && (*props.hasDrums)[index];
    return hasSlots || hasLayers || hasDrums;
}

lv_obj_t *DeviceSelector::createDeviceTypeIcon(lv_obj_t *parent, uint8_t deviceType)
{
    auto info = DeviceType::get(deviceType);
    if (!info.visible)
        return nullptr;

    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, info.icon);
    lv_obj_set_style_text_color(icon, lv_color_hex(info.color), 0);
    return icon;
}

lv_obj_t *DeviceSelector::createDeviceStateIcon(lv_obj_t *parent, bool enabled)
{
    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
    lv_obj_set_style_text_color(icon,
        lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
    return icon;
}

lv_obj_t *DeviceSelector::createFolderIcon(lv_obj_t *parent)
{
    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, Icon::DIRECTORY);
    lv_obj_set_style_text_color(icon, lv_color_hex(Color::INACTIVE_LIGHTER), 0);
    lv_obj_set_style_text_opa(icon, LV_OPA_70, 0);
    return icon;
}

void DeviceSelector::createIndicators(const DeviceSelectorProps &props)
{
    if (!props.names)
        return;

    const auto &names = *props.names;
    size_t count = names.size();

    type_icons_.clear();
    type_icons_.resize(count, nullptr);
    state_icons_.clear();
    state_icons_.resize(count, nullptr);
    folder_icons_.clear();
    folder_icons_.resize(count, nullptr);

    for (size_t i = 0; i < count; i++)
    {
        lv_obj_t *button = overlay().getButton(i);
        if (!button)
            continue;

        bool isBackItem = (i == 0) && (names[i] == Icon::ARROW_LEFT);

        if (isBackItem)
        {
            uint32_t childCount = lv_obj_get_child_cnt(button);
            for (uint32_t j = 0; j < childCount; j++)
            {
                lv_obj_t *child = lv_obj_get_child(button, j);
                if (child && lv_obj_check_type(child, &lv_label_class))
                {
                    if (bitwig_fonts.icons_14)
                        lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
                    break;
                }
            }
            continue;
        }

        bool isDevice = !isNonDeviceItem(names[i]);

        if (isDevice)
        {
            // Create type icon first (index 0)
            uint8_t deviceType = (props.deviceTypes && i < props.deviceTypes->size())
                ? (*props.deviceTypes)[i] : 0;
            lv_obj_t *typeIcon = createDeviceTypeIcon(button, deviceType);
            if (typeIcon)
            {
                lv_obj_move_to_index(typeIcon, 0);
                type_icons_[i] = typeIcon;
            }

            // Create state icon (index 1)
            bool isEnabled = props.deviceStates && i < props.deviceStates->size() && (*props.deviceStates)[i];
            lv_obj_t *stateIcon = createDeviceStateIcon(button, isEnabled);
            lv_obj_move_to_index(stateIcon, typeIcon ? 1 : 0);
            state_icons_[i] = stateIcon;
        }

        // Create folder icon (index 2 or less if no type/state)
        bool isFolder = hasChildren(props, i);
        lv_obj_t *folderIcon = createFolderIcon(button);
        int folderIndex = (type_icons_[i] ? 1 : 0) + (state_icons_[i] ? 1 : 0);
        lv_obj_move_to_index(folderIcon, folderIndex);
        folder_icons_[i] = folderIcon;

        if (!isFolder)
            lv_obj_add_flag(folderIcon, LV_OBJ_FLAG_HIDDEN);
    }
}

} // namespace Bitwig
