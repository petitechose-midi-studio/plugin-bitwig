#include "TrackTitleItem.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"

using namespace Theme;

namespace Bitwig {

TrackTitleItem::TrackTitleItem(lv_obj_t *parent, bool withMuteSolo, lv_coord_t barHeight)
    : parent_(parent), bar_height_(barHeight), has_mute_solo_(withMuteSolo) {}

TrackTitleItem::~TrackTitleItem() {
    if (color_bar_) lv_obj_delete(color_bar_);
    if (type_icon_) lv_obj_delete(type_icon_);
    if (label_) lv_obj_delete(label_);
    if (mute_icon_) lv_obj_delete(mute_icon_);
    if (solo_icon_) lv_obj_delete(solo_icon_);
}

void TrackTitleItem::ensureCreated() {
    if (color_bar_ || !parent_)
        return;

    color_bar_ = lv_obj_create(parent_);
    if (!color_bar_)
        return;

    lv_obj_set_size(color_bar_, Layout::COLOR_BAR_WIDTH, bar_height_);
    lv_obj_set_style_radius(color_bar_, 0, 0);
    lv_obj_set_style_border_width(color_bar_, 0, 0);
    lv_obj_set_style_pad_all(color_bar_, 0, 0);
    lv_obj_set_style_bg_opa(color_bar_, Opacity::FULL, 0);
    lv_obj_clear_flag(color_bar_, LV_OBJ_FLAG_SCROLLABLE);

    type_icon_ = lv_label_create(parent_);
    if (type_icon_) {
        Style::setTextColor(type_icon_, Color::TEXT_PRIMARY);
        lv_obj_set_style_text_opa(type_icon_, Opacity::SUBTLE, 0);
    }

    label_ = lv_label_create(parent_);
    if (label_) {
        Style::setTextColor(label_, Color::TEXT_LIGHT);
        lv_obj_set_style_text_font(label_, bitwig_fonts.track_label, 0);
    }

    if (has_mute_solo_ && label_) {
        lv_obj_set_flex_grow(label_, 1);

        mute_icon_ = lv_label_create(parent_);
        if (mute_icon_) {
            Icon::set(mute_icon_, Icon::MUTE);
            Style::setTextColor(mute_icon_, Color::TRACK_MUTE);
            lv_obj_set_style_text_opa(mute_icon_, Opacity::HINT, 0);
        }

        solo_icon_ = lv_label_create(parent_);
        if (solo_icon_) {
            Icon::set(solo_icon_, Icon::SOLO);
            Style::setTextColor(solo_icon_, Color::TRACK_SOLO);
            lv_obj_set_style_text_opa(solo_icon_, Opacity::HINT, 0);
        }

        level_bar_ = std::make_unique<LevelBar>(parent_, Layout::LEVEL_BAR_WIDTH, bar_height_);
    }
}

void TrackTitleItem::render(const TrackTitleItemProps &props) {
    ensureCreated();

    if (label_) {
        lv_label_set_text(label_, props.name ? props.name : "");
        uint32_t labelColor = props.highlighted ? Color::TEXT_PRIMARY : Color::INACTIVE_LIGHTER;
        Style::setTextColor(label_, labelColor);
    }

    if (color_bar_) {
        lv_obj_set_style_bg_color(color_bar_, lv_color_hex(props.color), 0);
        lv_obj_set_style_bg_opa(color_bar_, props.hideIndicators ? Opacity::HIDDEN : Opacity::FULL, 0);
    }

    if (type_icon_) {
        Icon::set(type_icon_, getTrackTypeIcon(props.trackType), Icon::M);
        lv_obj_set_style_text_opa(type_icon_, props.hideIndicators ? Opacity::HIDDEN : Opacity::SUBTLE, 0);
    }

    if (level_bar_) {
        level_bar_->render({
            .value = props.level,
            .color = props.color,
            .opacity = static_cast<lv_opa_t>(props.hideIndicators ? Opacity::HIDDEN : Opacity::DIMMED)});
    }

    if (!props.hideIndicators) {
        updateIndicatorOpacity(props.isMuted, props.isSoloed, props.highlighted);
    } else {
        if (mute_icon_) lv_obj_set_style_text_opa(mute_icon_, Opacity::HIDDEN, 0);
        if (solo_icon_) lv_obj_set_style_text_opa(solo_icon_, Opacity::HIDDEN, 0);
    }
}

void TrackTitleItem::updateIndicatorOpacity(bool isMuted, bool isSoloed, bool highlighted) {
    if (!has_mute_solo_)
        return;

    if (mute_icon_) {
        uint8_t opa = isMuted ? Opacity::FULL : (highlighted ? Opacity::FADED : Opacity::HINT);
        lv_obj_set_style_text_opa(mute_icon_, opa, 0);
    }

    if (solo_icon_) {
        uint8_t opa = isSoloed ? Opacity::FULL : (highlighted ? Opacity::FADED : Opacity::HINT);
        lv_obj_set_style_text_opa(solo_icon_, opa, 0);
    }
}

const char *TrackTitleItem::getTrackTypeIcon(uint8_t trackType) {
    switch (trackType) {
    case 0: return Icon::TRACK_AUDIO;
    case 1: return Icon::TRACK_INSTRUMENT;
    case 2: return Icon::TRACK_HYBRID;
    case 3: return Icon::DIRECTORY;
    case 4: return Icon::RETURN_TRACK;
    case 5: return Icon::MASTER_TRACK;
    default: return Icon::TRACK_AUDIO;
    }
}

}  // namespace Bitwig
