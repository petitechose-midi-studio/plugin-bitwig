#include "DeviceStateBar.hpp"
#include "../../theme/BitwigTheme.hpp"

namespace Bitwig
{

    DeviceStateBar::DeviceStateBar(lv_obj_t *parent)
        : parent_(parent)
    {
        // Lazy init - don't create LVGL widgets here
    }

    DeviceStateBar::~DeviceStateBar()
    {
        // unique_ptr handles cleanup of items
        if (container_)
        {
            lv_obj_delete(container_);
        }
    }

    void DeviceStateBar::ensureCreated()
    {
        if (container_ || !parent_) return;

        container_ = lv_obj_create(parent_);
        if (!container_) return;

        lv_obj_set_size(container_, LV_PCT(100), 20);
        lv_obj_set_pos(container_, 0, 0);
        lv_obj_set_style_bg_color(container_, lv_color_hex(Theme::Color::BACKGROUND_FILL), 0);
        lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(container_, 0, 0);
        lv_obj_set_style_pad_left(container_, 4, 0);
        lv_obj_set_style_pad_right(container_, 4, 0);
        lv_obj_set_style_pad_top(container_, 0, 0);
        lv_obj_set_style_pad_bottom(container_, 0, 0);
        lv_obj_set_scrollbar_mode(container_, LV_SCROLLBAR_MODE_OFF);

        // Grid layout: [Device] [Page]
        static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_layout(container_, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
        lv_obj_set_style_pad_column(container_, 6, 0);

        // Device cell (left) - content left-aligned
        device_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_START);
        lv_obj_set_grid_cell(device_cell_, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
        device_item_ = std::make_unique<DeviceTitleItem>(device_cell_, DeviceTitleItem::IconSize::Medium);

        // Page cell (right) - content right-aligned
        page_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_END);
        lv_obj_set_grid_cell(page_cell_, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
        page_item_ = std::make_unique<PageTitleItem>(page_cell_);

        // Apply pending state
        if (device_item_)
        {
            device_item_->setName(pending_device_name_.c_str());
            device_item_->setState(pending_device_enabled_);
            device_item_->setHasChildren(pending_device_has_children_);
        }
        if (page_item_)
        {
            page_item_->setName(pending_page_name_.c_str());
        }
    }

    void DeviceStateBar::setDeviceName(const String &device_name)
    {
        pending_device_name_ = device_name;
        ensureCreated();
        if (device_item_)
        {
            device_item_->setName(device_name.c_str());
        }
    }

    void DeviceStateBar::setDeviceState(bool enabled)
    {
        pending_device_enabled_ = enabled;
        ensureCreated();
        if (device_item_)
        {
            device_item_->setState(enabled);
        }
    }

    void DeviceStateBar::setDeviceHasChildren(bool hasChildren)
    {
        pending_device_has_children_ = hasChildren;
        ensureCreated();
        if (device_item_)
        {
            device_item_->setHasChildren(hasChildren);
        }
    }

    void DeviceStateBar::setPageName(const String &page_name)
    {
        pending_page_name_ = page_name;
        ensureCreated();
        if (page_item_)
        {
            page_item_->setName(page_name.c_str());
        }
    }

    lv_obj_t* DeviceStateBar::createCellWrapper(lv_obj_t* parent, lv_flex_align_t hAlign)
    {
        lv_obj_t* cell = lv_obj_create(parent);
        lv_obj_set_size(cell, LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_bg_opa(cell, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(cell, 0, 0);
        lv_obj_set_style_pad_all(cell, 0, 0);
        lv_obj_clear_flag(cell, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_remove_flag(cell, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

        // Flex layout for internal alignment
        lv_obj_set_flex_flow(cell, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(cell, hAlign, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_gap(cell, 6, 0);

        return cell;
    }

} // namespace Bitwig
