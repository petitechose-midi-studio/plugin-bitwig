#include "DeviceStateBar.hpp"
#include "../../theme/BitwigTheme.hpp"

namespace Bitwig
{

    DeviceStateBar::DeviceStateBar(lv_obj_t *parent)
    {
        container_ = lv_obj_create(parent);
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

        // Grid layout: [Track] [Device] [Page]
        static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        lv_obj_set_layout(container_, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
        lv_obj_set_style_pad_column(container_, 6, 0);  // 6px gap between columns

        // Track cell (left) - content left-aligned, clips on right
        track_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_START);
        lv_obj_set_grid_cell(track_cell_, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
        track_item_ = std::make_unique<TrackTitleItem>(track_cell_);

        // Device cell (center) - centered when fits, left-aligned when overflow
        device_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_grid_cell(device_cell_, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
        device_item_ = std::make_unique<DeviceTitleItem>(device_cell_, DeviceTitleItem::IconSize::Medium);

        // Page cell (right) - content right-aligned, clips on left
        page_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_END);
        lv_obj_set_grid_cell(page_cell_, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
        page_item_ = std::make_unique<PageTitleItem>(page_cell_);

        // Set defaults (without triggering alignment update during construction)
        if (track_item_)
        {
            track_item_->setName("No Track");
            track_item_->setColor(0xFFFFFF);
            track_item_->setType(0);
        }
        if (device_item_)
        {
            device_item_->setName("No Device");
            device_item_->setState(false);
        }
        if (page_item_)
        {
            page_item_->setName("");
        }
    }

    DeviceStateBar::~DeviceStateBar()
    {
        // unique_ptr handles cleanup of items
        if (container_)
        {
            lv_obj_delete(container_);
        }
    }

    void DeviceStateBar::setTrackName(const String &track_name)
    {
        if (track_item_)
        {
            track_item_->setName(track_name.c_str());
        }
    }

    void DeviceStateBar::setTrackColor(uint32_t color)
    {
        if (track_item_)
        {
            track_item_->setColor(color);
        }
    }

    void DeviceStateBar::setTrackType(uint8_t trackType)
    {
        if (track_item_)
        {
            track_item_->setType(trackType);
        }
    }

    void DeviceStateBar::setDeviceName(const String &device_name)
    {
        if (device_item_)
        {
            device_item_->setName(device_name.c_str());
            updateDeviceCellAlignment();
        }
    }

    void DeviceStateBar::updateDeviceCellAlignment()
    {
        if (!device_cell_ || !device_item_)
            return;

        // Defer to next frame when layout is calculated
        lv_timer_t *timer = lv_timer_create(
            [](lv_timer_t *t)
            {
                auto *self = static_cast<DeviceStateBar *>(lv_timer_get_user_data(t));
                if (self && self->device_cell_ && self->device_item_)
                {
                    lv_obj_update_layout(self->device_cell_);
                    lv_coord_t content_w = self->device_item_->getContentWidth();
                    lv_coord_t cell_w = lv_obj_get_width(self->device_cell_);

                    // Center if fits, left-align if overflows
                    lv_flex_align_t align = (content_w > cell_w) ? LV_FLEX_ALIGN_START : LV_FLEX_ALIGN_CENTER;
                    lv_obj_set_flex_align(self->device_cell_, align, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                }
            },
            50, this);
        lv_timer_set_repeat_count(timer, 1);
    }

    void DeviceStateBar::setDeviceState(bool enabled)
    {
        if (device_item_)
        {
            device_item_->setState(enabled);
        }
    }

    void DeviceStateBar::setDeviceHasChildren(bool hasChildren)
    {
        if (device_item_)
        {
            device_item_->setHasChildren(hasChildren);
        }
    }

    void DeviceStateBar::setPageName(const String &page_name)
    {
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
        lv_obj_remove_flag(cell, LV_OBJ_FLAG_OVERFLOW_VISIBLE);  // Clip content overflow

        // Flex layout for internal alignment
        lv_obj_set_flex_flow(cell, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(cell, hAlign, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_gap(cell, 6, 0);  // 6px gap between children

        return cell;
    }

} // namespace Bitwig
