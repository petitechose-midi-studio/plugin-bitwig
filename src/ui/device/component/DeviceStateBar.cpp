#include "DeviceStateBar.hpp"

#include "resource/common/ui/font/binary_font_buffer.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "resource/common/ui/util/TextUtils.hpp"

namespace Bitwig
{

    const lv_color_t COLOR_BACKGROUND = lv_color_hex(Theme::Color::BACKGROUND_FILL);
    const lv_color_t COLOR_TEXT = lv_color_hex(Theme::Color::TEXT_LIGHT);
    const lv_color_t COLOR_STATE_ENABLED = lv_color_hex(Theme::Color::DEVICE_STATE_ENABLED);
    const lv_color_t COLOR_STATE_DISABLED = lv_color_hex(Theme::Color::DEVICE_STATE_DISABLED);

    DeviceStateBar::DeviceStateBar(lv_obj_t *parent)
        : track_name_("No Track"), device_name_("No Device"), device_status_(DeviceStatus::DISABLED)
    {
        container_ = lv_obj_create(parent);
        lv_obj_set_size(container_, LV_PCT(100), 20);
        lv_obj_set_pos(container_, 0, 0);
        lv_obj_set_style_bg_color(container_, COLOR_BACKGROUND, 0);
        lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(container_, 0, 0);
        lv_obj_set_style_pad_left(container_, 0, 0);
        lv_obj_set_style_pad_right(container_, 4, 0);
        lv_obj_set_style_pad_top(container_, 0, 0);
        lv_obj_set_style_pad_bottom(container_, 0, 0);

        lv_obj_set_scrollbar_mode(container_, LV_SCROLLBAR_MODE_OFF);

        static const lv_coord_t col_dsc[] = {LV_GRID_FR(1),
                                             LV_GRID_FR(1),
                                             LV_GRID_FR(1),
                                             LV_GRID_TEMPLATE_LAST};
        static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

        lv_obj_set_layout(container_, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);

        createTrackSection();
        createDeviceSection();
        createPageSection();

        setTrackName("No Track");
        setDeviceName("No Device");
        setDeviceState(0);
        setPageName("");
    }

    DeviceStateBar::~DeviceStateBar()
    {
        if (container_)
        {
            lv_obj_delete(container_);
        }
    }

