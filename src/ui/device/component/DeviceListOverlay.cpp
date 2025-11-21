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

            bool isNonDeviceItem = (i < item_names_.size()) &&
                                   (item_names_[i] == "Back to parent" ||
                                    (!item_names_[i].empty() && item_names_[i][0] == '['));

            if (!isNonDeviceItem)
            {
                lv_obj_t *state_bullet = createDot(btn, Color::DEVICE_STATE_ENABLED);
                lv_obj_move_to_index(state_bullet, 0);
            }

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

            bool isHighlighted = (i == selectedIndex);
            bool isEnabled = (i < static_cast<int>(device_states_.size()))
                                 ? device_states_[i]
                                 : false;

            // Determine if this is a non-device item (Back to parent or folder)
            bool isNonDeviceItem = (i < static_cast<int>(item_names_.size())) &&
                                   (item_names_[i] == "Back to parent" ||
                                    (!item_names_[i].empty() && item_names_[i][0] == '['));

            uint32_t childCount = lv_obj_get_child_cnt(btn);
            lv_obj_t *firstChild = (childCount > 0) ? lv_obj_get_child(btn, 0) : nullptr;

            // Update state bullet (only for real devices, not Back/folders)
            if (hasIndicators && firstChild && !isNonDeviceItem)
            {
                lv_color_t color = isEnabled
                                       ? lv_color_hex(Color::DEVICE_STATE_ENABLED)
                                       : lv_color_hex(Color::DEVICE_STATE_DISABLED);
                lv_opa_t opa = isHighlighted ? LV_OPA_COVER : LV_OPA_50;

                lv_obj_set_style_bg_color(firstChild, color, 0);
                lv_obj_set_style_bg_opa(firstChild, opa, 0);
            }

            uint32_t labelIndex = isNonDeviceItem ? 0 : 1;
            lv_obj_t *label = (labelIndex < childCount) ? lv_obj_get_child(btn, labelIndex) : nullptr;
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
        indicator_dots_.clear();
    }

} // namespace Bitwig
