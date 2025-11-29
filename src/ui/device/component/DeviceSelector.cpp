#include "DeviceSelector.hpp"
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
}

DeviceSelector::~DeviceSelector()
{
    footer_.reset();
}

void DeviceSelector::setItems(const std::vector<std::string> &items)
{
    items_ = items;
    device_states_.clear();
    has_slots_.clear();
    has_layers_.clear();
    has_drums_.clear();

    overlay().setItems(items);
    clearIndicators();
}

void DeviceSelector::setDeviceItems(const std::vector<std::string> &names,
                                    int currentIndex,
                                    const std::vector<bool> &deviceStates,
                                    const std::vector<bool> &hasSlots,
                                    const std::vector<bool> &hasLayers,
                                    const std::vector<bool> &hasDrums)
{
    items_ = names;
    current_item_index_ = currentIndex;

    size_t itemCount = names.size();
    device_states_.assign(deviceStates.begin(), deviceStates.end());
    has_slots_.assign(hasSlots.begin(), hasSlots.end());
    has_layers_.assign(hasLayers.begin(), hasLayers.end());
    has_drums_.assign(hasDrums.begin(), hasDrums.end());

    device_states_.resize(itemCount, false);
    has_slots_.resize(itemCount, false);
    has_layers_.resize(itemCount, false);
    has_drums_.resize(itemCount, false);

    overlay().setItems(names);
    overlay().setSelectedIndex(currentIndex);

    clearIndicators();
    createIndicators();
}

void DeviceSelector::setChildrenItems(const std::vector<std::string> &items,
                                      const std::vector<uint8_t> &itemTypes)
{
    items_ = items;
    device_states_.clear();
    has_slots_.clear();
    has_layers_.clear();
    has_drums_.clear();

    overlay().setItems(items);

    // Apply icon font to Back button (index 0)
    if (!items.empty() && items[0] == Icon::ARROW_LEFT)
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
    for (size_t i = 0; i < items.size(); i++)
    {
        if (i == 0 || i >= itemTypes.size())
            continue;

        lv_obj_t *btn = overlay().getButton(i);
        if (!btn)
            continue;

        const char *iconSymbol = nullptr;
        switch (itemTypes[i])
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

    clearIndicators();
}

void DeviceSelector::setCurrentItemIndex(int index)
{
    current_item_index_ = index;
    overlay().setSelectedIndex(index);
}

void DeviceSelector::setDeviceStateAtIndex(int displayIndex, bool enabled)
{
    if (displayIndex < 0 || displayIndex >= static_cast<int>(device_states_.size()))
        return;

    device_states_[displayIndex] = enabled;

    lv_obj_t *button = overlay().getButton(displayIndex);
    if (!button || isNonDeviceItem(displayIndex))
        return;

    uint32_t childCount = lv_obj_get_child_cnt(button);
    if (childCount > 0)
    {
        lv_obj_t *stateIcon = lv_obj_get_child(button, 0);
        if (stateIcon && lv_obj_check_type(stateIcon, &lv_label_class))
        {
            lv_label_set_text(stateIcon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
            lv_obj_set_style_text_color(stateIcon,
                lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
        }
    }
}

void DeviceSelector::show()
{
    overlay().setSelectedIndex(current_item_index_);
    BaseSelector::show();
}

void DeviceSelector::showWithFooter()
{
    overlay().setSelectedIndex(current_item_index_);
    BaseSelector::show();

    if (!footer_)
        createFooter();
    footer_->show();
}

void DeviceSelector::showWithoutFooter()
{
    overlay().setSelectedIndex(current_item_index_);
    BaseSelector::show();

    if (footer_)
        footer_->hide();
}

void DeviceSelector::hide()
{
    BaseSelector::hide();
    if (footer_)
        footer_->hide();
}

void DeviceSelector::createFooter()
{
    footer_ = std::make_unique<ButtonHintBar>(parent_);
    footer_->setLeftLabel("Track");
    footer_->setCenterLabel("State");
}

void DeviceSelector::clearIndicators()
{
    folder_icons_.clear();
}

bool DeviceSelector::isNonDeviceItem(size_t index) const
{
    if (index >= items_.size())
        return false;
    const std::string &name = items_[index];
    return name == Icon::ARROW_LEFT || (!name.empty() && name[0] == '[');
}

bool DeviceSelector::hasChildren(size_t index) const
{
    return index < has_slots_.size() &&
           (has_slots_[index] || has_layers_[index] || has_drums_[index]);
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

void DeviceSelector::createIndicators()
{
    folder_icons_.clear();
    folder_icons_.resize(items_.size(), nullptr);

    for (size_t i = 0; i < items_.size(); i++)
    {
        lv_obj_t *button = overlay().getButton(i);
        if (!button)
            continue;

        bool isBackItem = (i == 0) && (items_[i] == Icon::ARROW_LEFT);

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

        bool isDevice = !isNonDeviceItem(i);

        if (isDevice)
        {
            bool isEnabled = (i < device_states_.size()) && device_states_[i];
            lv_obj_t *stateIcon = createDeviceStateIcon(button, isEnabled);
            lv_obj_move_to_index(stateIcon, 0);
        }

        bool isFolder = hasChildren(i);
        lv_obj_t *folderIcon = createFolderIcon(button);
        lv_obj_move_to_index(folderIcon, 1);
        folder_icons_[i] = folderIcon;

        if (!isFolder)
            lv_obj_add_flag(folderIcon, LV_OBJ_FLAG_HIDDEN);
    }
}

} // namespace Bitwig
