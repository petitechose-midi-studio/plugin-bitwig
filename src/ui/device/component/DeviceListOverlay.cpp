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

    // Helper function to create a circular indicator dot
    static lv_obj_t* createDot(lv_obj_t* parent, uint32_t color)
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

    void DeviceListOverlay::createIndicators()
    {
        indicator_dots_.clear();
        indicator_dots_.resize(has_slots_.size());

        for (size_t i = 0; i < has_slots_.size(); i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            // Create state bullet (enabled/disabled indicator) and move to index 0
            lv_obj_t *state_bullet = createDot(btn, Color::DEVICE_STATE_ENABLED);
            lv_obj_move_to_index(state_bullet, 0);

            // Create type indicator dots (drums, layers, slots)
            std::vector<lv_obj_t *> item_dots;
            if (has_drums_[i])  item_dots.push_back(createDot(btn, Color::DEVICE_DRUM_PAD));
            if (has_layers_[i]) item_dots.push_back(createDot(btn, Color::DEVICE_LAYER));
            if (has_slots_[i])  item_dots.push_back(createDot(btn, Color::DEVICE_SLOT));

            indicator_dots_[i] = item_dots;
        }
    }

    void DeviceListOverlay::updateBulletStates()
    {
        int itemCount = list_.getItemCount();
        int selectedIndex = list_.getSelectedIndex();

        bool hasIndicators = !indicator_dots_.empty();

        for (int i = 0; i < itemCount; i++)
        {
            lv_obj_t *btn = list_.getButton(i);
            if (!btn)
                continue;

            bool isNonDeviceItem = false;
            if (i < static_cast<int>(item_names_.size()))
            {
                const std::string &itemName = item_names_[i];
                isNonDeviceItem = (!itemName.empty() && itemName[0] == '[') ||
                                  (itemName == "Back to parent");
            }

            bool isHighlighted = (i == selectedIndex);
            bool isEnabled = (i < static_cast<int>(device_states_.size()))
                                 ? device_states_[i]
                                 : false;

            // Update bullet only if indicators exist
            if (hasIndicators)
            {
                lv_obj_t *bullet = lv_obj_get_child(btn, 0);
                if (bullet)
                {
                    // Update bullet (hide for non-device items)
                    if (isNonDeviceItem)
                    {
                        lv_obj_set_style_bg_opa(bullet, LV_OPA_TRANSP, 0);
                    }
                    else
                    {
                        lv_color_t color = isEnabled
                                               ? lv_color_hex(Color::DEVICE_STATE_ENABLED)
                                               : lv_color_hex(Color::DEVICE_STATE_DISABLED);
                        bool isCurrent = (i == current_device_index_);
                        lv_opa_t opa = isHighlighted ? LV_OPA_COVER : isCurrent ? LV_OPA_70 : LV_OPA_50;

                        lv_obj_set_style_bg_color(bullet, color, 0);
                        lv_obj_set_style_bg_opa(bullet, opa, 0);
                    }
                }
            }

            // Update label (same logic for all items)
            // Label is at index 1 if indicators exist, index 0 otherwise
            int labelIndex = hasIndicators ? 1 : 0;
            lv_obj_t *label = lv_obj_get_child(btn, labelIndex);
            if (label)
            {
                lv_color_t label_color = isHighlighted
                                             ? lv_color_hex(Color::TEXT_PRIMARY)
                                             : lv_color_hex(Color::INACTIVE_LIGHTER);
                lv_obj_set_style_text_color(label, label_color, 0);
                lv_obj_set_style_text_opa(label, LV_OPA_COVER, 0);
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
