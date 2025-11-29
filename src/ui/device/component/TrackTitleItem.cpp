#include "TrackTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"
#include "util/TextUtils.hpp"

using namespace Theme;

namespace Bitwig
{

    TrackTitleItem::TrackTitleItem(lv_obj_t *parent, bool withMuteSolo, lv_coord_t barHeight)
        : parent_(parent), bar_height_(barHeight), has_mute_solo_(withMuteSolo)
    {
        // Lazy init - don't create LVGL widgets here
    }

    TrackTitleItem::~TrackTitleItem()
    {
        // Don't delete anything - LVGL parent handles cleanup
    }

    void TrackTitleItem::ensureCreated()
    {
        if (color_bar_ || !parent_) return;

        // Color bar
        color_bar_ = lv_obj_create(parent_);
        if (!color_bar_) return;

        lv_obj_set_size(color_bar_, 4, bar_height_);
        lv_obj_set_style_radius(color_bar_, 0, 0);
        lv_obj_set_style_border_width(color_bar_, 0, 0);
        lv_obj_set_style_pad_all(color_bar_, 0, 0);
        lv_obj_set_style_bg_color(color_bar_, lv_color_hex(track_color_), 0);
        lv_obj_set_style_bg_opa(color_bar_, LV_OPA_COVER, 0);
        lv_obj_clear_flag(color_bar_, LV_OBJ_FLAG_SCROLLABLE);

        // Track type icon
        type_icon_ = lv_label_create(parent_);
        if (type_icon_)
        {
            Icon::set(type_icon_, getTrackTypeIcon(track_type_), Icon::S14);
            lv_obj_set_style_text_color(type_icon_, lv_color_hex(Color::TEXT_PRIMARY), 0);
            lv_obj_set_style_text_opa(type_icon_, LV_OPA_70, 0);
        }

        // Track name label
        label_ = lv_label_create(parent_);
        if (label_)
        {
            lv_obj_set_style_text_color(label_, lv_color_hex(Color::TEXT_LIGHT), 0);
            lv_obj_set_style_text_font(label_, bitwig_fonts.track_label, 0);
            lv_label_set_text(label_, pending_name_.c_str());
        }

        // Mute/solo icons and level bar (optional)
        if (has_mute_solo_ && label_)
        {
            lv_obj_set_flex_grow(label_, 1);

            mute_icon_ = lv_label_create(parent_);
            if (mute_icon_)
            {
                Icon::set(mute_icon_, Icon::MUTE);
                lv_obj_set_style_text_color(mute_icon_, lv_color_hex(Color::TRACK_MUTE), 0);
                lv_obj_set_style_text_opa(mute_icon_, LV_OPA_20, 0);
            }

            solo_icon_ = lv_label_create(parent_);
            if (solo_icon_)
            {
                Icon::set(solo_icon_, Icon::SOLO);
                lv_obj_set_style_text_color(solo_icon_, lv_color_hex(Color::TRACK_SOLO), 0);
                lv_obj_set_style_text_opa(solo_icon_, LV_OPA_20, 0);
            }

            // Level bar - already lazy init internally
            level_bar_ = std::make_unique<LevelBar>(parent_, 60, bar_height_);
            level_bar_->setColor(track_color_);
            level_bar_->setOpacity(LV_OPA_50);
            level_bar_->setValue(pending_level_);
        }

        // Apply pending states
        if (indicators_hidden_) hideIndicators();
        updateIndicatorOpacity();
    }

    void TrackTitleItem::setName(const std::string &name)
    {
        pending_name_ = TextUtils::sanitizeText(name.c_str()).c_str();

        ensureCreated();
        if (label_)
        {
            lv_label_set_text(label_, pending_name_.c_str());
        }
    }

    void TrackTitleItem::setColor(uint32_t color)
    {
        track_color_ = color;

        ensureCreated();
        if (color_bar_)
        {
            lv_obj_set_style_bg_color(color_bar_, lv_color_hex(color), 0);
        }
        if (level_bar_)
        {
            level_bar_->setColor(color);
        }
    }

    void TrackTitleItem::setType(uint8_t trackType)
    {
        track_type_ = trackType;

        ensureCreated();
        if (type_icon_)
        {
            Icon::set(type_icon_, getTrackTypeIcon(trackType), Icon::S14);
        }
    }

    void TrackTitleItem::setMuteState(bool isMuted)
    {
        is_muted_ = isMuted;
        ensureCreated();
        updateIndicatorOpacity();
    }

    void TrackTitleItem::setSoloState(bool isSoloed)
    {
        is_soloed_ = isSoloed;
        ensureCreated();
        updateIndicatorOpacity();
    }

    void TrackTitleItem::setLevel(float normalizedValue)
    {
        pending_level_ = normalizedValue;

        ensureCreated();
        if (level_bar_)
        {
            level_bar_->setValue(normalizedValue);
        }
    }

    void TrackTitleItem::setHighlighted(bool highlighted)
    {
        is_highlighted_ = highlighted;

        ensureCreated();
        updateIndicatorOpacity();

        if (label_)
        {
            lv_color_t labelColor = highlighted ? lv_color_hex(Color::TEXT_PRIMARY)
                                                : lv_color_hex(Color::INACTIVE_LIGHTER);
            lv_obj_set_style_text_color(label_, labelColor, 0);
        }
    }

    void TrackTitleItem::hideIndicators()
    {
        indicators_hidden_ = true;

        if (mute_icon_)
        {
            lv_obj_set_style_text_opa(mute_icon_, LV_OPA_TRANSP, 0);
        }
        if (solo_icon_)
        {
            lv_obj_set_style_text_opa(solo_icon_, LV_OPA_TRANSP, 0);
        }
        if (color_bar_)
        {
            lv_obj_set_style_bg_opa(color_bar_, LV_OPA_TRANSP, 0);
        }
        if (type_icon_)
        {
            lv_obj_set_style_text_opa(type_icon_, LV_OPA_TRANSP, 0);
        }
        if (level_bar_)
        {
            level_bar_->setOpacity(LV_OPA_TRANSP);
        }
    }

    void TrackTitleItem::updateIndicatorOpacity()
    {
        if (!has_mute_solo_ || indicators_hidden_)
            return;

        if (mute_icon_)
        {
            lv_opa_t mute_opa = is_muted_ ? LV_OPA_COVER : (is_highlighted_ ? LV_OPA_40 : LV_OPA_20);
            lv_obj_set_style_text_opa(mute_icon_, mute_opa, 0);
        }

        if (solo_icon_)
        {
            lv_opa_t solo_opa = is_soloed_ ? LV_OPA_COVER : (is_highlighted_ ? LV_OPA_40 : LV_OPA_20);
            lv_obj_set_style_text_opa(solo_icon_, solo_opa, 0);
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
