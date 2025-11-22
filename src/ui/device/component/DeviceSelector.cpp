#include "DeviceSelector.hpp"
#include "font/binary_font_buffer.hpp"
#include "../../theme/BitwigTheme.hpp"

namespace Bitwig
{

    DeviceSelector::DeviceSelector(lv_obj_t *parent)
        : parent_(parent), overlay_(parent)
    {
        overlay_.setTitle("Select Device");
    }

    DeviceSelector::~DeviceSelector()
    {
        destroyFooter();
    }

    void DeviceSelector::setTitle(const std::string &title)
    {
        overlay_.setTitle(title);
    }

    void DeviceSelector::setItems(const std::vector<std::string> &items)
    {
        items_ = items;
        overlay_.setItems(items_);
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
        overlay_.setDeviceItems(names, currentIndex, deviceStates, hasSlots, hasLayers, hasDrums);
    }

    void DeviceSelector::setChildrenItems(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes)
    {
        items_ = items;
        overlay_.setChildrenItems(items, itemTypes);
    }

    void DeviceSelector::setCurrentItemIndex(int index)
    {
        current_item_index_ = index;

        overlay_.setSelectedIndex(index);
    }

    void DeviceSelector::setSelectedIndex(int index)
    {
        overlay_.setSelectedIndex(index);
    }

    void DeviceSelector::show()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();
    }

    void DeviceSelector::showWithFooter()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();

        if (!footer_container_)
        {
            createFooter();
        }
        else
        {
            lv_obj_clear_flag(footer_container_, LV_OBJ_FLAG_HIDDEN);
        }
    }

    void DeviceSelector::showWithoutFooter()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();

        if (footer_container_)
        {
            lv_obj_add_flag(footer_container_, LV_OBJ_FLAG_HIDDEN);
        }
    }

    void DeviceSelector::hide()
    {
        overlay_.hide();

        // Hide footer when overlay is hidden
        if (footer_container_)
        {
            lv_obj_add_flag(footer_container_, LV_OBJ_FLAG_HIDDEN);
        }
    }

    bool DeviceSelector::isVisible() const
    {
        return overlay_.isVisible();
    }

    int DeviceSelector::getSelectedIndex() const
    {
        return overlay_.getSelectedIndex();
    }

    int DeviceSelector::getItemCount() const
    {
        return overlay_.getItemCount();
    }

    void DeviceSelector::setDeviceStateAtIndex(int displayIndex, bool enabled)
    {
        overlay_.setDeviceStateAtIndex(displayIndex, enabled);
    }

    void DeviceSelector::createFooter()
    {
        if (!parent_)
            return;

        // Create footer container at parent screen level (fixed position, bottom center, full width)
        footer_container_ = lv_obj_create(parent_);
        lv_obj_add_flag(footer_container_, LV_OBJ_FLAG_FLOATING);   // Remove from parent layout
        lv_obj_set_size(footer_container_, LV_PCT(100), 30);        // Full width, fixed height
        lv_obj_align(footer_container_, LV_ALIGN_BOTTOM_MID, 0, 0); // Centered at bottom, flush with edge
        lv_obj_set_style_bg_opa(footer_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(footer_container_, 0, 0);
        lv_obj_set_style_pad_left(footer_container_, 10, 0);
        lv_obj_set_style_pad_right(footer_container_, 10, 0);
        lv_obj_set_style_pad_top(footer_container_, 0, 0); // No vertical padding
        lv_obj_set_style_pad_bottom(footer_container_, 0, 0);

        // Grid layout: 1 row × 3 columns (left, center, right - each taking equal space)
        lv_obj_set_layout(footer_container_, LV_LAYOUT_GRID);

        static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // Equal fractions
        static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(footer_container_, col_dsc, row_dsc);

        lv_obj_set_style_pad_column(footer_container_, 10, 0); // Gap between columns

        // Label gauche : "Track" (cell 0, 0)
        footer_left_label_ = lv_label_create(footer_container_);
        lv_label_set_text(footer_left_label_, "Track");
        lv_obj_set_style_text_color(footer_left_label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_grid_cell(footer_left_label_, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        // Label centre : "State" (cell 1, 0) - centered in its cell
        footer_center_label_ = lv_label_create(footer_container_);
        lv_label_set_text(footer_center_label_, "State");
        lv_obj_set_style_text_color(footer_center_label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_grid_cell(footer_center_label_, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        // Cell droite (2, 0) reste vide

        // Use small font if available
        if (fonts.tempo_label)
        {
            lv_obj_set_style_text_font(footer_left_label_, fonts.tempo_label, 0);
            lv_obj_set_style_text_font(footer_center_label_, fonts.tempo_label, 0);
        }
    }

    void DeviceSelector::destroyFooter()
    {
        if (footer_container_)
        {
            lv_obj_del(footer_container_);
            footer_container_ = nullptr;
            footer_left_label_ = nullptr;
            footer_center_label_ = nullptr;
        }
    }

} // namespace Bitwig
