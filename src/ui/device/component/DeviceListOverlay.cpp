#include "DeviceListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "font/binary_font_buffer.hpp"
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
    updateBulletStates();
}

void DeviceListOverlay::setSelectedIndex(int index)
{
    list_.setSelectedIndex(index);
    updateBulletStates();
}

void DeviceListOverlay::show()
{
    list_.show();
    updateBulletStates();
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
    if (displayIndex >= 0 && displayIndex < static_cast<int>(device_states_.size()))
    {
        device_states_[displayIndex] = enabled;
        updateBulletStates();
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
    return name == "Back to parent" || (!name.empty() && name[0] == '[');
}

bool DeviceListOverlay::hasChildren(size_t index) const
{
    return index < has_slots_.size() &&
           (has_slots_[index] || has_layers_[index] || has_drums_[index]);
}

lv_obj_t *DeviceListOverlay::createDot(lv_obj_t *parent, uint32_t color)
{
    lv_obj_t *dot = lv_obj_create(parent);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_set_style_bg_color(dot, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(dot, 0, 0);
    lv_obj_set_style_pad_all(dot, 0, 0);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    return dot;
}

lv_obj_t *DeviceListOverlay::createFolderIcon(lv_obj_t *parent)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon = lv_label_create(container);
    lv_label_set_text(icon, LV_SYMBOL_DIRECTORY);
    if (fonts.lvgl_symbols)
    {
        lv_obj_set_style_text_font(icon, fonts.lvgl_symbols, 0);
    }
    lv_obj_set_style_text_color(icon, lv_color_hex(Color::TEXT_PRIMARY), 0);
    lv_obj_set_style_text_opa(icon, LV_OPA_70, 0);

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

        bool isFolder = hasChildren(i);
        bool isDevice = !isNonDeviceItem(i);

        if (isFolder)
        {
            lv_obj_t *folderIcon = createFolderIcon(button);
            lv_obj_move_to_index(folderIcon, 0);
            folder_icons_[i] = folderIcon;
        }

        if (isDevice)
        {
            lv_obj_t *stateBullet = createDot(button, Color::DEVICE_STATE_ENABLED);
            lv_obj_move_to_index(stateBullet, isFolder ? 1 : 0);
        }
    }
}

void DeviceListOverlay::updateBulletStates()
{
    int itemCount = list_.getItemCount();
    int selectedIndex = list_.getSelectedIndex();

    for (int i = 0; i < itemCount; i++)
    {
        lv_obj_t *button = list_.getButton(i);
        if (!button)
            continue;

        bool isSelected = (i == selectedIndex);
        bool isEnabled = (i < static_cast<int>(device_states_.size())) && device_states_[i];
        bool isDevice = !isNonDeviceItem(i);
        bool hasFolder = (i < static_cast<int>(folder_icons_.size())) && folder_icons_[i];

        uint32_t childCount = lv_obj_get_child_cnt(button);

        if (isDevice)
        {
            uint32_t bulletIndex = hasFolder ? 1 : 0;
            if (bulletIndex < childCount)
            {
                lv_obj_t *bullet = lv_obj_get_child(button, bulletIndex);
                lv_color_t color = isEnabled ? lv_color_hex(Color::DEVICE_STATE_ENABLED)
                                             : lv_color_hex(Color::DEVICE_STATE_DISABLED);
                lv_opa_t opacity = isSelected ? LV_OPA_COVER : LV_OPA_50;

                lv_obj_set_style_bg_color(bullet, color, 0);
                lv_obj_set_style_bg_opa(bullet, opacity, 0);
            }
        }

        uint32_t labelIndex = (hasFolder ? 1 : 0) + (isDevice ? 1 : 0);
        if (labelIndex < childCount)
        {
            lv_obj_t *label = lv_obj_get_child(button, labelIndex);
            lv_color_t labelColor = isSelected ? lv_color_hex(Color::TEXT_PRIMARY)
                                                : lv_color_hex(Color::INACTIVE_LIGHTER);
            lv_obj_set_style_text_color(label, labelColor, 0);
            lv_obj_set_style_text_opa(label, LV_OPA_COVER, 0);
        }
    }
}

} // namespace Bitwig
