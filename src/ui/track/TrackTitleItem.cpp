#include "TrackTitleItem.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/BitwigIcons.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;
namespace style = oc::ui::lvgl::style;
namespace icons = bitwig::icons;

namespace bitwig::ui {

TrackTitleItem::TrackTitleItem(lv_obj_t *parent, bool withMuteSolo, lv_coord_t barHeight)
    : bar_height_(barHeight), has_mute_solo_(withMuteSolo) {
    if (!parent) return;

    // Create container that holds all child elements
    container_ = lv_obj_create(parent);
    if (!container_) return;

    // Style container as transparent flex row
    lv_obj_set_size(container_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(container_, 4, LV_STATE_DEFAULT);
    lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);

    color_bar_ = lv_obj_create(container_);
    if (!color_bar_) return;

    lv_obj_set_size(color_bar_, layout::COLOR_BAR_WIDTH, bar_height_);
    lv_obj_set_style_radius(color_bar_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(color_bar_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(color_bar_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(color_bar_, opacity::FULL, LV_STATE_DEFAULT);
    lv_obj_clear_flag(color_bar_, LV_OBJ_FLAG_SCROLLABLE);

    type_icon_ = lv_label_create(container_);
    if (type_icon_) {
        style::apply(type_icon_).textColor(color::TEXT_PRIMARY);
        lv_obj_set_style_text_opa(type_icon_, opacity::SUBTLE, LV_STATE_DEFAULT);
    }

    // Use framework Label widget with auto-scroll for overflow text
    // ownsLvglObjects(false) lets LVGL parent-child handle deletion
    label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    label_->alignment(LV_TEXT_ALIGN_LEFT)
        .color(color::TEXT_LIGHT)
        .font(bitwig_fonts.track_label)
        .ownsLvglObjects(false);

    if (has_mute_solo_ && label_) {
        label_->flexGrow(true);

        mute_icon_ = lv_label_create(container_);
        if (mute_icon_) {
            icons::set(mute_icon_, icons::CHANNEL_MUTE);
            style::apply(mute_icon_).textColor(color::TRACK_MUTE);
            lv_obj_set_style_text_opa(mute_icon_, opacity::HINT, LV_STATE_DEFAULT);
        }

        solo_icon_ = lv_label_create(container_);
        if (solo_icon_) {
            icons::set(solo_icon_, icons::CHANNEL_SOLO);
            style::apply(solo_icon_).textColor(color::TRACK_SOLO);
            lv_obj_set_style_text_opa(solo_icon_, opacity::HINT, LV_STATE_DEFAULT);
        }

        level_bar_ = std::make_unique<LevelBar>(container_, layout::LEVEL_BAR_WIDTH, bar_height_);
    }
}

TrackTitleItem::~TrackTitleItem() {
    // Delete container - this also deletes all children (color_bar_, type_icon_, etc.)
    // label_ unique_ptr is cleaned up automatically (ownsLvglObjects=false means LVGL handles it)
    // level_bar_ unique_ptr is cleaned up automatically
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void TrackTitleItem::render(const TrackTitleItemProps &props) {
    if (label_) {
        label_->setText(props.name ? props.name : "");
        uint32_t labelColor = props.highlighted ? color::TEXT_PRIMARY : color::INACTIVE_LIGHTER;
        label_->color(labelColor);
    }

    if (color_bar_) {
        lv_obj_set_style_bg_color(color_bar_, lv_color_hex(props.color), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(color_bar_, props.hideIndicators ? opacity::HIDDEN : opacity::FULL,
                                LV_STATE_DEFAULT);
    }

    if (type_icon_) {
        icons::set(type_icon_, getTrackTypeIcon(props.trackType), icons::Size::M);
        lv_obj_set_style_text_opa(
            type_icon_, props.hideIndicators ? opacity::HIDDEN : opacity::SUBTLE, LV_STATE_DEFAULT);
    }

    if (level_bar_) {
        level_bar_->render({.value = props.level,
                            .color = props.color,
                            .opacity = static_cast<lv_opa_t>(
                                props.hideIndicators ? opacity::HIDDEN : opacity::DIMMED)});
    }

    if (!props.hideIndicators) {
        updateIndicatorOpacity(props.isMuted, props.isSoloed, props.highlighted);
    } else {
        if (mute_icon_) lv_obj_set_style_text_opa(mute_icon_, opacity::HIDDEN, LV_STATE_DEFAULT);
        if (solo_icon_) lv_obj_set_style_text_opa(solo_icon_, opacity::HIDDEN, LV_STATE_DEFAULT);
    }
}

void TrackTitleItem::hide() {
    if (container_) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    visible_ = false;
}

void TrackTitleItem::show() {
    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    visible_ = true;
}

void TrackTitleItem::setHighlighted(bool highlighted) {
    highlighted_ = highlighted;
    if (label_) {
        uint32_t labelColor = highlighted ? color::TEXT_PRIMARY : color::INACTIVE_LIGHTER;
        label_->color(labelColor);
    }
}

void TrackTitleItem::updateIndicatorOpacity(bool isMuted, bool isSoloed, bool highlighted) {
    if (!has_mute_solo_) return;

    if (mute_icon_) {
        uint8_t opa = isMuted ? opacity::FULL : (highlighted ? opacity::FADED : opacity::HINT);
        lv_obj_set_style_text_opa(mute_icon_, opa, LV_STATE_DEFAULT);
    }

    if (solo_icon_) {
        uint8_t opa = isSoloed ? opacity::FULL : (highlighted ? opacity::FADED : opacity::HINT);
        lv_obj_set_style_text_opa(solo_icon_, opa, LV_STATE_DEFAULT);
    }
}

const char *TrackTitleItem::getTrackTypeIcon(TrackType trackType) {
    switch (trackType) {
        case TrackType::AUDIO: return icons::TRACK_AUDIO;
        case TrackType::INSTRUMENT: return icons::TRACK_INSTRUMENT;
        case TrackType::HYBRID: return icons::TRACK_HYBRID;
        case TrackType::GROUP: return icons::BROWSER_DIRECTORY;
        case TrackType::EFFECT: return icons::TRACK_RETURN;
        case TrackType::MASTER: return icons::TRACK_MASTER;
        default: return icons::TRACK_AUDIO;
    }
}

}  // namespace bitwig::ui
