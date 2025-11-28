#include "TrackTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"
#include "util/TextUtils.hpp"

namespace Bitwig
{

    TrackTitleItem::TrackTitleItem(lv_obj_t *parent)
    {
        container_ = lv_obj_create(parent);
        lv_obj_set_size(container_, LV_PCT(100), LV_PCT(100));  // Fill parent cell
        lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(container_, 0, 0);
        lv_obj_set_style_pad_all(container_, 0, 0);
        lv_obj_set_style_pad_gap(container_, 4, 0);
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_,
                              LV_FLEX_ALIGN_START,
                              LV_FLEX_ALIGN_CENTER,
                              LV_FLEX_ALIGN_CENTER);

        // Color bar
        color_bar_ = lv_obj_create(container_);
        lv_obj_set_size(color_bar_, 4, LV_PCT(100));
        lv_obj_set_style_radius(color_bar_, 0, 0);
        lv_obj_set_style_border_width(color_bar_, 0, 0);
        lv_obj_set_style_pad_all(color_bar_, 0, 0);
        lv_obj_set_style_bg_color(color_bar_, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_bg_opa(color_bar_, LV_OPA_COVER, 0);
        lv_obj_clear_flag(color_bar_, LV_OBJ_FLAG_SCROLLABLE);

        // Track type icon
        type_icon_ = lv_label_create(container_);
        Icon::set(type_icon_, Icon::TRACK_AUDIO, Icon::S14);
        lv_obj_set_style_text_color(type_icon_, lv_color_hex(Theme::Color::TEXT_PRIMARY), 0);
        lv_obj_set_style_text_opa(type_icon_, LV_OPA_70, 0);

        // Track name label with auto-scroll
        label_ = std::make_unique<Label>(container_);
        label_->setColor(lv_color_hex(Theme::Color::TEXT_LIGHT));
        label_->setFont(fonts.tempo_label);
    }

    TrackTitleItem::~TrackTitleItem()
    {
        if (container_)
        {
            lv_obj_delete(container_);
        }
    }

    void TrackTitleItem::setName(const std::string &name)
    {
        if (label_)
        {
            std::string clean = TextUtils::sanitizeText(name.c_str()).c_str();
            label_->setText(clean);
        }
    }

    void TrackTitleItem::setColor(uint32_t color)
    {
        if (color_bar_)
        {
            lv_obj_set_style_bg_color(color_bar_, lv_color_hex(color), 0);
        }
    }

    void TrackTitleItem::setType(uint8_t trackType)
    {
        if (type_icon_)
        {
            Icon::set(type_icon_, getTrackTypeIcon(trackType), Icon::S14);
        }
    }

    const char *TrackTitleItem::getTrackTypeIcon(uint8_t trackType)
    {
        switch (trackType)
        {
        case 0:
            return Icon::TRACK_AUDIO;
        case 1:
            return Icon::TRACK_INSTRUMENT;
        case 2:
            return Icon::TRACK_HYBRID;
        case 3:
            return Icon::DIRECTORY;
        case 4:
            return Icon::RETURN_TRACK;
        case 5:
            return Icon::MASTER_TRACK;
        default:
            return Icon::TRACK_AUDIO;
        }
    }

} // namespace Bitwig
