#include "DeviceListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/icon.hpp"
#include <algorithm>

using namespace Theme;

namespace Bitwig
{

DeviceListOverlay::DeviceListOverlay(lv_obj_t *parent)
    : list_(parent)
{
}

DeviceListOverlay::~DeviceListOverlay()
{
    clearIndicators();
}

void DeviceListOverlay::setTitle(const std::string &title)
{
    list_.setTitle(title);
}

void DeviceListOverlay::setItems(const std::vector<std::string> &items)
{
    item_names_ = items;
    device_states_.clear();
    has_slots_.clear();
    has_layers_.clear();
    has_drums_.clear();
    current_device_index_ = -1;

    list_.setItems(items);
    clearIndicators();
}

void DeviceListOverlay::setChildrenItems(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes)
{
    item_names_ = items;
    device_states_.clear();
    has_slots_.clear();
    has_layers_.clear();
    has_drums_.clear();
    current_device_index_ = -1;

    list_.setItems(items);

    // Apply lvgl_symbols font to the Back button (index 0)
    if (!items.empty() && items[0] == Icon::ARROW_LEFT)
    {
        lv_obj_t *backBtn = list_.getButton(0);
        if (backBtn)
        {
            // Find the text label (first label child)
            uint32_t childCount = lv_obj_get_child_cnt(backBtn);
            for (uint32_t j = 0; j < childCount; j++)
            {
                lv_obj_t *child = lv_obj_get_child(backBtn, j);
                if (child && lv_obj_check_type(child, &lv_label_class))
                {
                    if (bitwig_fonts.icons_14)
                    {
                        lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
                    }
                    break;
                }
            }
        }
    }

    // Add icon labels for device children (same approach as TrackListOverlay)
    for (size_t i = 0; i < items.size(); i++)
    {
        if (i == 0 || i >= itemTypes.size())
            continue; // Skip BACK button

        lv_obj_t *btn = list_.getButton(i);
        if (!btn)
            continue;

        const char *iconSymbol = nullptr;
        switch (itemTypes[i])
        {
        case 0: // Slot
            iconSymbol = Icon::SLIDER;
            break;
        case 1: // Layer
            iconSymbol = Icon::LAYER;
            break;
        case 2: // Drum pad
            iconSymbol = Icon::DRUM_PAD_MATRIX;
            break;
        }

        if (iconSymbol)
        {
            lv_obj_t *icon_label = lv_label_create(btn);
            Icon::set(icon_label, iconSymbol);

            // Apply state-based colors to icons (same as labels)
            lv_obj_set_style_text_color(icon_label, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(icon_label, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_FOCUSED);
            lv_obj_set_style_text_color(icon_label, lv_color_white(), LV_STATE_PRESSED);
            lv_obj_set_style_text_color(icon_label, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DISABLED);
            lv_obj_set_style_text_opa(icon_label, LV_OPA_50, LV_STATE_DISABLED);

            lv_obj_move_to_index(icon_label, 0); // Icon before text
        }
    }

    clearIndicators();
}

void DeviceListOverlay::setDeviceItems(const std::vector<std::string> &names, int currentIndex,
                                       const std::vector<bool> &deviceStates,
                                       const std::vector<bool> &hasSlots,
                                       const std::vector<bool> &hasLayers,
                                       const std::vector<bool> &hasDrums)
{
    item_names_ = names;
    current_device_index_ = currentIndex;

    size_t itemCount = names.size();
    device_states_.assign(deviceStates.begin(), deviceStates.end());
    has_slots_.assign(hasSlots.begin(), hasSlots.end());
    has_layers_.assign(hasLayers.begin(), hasLayers.end());
    has_drums_.assign(hasDrums.begin(), hasDrums.end());

    device_states_.resize(itemCount, false);
    has_slots_.resize(itemCount, false);
    has_layers_.resize(itemCount, false);
    has_drums_.resize(itemCount, false);

    list_.setItems(names);
    list_.setSelectedIndex(currentIndex);

    clearIndicators();
    createIndicators();
}

void DeviceListOverlay::setSelectedIndex(int index)
{
    list_.setSelectedIndex(index);
}

void DeviceListOverlay::show()
{
    list_.show();
}

void DeviceListOverlay::hide()
{
    list_.hide();
}

bool DeviceListOverlay::isVisible() const
{
    return list_.isVisible();
}

int DeviceListOverlay::getSelectedIndex() const
{
    return list_.getSelectedIndex();
}

int DeviceListOverlay::getItemCount() const
{
    return list_.getItemCount();
}

void DeviceListOverlay::setDeviceStateAtIndex(int displayIndex, bool enabled)
{
    if (displayIndex < 0 || displayIndex >= static_cast<int>(device_states_.size()))
        return;

    device_states_[displayIndex] = enabled;

    lv_obj_t *button = list_.getButton(displayIndex);
    if (!button)
        return;

    bool isDevice = !isNonDeviceItem(displayIndex);
    if (!isDevice)
        return;

    // Device state icon is always at index 0 for devices
    uint32_t childCount = lv_obj_get_child_cnt(button);
    if (childCount > 0)
    {
        lv_obj_t *stateIcon = lv_obj_get_child(button, 0);
        if (stateIcon && lv_obj_check_type(stateIcon, &lv_label_class))
        {
            lv_label_set_text(stateIcon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
            lv_obj_set_style_text_color(stateIcon, lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
        }
    }
}

void DeviceListOverlay::clearIndicators()
{
    folder_icons_.clear();
}

bool DeviceListOverlay::isNonDeviceItem(size_t index) const
{
    if (index >= item_names_.size())
        return false;

    const std::string &name = item_names_[index];
    return name == Icon::ARROW_LEFT || (!name.empty() && name[0] == '[');
}

bool DeviceListOverlay::hasChildren(size_t index) const
{
    return index < has_slots_.size() &&
           (has_slots_[index] || has_layers_[index] || has_drums_[index]);
}

lv_obj_t *DeviceListOverlay::createDeviceStateIcon(lv_obj_t *parent, bool enabled)
{
    lv_obj_t *icon = lv_label_create(parent);
    Icon::set(icon, enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF);
    lv_obj_set_style_text_color(icon, lv_color_hex(enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED), 0);
    return icon;
}

lv_obj_t *DeviceListOverlay::createFolderIcon(lv_obj_t *parent, bool isSlot)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon = lv_label_create(container);
    Icon::set(icon, isSlot ? Icon::SLIDER : Icon::DIRECTORY);

    // Apply state-based colors with reduced opacity for folder icon
    lv_obj_set_style_text_color(icon, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(icon, LV_OPA_70, LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(icon, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(icon, LV_OPA_70, LV_STATE_FOCUSED);

    lv_obj_set_style_text_color(icon, lv_color_white(), LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(icon, LV_OPA_70, LV_STATE_PRESSED);

    lv_obj_set_style_text_color(icon, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(icon, LV_OPA_30, LV_STATE_DISABLED);

    return container;
}

void DeviceListOverlay::createIndicators()
{
    folder_icons_.clear();
    folder_icons_.resize(has_slots_.size(), nullptr);

    for (size_t i = 0; i < has_slots_.size(); i++)
    {
        lv_obj_t *button = list_.getButton(i);
        if (!button)
            continue;

        // Check if this is the "Back" item (first item with Icon::ARROW_LEFT)
        bool isBackItem = (i == 0) && (i < item_names_.size()) && (item_names_[i] == Icon::ARROW_LEFT);

        // Apply lvgl_symbols font to text label if this is the Back item
        if (isBackItem)
        {
            // Find the text label (first label child)
            uint32_t childCount = lv_obj_get_child_cnt(button);
            for (uint32_t j = 0; j < childCount; j++)
            {
                lv_obj_t *child = lv_obj_get_child(button, j);
                if (child && lv_obj_check_type(child, &lv_label_class))
                {
                    if (bitwig_fonts.icons_14)
                    {
                        lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
                    }
                    break;
                }
            }
        }

        bool isDevice = !isNonDeviceItem(i);

        // Always create device state icon at index 0 for devices (not for Back item)
        if (isDevice)
        {
            bool isEnabled = (i < device_states_.size()) && device_states_[i];
            lv_obj_t *stateIcon = createDeviceStateIcon(button, isEnabled);
            lv_obj_move_to_index(stateIcon, 0);
        }

        // Always create folder icon container at index 1 (2nd position)
        bool isFolder = hasChildren(i);
        bool isSlot = (i < has_slots_.size()) && has_slots_[i];
        lv_obj_t *folderIcon = createFolderIcon(button, isSlot);
        lv_obj_move_to_index(folderIcon, 1);
        folder_icons_[i] = folderIcon;

        // Hide if not a folder
        if (!isFolder)
        {
            lv_obj_add_flag(folderIcon, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

} // namespace Bitwig
