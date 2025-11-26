#include "TransportBar.hpp"
#include "../theme/BitwigTheme.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/bitwig_icons.hpp"
#include "log/Macros.hpp"

namespace {
const lv_color_t COLOR_BACKGROUND = lv_color_hex(Theme::Color::BACKGROUND_FILL);
const lv_color_t COLOR_TEXT = lv_color_hex(Theme::Color::TEXT_LIGHT);
const lv_color_t COLOR_INACTIVE = lv_color_hex(Theme::Color::DATA_INACTIVE);
const lv_color_t COLOR_ACTIVE = lv_color_hex(Theme::Color::DATA_ACTIVE);
const lv_color_t COLOR_GREEN = lv_color_hex(Theme::Color::MACRO_5);
const lv_color_t COLOR_RED = lv_color_hex(Theme::Color::MACRO_1);
const lv_color_t COLOR_MIDI_INDICATOR = lv_color_hex(Theme::Color::KNOB_VALUE_RIBBON);
}  // namespace

TransportBar::TransportBar(lv_obj_t* parent) {
    createContainer(parent);
    createTransportControls();
    createTempoDisplay();
    show();
}

TransportBar::~TransportBar() {
    if (container_) {
        lv_obj_del(container_);
    }
}

void TransportBar::setPlay(bool playing) {
    if (play_icon_) {
        lv_color_t color = playing ? COLOR_GREEN : COLOR_INACTIVE;
        lv_obj_set_style_text_color(play_icon_, color, 0);
    }
}

void TransportBar::setRecord(bool recording) {
    if (record_icon_) {
        lv_color_t color = recording ? COLOR_RED : COLOR_INACTIVE;
        lv_obj_set_style_text_color(record_icon_, color, 0);
    }
}

void TransportBar::setTempo(float bpm) {
    if (bpm_label_) {
        lv_label_set_text_fmt(bpm_label_, "%.2f", static_cast<double>(bpm));
    }
}

void TransportBar::show() {
    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
}

void TransportBar::hide() {
    if (container_) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
}

bool TransportBar::isVisible() const {
    if (container_) {
        return !lv_obj_has_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    return false;
}

void TransportBar::createContainer(lv_obj_t* parent) {
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, LV_PCT(100), 20);
    lv_obj_set_style_bg_color(container_, COLOR_BACKGROUND, 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_pad_all(container_, 0, 0);
    lv_obj_set_scrollbar_mode(container_, LV_SCROLLBAR_MODE_OFF);

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1),
                                         LV_GRID_FR(1),
                                         LV_GRID_FR(1),
                                         LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
}

void TransportBar::createTransportControls() {
    lv_obj_t* midi_container = lv_obj_create(container_);
    lv_obj_set_grid_cell(midi_container, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_set_style_bg_opa(midi_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(midi_container, 0, 0);
    lv_obj_set_style_pad_all(midi_container, 0, 0);
    lv_obj_set_style_pad_gap(midi_container, 4, 0);
    lv_obj_set_flex_flow(midi_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(midi_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    midi_in_indicator_ = std::make_unique<ButtonIndicator>(midi_container, 12);
    midi_in_indicator_->setCustomColor(ButtonIndicator::OFF, COLOR_MIDI_INDICATOR);
    midi_in_indicator_->setCustomOpacity(ButtonIndicator::OFF, LV_OPA_COVER);
    midi_in_indicator_->setCustomColor(ButtonIndicator::ACTIVE, COLOR_ACTIVE);
    midi_in_indicator_->setCustomOpacity(ButtonIndicator::ACTIVE, LV_OPA_COVER);
    midi_in_indicator_->setState(ButtonIndicator::OFF);

    midi_out_indicator_ = std::make_unique<ButtonIndicator>(midi_container, 12);
    midi_out_indicator_->setCustomColor(ButtonIndicator::OFF, COLOR_MIDI_INDICATOR);
    midi_out_indicator_->setCustomOpacity(ButtonIndicator::OFF, LV_OPA_COVER);
    midi_out_indicator_->setCustomColor(ButtonIndicator::ACTIVE, COLOR_ACTIVE);
    midi_out_indicator_->setCustomOpacity(ButtonIndicator::ACTIVE, LV_OPA_COVER);
    midi_out_indicator_->setState(ButtonIndicator::OFF);

    lv_obj_t* transport_container = lv_obj_create(container_);
    lv_obj_set_grid_cell(transport_container,
                         LV_GRID_ALIGN_CENTER,
                         1,
                         1,
                         LV_GRID_ALIGN_STRETCH,
                         0,
                         1);
    lv_obj_set_style_bg_opa(transport_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(transport_container, 0, 0);
    lv_obj_set_style_pad_all(transport_container, 0, 0);
    lv_obj_set_style_pad_gap(transport_container, 4, 0);
    lv_obj_set_flex_flow(transport_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(transport_container,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    // Play icon
    play_icon_ = lv_label_create(transport_container);
    lv_label_set_text(play_icon_, BitwigIcon::TRANSPORT_PLAY);
    lv_obj_set_style_text_font(play_icon_, bitwig_fonts.icons, 0);
    lv_obj_set_style_text_color(play_icon_, COLOR_INACTIVE, 0);

    // Stop icon
    stop_icon_ = lv_label_create(transport_container);
    lv_label_set_text(stop_icon_, BitwigIcon::TRANSPORT_STOP);
    lv_obj_set_style_text_font(stop_icon_, bitwig_fonts.icons, 0);
    lv_obj_set_style_text_color(stop_icon_, COLOR_INACTIVE, 0);

    // Record icon
    record_icon_ = lv_label_create(transport_container);
    lv_label_set_text(record_icon_, BitwigIcon::TRANSPORT_RECORD);
    lv_obj_set_style_text_font(record_icon_, bitwig_fonts.icons, 0);
    lv_obj_set_style_text_color(record_icon_, COLOR_INACTIVE, 0);
}

void TransportBar::createTempoDisplay() {
    bpm_label_ = lv_label_create(container_);
    lv_obj_set_grid_cell(bpm_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_label_set_text(bpm_label_, "120.00");
    lv_obj_set_style_text_color(bpm_label_, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(bpm_label_, fonts.tempo_label, 0);
}
