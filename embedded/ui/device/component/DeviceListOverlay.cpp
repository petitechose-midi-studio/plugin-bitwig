#include "DeviceListOverlay.hpp"
#include "../../theme/BitwigTheme.hpp"
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
        current_device_index_ = -1;
        has_slots_.clear();
        has_layers_.clear();
        has_drums_.clear();

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
        device_states_ = deviceStates;
        has_slots_ = hasSlots;
        has_layers_ = hasLayers;
        has_drums_ = hasDrums;

        device_states_.resize(names.size(), false);
        has_slots_.resize(names.size(), false);
        has_layers_.resize(names.size(), false);
        has_drums_.resize(names.size(), false);

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

    void DeviceListOverlay::createIndicators()
    {
        indicator_dots_.clear();
        indicator_dots_.resize(has_slots_.size());

        for (size_t i = 0; i < has_slots_.size(); i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            std::vector<lv_obj_t *> item_dots;

            // Create state bullet (enabled/disabled indicator) first
            lv_obj_t *state_bullet = lv_obj_create(btn);
            lv_obj_set_size(state_bullet, 10, 10);
            lv_obj_set_style_bg_color(state_bullet, lv_color_hex(Color::DEVICE_STATE_ENABLED), 0);
            lv_obj_set_style_bg_opa(state_bullet, LV_OPA_COVER, 0);
            lv_obj_set_style_radius(state_bullet, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_border_width(state_bullet, 0, 0);
            lv_obj_set_style_pad_all(state_bullet, 0, 0);
            lv_obj_clear_flag(state_bullet, LV_OBJ_FLAG_SCROLLABLE);
            // Move to index 0 (before label)
            lv_obj_move_to_index(state_bullet, 0);

            if (has_drums_[i])
            {
                lv_obj_t *dot = lv_obj_create(btn);
                lv_obj_set_size(dot, 10, 10);
                lv_obj_set_style_bg_color(dot, lv_color_hex(Color::DEVICE_DRUM_PAD), 0);
                lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
                lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
                lv_obj_set_style_border_width(dot, 0, 0);
                lv_obj_set_style_pad_all(dot, 0, 0);
                lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
                item_dots.push_back(dot);
            }

            if (has_layers_[i])
            {
                lv_obj_t *dot = lv_obj_create(btn);
                lv_obj_set_size(dot, 10, 10);
                lv_obj_set_style_bg_color(dot, lv_color_hex(Color::DEVICE_LAYER), 0);
                lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
                lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
                lv_obj_set_style_border_width(dot, 0, 0);
                lv_obj_set_style_pad_all(dot, 0, 0);
                lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
                item_dots.push_back(dot);
            }

            if (has_slots_[i])
            {
                lv_obj_t *dot = lv_obj_create(btn);
                lv_obj_set_size(dot, 10, 10);
                lv_obj_set_style_bg_color(dot, lv_color_hex(Color::DEVICE_SLOT), 0);
                lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
                lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
                lv_obj_set_style_border_width(dot, 0, 0);
                lv_obj_set_style_pad_all(dot, 0, 0);
                lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
                item_dots.push_back(dot);
            }

            indicator_dots_[i] = item_dots;
        }
    }

    void DeviceListOverlay::updateBulletStates()
    {
        int itemCount = list_.getItemCount();
        int selectedIndex = list_.getSelectedIndex();

        for (int i = 0; i < itemCount; i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            lv_obj_t *bullet = lv_obj_get_child(btn, 0);
            if (!bullet)
                continue;

            bool isNonDeviceItem = false;
            if (i < static_cast<int>(item_names_.size()))
            {
                const std::string &itemName = item_names_[i];
                isNonDeviceItem = (!itemName.empty() && itemName[0] == '[') ||
                                  (itemName == "Back to parent");
            }

            if (isNonDeviceItem)
            {
                lv_obj_set_style_bg_opa(bullet, LV_OPA_TRANSP, 0);
                continue;
            }

            bool isEnabled = (i < static_cast<int>(device_states_.size()))
                                 ? device_states_[i]
                                 : false;
            lv_color_t color = isEnabled
                                   ? lv_color_hex(Color::DEVICE_STATE_ENABLED)
                                   : lv_color_hex(Color::DEVICE_STATE_DISABLED);

            bool isHighlighted = (i == selectedIndex);
            bool isCurrent = (i == current_device_index_);

            lv_opa_t opa = isHighlighted ? LV_OPA_COVER : isCurrent ? LV_OPA_70
                                                                    : LV_OPA_50;

            lv_obj_set_style_bg_color(bullet, color, 0);
            lv_obj_set_style_bg_opa(bullet, opa, 0);

            // Update label color and opacity
            // Label is now after state bullet: state_bullet=0, label=1, [dots...]
            lv_obj_t *label = lv_obj_get_child(btn, 1);
            if (label)
            {
                // Change color based on highlight (like track list)
                lv_color_t label_color = isHighlighted
                                             ? lv_color_hex(Color::TEXT_PRIMARY)
                                             : lv_color_hex(Color::INACTIVE_LIGHTER);
                lv_obj_set_style_text_color(label, label_color, 0);

                // Update opacity based on device state (matching DeviceStateBar behavior)
                lv_opa_t label_opa = isEnabled ? LV_OPA_COVER : LV_OPA_50;
                lv_obj_set_style_text_opa(label, label_opa, 0);
            }
        }
    }

    void DeviceListOverlay::setDeviceStateAtIndex(uint8_t deviceIndex, bool enabled)
    {
        // Check if we have a back button (nested mode)
        bool isNested = !item_names_.empty() &&
                        (item_names_[0] == "Back to parent" || item_names_[0] == "[^ Back]");

        // Adjust index for display (add 1 if nested to skip back button)
        int displayIndex = isNested ? deviceIndex + 1 : deviceIndex;

        // Update the cached state at the display index
        if (displayIndex >= 0 && displayIndex < static_cast<int>(device_states_.size()))
        {
            device_states_[displayIndex] = enabled;
            updateBulletStates();
        }
    }

    void DeviceListOverlay::clearIndicators()
    {
        indicator_dots_.clear();
    }

} // namespace Bitwig
