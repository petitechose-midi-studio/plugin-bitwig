#include "TrackTitleItem.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;
namespace style = oc::ui::lvgl::style;

namespace bitwig {

TrackTitleItem::TrackTitleItem(lv_obj_t *parent, bool withMuteSolo, lv_coord_t barHeight)
    : parent_(parent), bar_height_(barHeight), has_mute_solo_(withMuteSolo) {
    if (!parent_) return;

    color_bar_ = lv_obj_create(parent_);
    if (!color_bar_) return;

    lv_obj_set_size(color_bar_, Layout::COLOR_BAR_WIDTH, bar_height_);
    lv_obj_set_style_radius(color_bar_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(color_bar_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(color_bar_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(color_bar_, Opacity::FULL, LV_STATE_DEFAULT);
    lv_obj_clear_flag(color_bar_, LV_OBJ_FLAG_SCROLLABLE);

    type_icon_ = lv_label_create(parent_);
    if (type_icon_) {
        style::apply(type_icon_).textColor(Color::TEXT_PRIMARY);
        lv_obj_set_style_text_opa(type_icon_, Opacity::SUBTLE, LV_STATE_DEFAULT);
    }

    label_ = lv_label_create(parent_);
    if (label_) {
        style::apply(label_).textColor(Color::TEXT_LIGHT);
        lv_obj_set_style_text_font(label_, bitwig_fonts.track_label, LV_STATE_DEFAULT);
    }

    if (has_mute_solo_ && label_) {
        lv_obj_set_flex_grow(label_, 1);

        mute_icon_ = lv_label_create(parent_);
        if (mute_icon_) {
            Icon::set(mute_icon_, Icon::MUTE);
            style::apply(mute_icon_).textColor(Color::TRACK_MUTE);
            lv_obj_set_style_text_opa(mute_icon_, Opacity::HINT, LV_STATE_DEFAULT);
        }

        solo_icon_ = lv_label_create(parent_);
        if (solo_icon_) {
            Icon::set(solo_icon_, Icon::SOLO);
            style::apply(solo_icon_).textColor(Color::TRACK_SOLO);
            lv_obj_set_style_text_opa(solo_icon_, Opacity::HINT, LV_STATE_DEFAULT);
        }

        level_bar_ = std::make_unique<LevelBar>(parent_, Layout::LEVEL_BAR_WIDTH, bar_height_);
    }
}

TrackTitleItem::~TrackTitleItem() {
    if (color_bar_) {
        lv_obj_delete(color_bar_);
        color_bar_ = nullptr;
    }
    if (type_icon_) {
        lv_obj_delete(type_icon_);
        type_icon_ = nullptr;
    }
    if (label_) {
        lv_obj_delete(label_);
        label_ = nullptr;
    }
    if (mute_icon_) {
        lv_obj_delete(mute_icon_);
        mute_icon_ = nullptr;
    }
    if (solo_icon_) {
        lv_obj_delete(solo_icon_);
        solo_icon_ = nullptr;
    }
}

void TrackTitleItem::render(const TrackTitleItemProps &props) {
    if (label_) {
        lv_label_set_text(label_, props.name ? props.name : "");
        uint32_t labelColor = props.highlighted ? Color::TEXT_PRIMARY : Color::INACTIVE_LIGHTER;
        style::apply(label_).textColor(labelColor);
    }

    if (color_bar_) {
        lv_obj_set_style_bg_color(color_bar_, lv_color_hex(props.color), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(color_bar_, props.hideIndicators ? Opacity::HIDDEN : Opacity::FULL,
                                LV_STATE_DEFAULT);
    }

    if (type_icon_) {
        Icon::set(type_icon_, getTrackTypeIcon(props.trackType), Icon::Size::M);
        lv_obj_set_style_text_opa(
            type_icon_, props.hideIndicators ? Opacity::HIDDEN : Opacity::SUBTLE, LV_STATE_DEFAULT);
    }

    if (level_bar_) {
        level_bar_->render({.value = props.level,
                            .color = props.color,
                            .opacity = static_cast<lv_opa_t>(
                                props.hideIndicators ? Opacity::HIDDEN : Opacity::DIMMED)});
    }

    if (!props.hideIndicators) {
        updateIndicatorOpacity(props.isMuted, props.isSoloed, props.highlighted);
    } else {
        if (mute_icon_) lv_obj_set_style_text_opa(mute_icon_, Opacity::HIDDEN, LV_STATE_DEFAULT);
        if (solo_icon_) lv_obj_set_style_text_opa(solo_icon_, Opacity::HIDDEN, LV_STATE_DEFAULT);
    }
}

void TrackTitleItem::updateIndicatorOpacity(bool isMuted, bool isSoloed, bool highlighted) {
    if (!has_mute_solo_) return;

    if (mute_icon_) {
        uint8_t opa = isMuted ? Opacity::FULL : (highlighted ? Opacity::FADED : Opacity::HINT);
        lv_obj_set_style_text_opa(mute_icon_, opa, LV_STATE_DEFAULT);
    }

    if (solo_icon_) {
        uint8_t opa = isSoloed ? Opacity::FULL : (highlighted ? Opacity::FADED : Opacity::HINT);
        lv_obj_set_style_text_opa(solo_icon_, opa, LV_STATE_DEFAULT);
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

}  // namespace bitwig