    void DeviceStateBar::createTrackSection()
    {
        track_container_ = lv_obj_create(container_);
        lv_obj_set_grid_cell(track_container_, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
        lv_obj_set_style_bg_opa(track_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(track_container_, 0, 0);
        lv_obj_set_style_pad_all(track_container_, 0, 0);
        lv_obj_set_style_pad_gap(track_container_, 4, 0);
        lv_obj_set_scrollbar_mode(track_container_, LV_SCROLLBAR_MODE_OFF);

        lv_obj_set_flex_flow(track_container_, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(track_container_,
                              LV_FLEX_ALIGN_START,
                              LV_FLEX_ALIGN_CENTER,
                              LV_FLEX_ALIGN_CENTER);

        track_color_indicator_ = lv_obj_create(track_container_);
        lv_obj_set_size(track_color_indicator_, 4, LV_PCT(100));
        lv_obj_set_style_radius(track_color_indicator_, 0, 0);
        lv_obj_set_style_border_width(track_color_indicator_, 0, 0);
        lv_obj_set_style_pad_all(track_color_indicator_, 0, 0);
        lv_obj_set_style_bg_color(track_color_indicator_, lv_color_hex(track_color_), 0);
        lv_obj_set_style_bg_opa(track_color_indicator_, LV_OPA_COVER, 0);

        track_label_ = lv_label_create(track_container_);
        lv_obj_set_style_text_color(track_label_, COLOR_TEXT, 0);
        lv_obj_set_style_text_font(track_label_, fonts.tempo_label, 0);

        String clean_name = TextUtils::sanitizeText(track_name_);
        lv_label_set_text(track_label_, clean_name.c_str());
    }

    void DeviceStateBar::createDeviceSection()
    {
        device_container_ = lv_obj_create(container_);
        lv_obj_set_grid_cell(device_container_, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
        lv_obj_set_style_bg_opa(device_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(device_container_, 0, 0);
        lv_obj_set_style_pad_all(device_container_, 0, 0);
        lv_obj_set_style_pad_gap(device_container_, 4, 0);

        lv_obj_set_flex_flow(device_container_, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(device_container_,
                              LV_FLEX_ALIGN_CENTER,
                              LV_FLEX_ALIGN_CENTER,
                              LV_FLEX_ALIGN_CENTER);

        device_icon_ = lv_obj_create(device_container_);
        lv_obj_set_size(device_icon_, 10, 10);
        lv_obj_set_style_radius(device_icon_, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(device_icon_, 0, 0);
        lv_obj_set_style_bg_color(device_icon_, COLOR_STATE_ENABLED, 0);
        lv_obj_set_style_bg_opa(device_icon_, LV_OPA_COVER, 0);

        device_label_ = lv_label_create(device_container_);
        lv_obj_set_style_text_color(device_label_, COLOR_TEXT, 0);
        lv_obj_set_style_text_font(device_label_, fonts.device_label, 0);

        String clean_name = TextUtils::sanitizeText(device_name_);
        lv_label_set_text(device_label_, clean_name.c_str());
    }

    void DeviceStateBar::createPageSection()
    {
        page_label_ = lv_label_create(container_);
        lv_obj_set_grid_cell(page_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        lv_label_set_text(page_label_, "Page");
        lv_obj_set_style_text_color(page_label_, COLOR_TEXT, 0);
        lv_obj_set_style_text_font(page_label_, fonts.page_label, 0);
    }

    void DeviceStateBar::setTrackName(const String &track_name)
    {
        track_name_ = track_name;
        if (track_label_)
        {
            const lv_font_t *font = lv_obj_get_style_text_font(track_label_, LV_PART_MAIN);
            lv_coord_t available_width = getTrackAvailableWidth();

            String clean_name = TextUtils::sanitizeText(track_name);
            String truncated_text = TextUtils::truncateWithEllipsis(clean_name, available_width, font);
            lv_label_set_text(track_label_, truncated_text.c_str());
        }
    }

    void DeviceStateBar::setTrackColor(uint32_t color)
    {
        track_color_ = color;
        if (track_color_indicator_)
        {
            lv_obj_set_style_bg_color(track_color_indicator_, lv_color_hex(color), 0);
        }
    }

    void DeviceStateBar::setDeviceName(const String &device_name)
    {
        device_name_ = device_name;
        if (device_label_)
        {
            const lv_font_t *font = lv_obj_get_style_text_font(device_label_, LV_PART_MAIN);
            lv_coord_t available_width = getDeviceAvailableWidth();

            String clean_name = TextUtils::sanitizeText(device_name);
            String truncated_text = TextUtils::truncateWithEllipsis(clean_name, available_width, font);
            lv_label_set_text(device_label_, truncated_text.c_str());
        }
    }

    void DeviceStateBar::setDeviceState(uint8_t state)
    {
        device_status_ = (state == 1) ? DeviceStatus::ENABLED : DeviceStatus::DISABLED;

        if (device_icon_)
        {
            lv_color_t indicator_color =
                (device_status_ == DeviceStatus::ENABLED) ? COLOR_STATE_ENABLED : COLOR_STATE_DISABLED;
            lv_obj_set_style_bg_color(device_icon_, indicator_color, 0);
        }

        if (device_label_)
        {
            if (device_status_ == DeviceStatus::ENABLED)
            {
                lv_obj_set_style_text_color(device_label_, COLOR_TEXT, 0);
                lv_obj_set_style_text_opa(device_label_, LV_OPA_COVER, 0);
            }
            else
            {
                lv_obj_set_style_text_color(device_label_, COLOR_TEXT, 0);
                lv_obj_set_style_text_opa(device_label_, LV_OPA_50, 0);
            }
        }
    }

    void DeviceStateBar::setPageName(const String &page_name)
    {
        if (page_label_)
        {
            if (page_name.length() > 0)
            {
                String clean_name = TextUtils::sanitizeText(page_name);
                lv_label_set_text(page_label_, clean_name.c_str());
            }
            else
            {
                lv_label_set_text(page_label_, "Page");
            }
        }
    }

    lv_coord_t DeviceStateBar::getTrackAvailableWidth() const
    {
        if (!container_ || !track_color_indicator_)
            return 0;
        return (lv_obj_get_width(container_) / 3) - lv_obj_get_width(track_color_indicator_) - 4;
    }

    lv_coord_t DeviceStateBar::getDeviceAvailableWidth() const
    {
        if (!container_ || !device_icon_)
            return 0;
        return (lv_obj_get_width(container_) / 3) - lv_obj_get_width(device_icon_) - 4;
    }

} // namespace Bitwig
