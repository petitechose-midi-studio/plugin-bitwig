#include "TrackTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"

using namespace Theme;

namespace Bitwig
{

TrackTitleItem::TrackTitleItem(lv_obj_t *parent, bool withMuteSolo, lv_coord_t barHeight)
    : parent_(parent), bar_height_(barHeight), has_mute_solo_(withMuteSolo)
{
}

TrackTitleItem::~TrackTitleItem()
{
    // Delete LVGL objects (children of parent button)
    if (color_bar_)
        lv_obj_delete(color_bar_);
    if (type_icon_)
        lv_obj_delete(type_icon_);
    if (label_)
        lv_obj_delete(label_);
    if (mute_icon_)
        lv_obj_delete(mute_icon_);
    if (solo_icon_)
        lv_obj_delete(solo_icon_);
    // level_bar_ is managed by unique_ptr
}

void TrackTitleItem::ensureCreated()
{
    if (color_bar_ || !parent_)
        return;

    color_bar_ = lv_obj_create(parent_);
    if (!color_bar_)
        return;

    lv_obj_set_size(color_bar_, 4, bar_height_);
    lv_obj_set_style_radius(color_bar_, 0, 0);
    lv_obj_set_style_border_width(color_bar_, 0, 0);
    lv_obj_set_style_pad_all(color_bar_, 0, 0);
    lv_obj_set_style_bg_opa(color_bar_, LV_OPA_COVER, 0);
    lv_obj_clear_flag(color_bar_, LV_OBJ_FLAG_SCROLLABLE);

    type_icon_ = lv_label_create(parent_);
    if (type_icon_)
    {
        lv_obj_set_style_text_color(type_icon_, lv_color_hex(Color::TEXT_PRIMARY), 0);
        lv_obj_set_style_text_opa(type_icon_, LV_OPA_70, 0);
    }

    label_ = lv_label_create(parent_);
    if (label_)
    {
        lv_obj_set_style_text_color(label_, lv_color_hex(Color::TEXT_LIGHT), 0);
        lv_obj_set_style_text_font(label_, bitwig_fonts.track_label, 0);
    }

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

        level_bar_ = std::make_unique<LevelBar>(parent_, 60, bar_height_);
    }
}

void TrackTitleItem::render(const TrackTitleItemProps &props)
{
    ensureCreated();

    // Name
    if (label_)
    {
        lv_label_set_text(label_, props.name ? props.name : "");
        lv_color_t labelColor = props.highlighted ? lv_color_hex(Color::TEXT_PRIMARY)
                                                  : lv_color_hex(Color::INACTIVE_LIGHTER);
        lv_obj_set_style_text_color(label_, labelColor, 0);
    }

    // Color bar
    if (color_bar_)
    {
        lv_obj_set_style_bg_color(color_bar_, lv_color_hex(props.color), 0);
        lv_obj_set_style_bg_opa(color_bar_, props.hideIndicators ? LV_OPA_TRANSP : LV_OPA_COVER, 0);
    }

    // Type icon
    if (type_icon_)
    {
        Icon::set(type_icon_, getTrackTypeIcon(props.trackType), Icon::S14);
        lv_obj_set_style_text_opa(type_icon_, props.hideIndicators ? LV_OPA_TRANSP : LV_OPA_70, 0);
    }

    // Level bar
    if (level_bar_)
    {
        level_bar_->render({
            .value = props.level,
            .color = props.color,
            .opacity = static_cast<lv_opa_t>(props.hideIndicators ? LV_OPA_TRANSP : LV_OPA_50)});
    }

    // Mute/Solo indicators
    if (!props.hideIndicators)
    {
        updateIndicatorOpacity(props.isMuted, props.isSoloed, props.highlighted);
    }
    else
    {
        if (mute_icon_)
            lv_obj_set_style_text_opa(mute_icon_, LV_OPA_TRANSP, 0);
        if (solo_icon_)
            lv_obj_set_style_text_opa(solo_icon_, LV_OPA_TRANSP, 0);
    }
}

void TrackTitleItem::updateIndicatorOpacity(bool isMuted, bool isSoloed, bool highlighted)
{
    if (!has_mute_solo_)
        return;

    if (mute_icon_)
    {
        lv_opa_t mute_opa = isMuted ? LV_OPA_COVER : (highlighted ? LV_OPA_40 : LV_OPA_20);
        lv_obj_set_style_text_opa(mute_icon_, mute_opa, 0);
    }

    if (solo_icon_)
    {
        lv_opa_t solo_opa = isSoloed ? LV_OPA_COVER : (highlighted ? LV_OPA_40 : LV_OPA_20);
        lv_obj_set_style_text_opa(solo_icon_, solo_opa, 0);
    }
}

const char *TrackTitleItem::getTrackTypeIcon(uint8_t trackType)
{
    switch (trackType)
    {
    case 0: return Icon::TRACK_AUDIO;
    case 1: return Icon::TRACK_INSTRUMENT;
    case 2: return Icon::TRACK_HYBRID;
    case 3: return Icon::DIRECTORY;
    case 4: return Icon::RETURN_TRACK;
    case 5: return Icon::MASTER_TRACK;
    default: return Icon::TRACK_AUDIO;
    }
}

} // namespace Bitwig
