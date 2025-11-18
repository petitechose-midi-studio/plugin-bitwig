#include "TransportBar.hpp"
#include "../theme/BitwigTheme.hpp"
#include "resource/common/ui/font/binary_font_buffer.hpp"
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
    if (play_btn_) {
        lv_opa_t opa = playing ? LV_OPA_COVER : LV_OPA_30;
        lv_obj_set_style_opa(play_btn_, opa, 0);
    }
}

void TransportBar::setRecord(bool recording) {
    if (record_btn_) {
        lv_opa_t opa = recording ? LV_OPA_COVER : LV_OPA_30;
        lv_obj_set_style_opa(record_btn_, opa, 0);
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
    lv_obj_set_style_pad_all(container_, 4, 0);
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
    lv_obj_set_grid_cell(midi_container, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
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
                         LV_GRID_ALIGN_CENTER,
                         0,
                         1);
    lv_obj_set_style_bg_opa(transport_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(transport_container, 0, 0);
    lv_obj_set_style_pad_all(transport_container, 0, 0);
    lv_obj_set_style_pad_gap(transport_container, 6, 0);
    lv_obj_set_flex_flow(transport_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(transport_container,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    play_btn_ = lv_canvas_create(transport_container);
    lv_obj_set_size(play_btn_, 13, 16);
    static uint8_t play_canvas_buf[LV_CANVAS_BUF_SIZE(13, 16, 16, 1)];
    lv_canvas_set_buffer(play_btn_, play_canvas_buf, 13, 16, LV_COLOR_FORMAT_RGB565);
    drawPlayTriangle();
    lv_obj_set_style_opa(play_btn_, LV_OPA_30, 0);  // Initial state: stopped

    stop_btn_ = lv_obj_create(transport_container);
    lv_obj_set_size(stop_btn_, 14, 14);
    lv_obj_set_style_bg_color(stop_btn_, COLOR_INACTIVE, 0);
    lv_obj_set_style_bg_opa(stop_btn_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(stop_btn_, 0, 0);
    lv_obj_set_style_radius(stop_btn_, 2, 0);

    record_btn_ = lv_obj_create(transport_container);
    lv_obj_set_size(record_btn_, 14, 14);
    lv_obj_set_style_bg_color(record_btn_, COLOR_RED, 0);
    lv_obj_set_style_bg_opa(record_btn_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(record_btn_, 0, 0);
    lv_obj_set_style_radius(record_btn_, LV_RADIUS_CIRCLE, 0);  // 100% radius = circle
    lv_obj_set_style_opa(record_btn_, LV_OPA_30, 0);  // Initial state: off
}

void TransportBar::createTempoDisplay() {
    bpm_label_ = lv_label_create(container_);
    lv_obj_set_grid_cell(bpm_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_label_set_text(bpm_label_, "120.00");
    lv_obj_set_style_text_color(bpm_label_, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(bpm_label_, fonts.tempo_label, 0);
}

void TransportBar::drawPlayTriangle() {
    lv_canvas_fill_bg(play_btn_, COLOR_BACKGROUND, LV_OPA_COVER);

    lv_layer_t layer;
    lv_canvas_init_layer(play_btn_, &layer);

    lv_draw_triangle_dsc_t triangle_dsc;
    lv_draw_triangle_dsc_init(&triangle_dsc);
    triangle_dsc.p[0] = {1, 1};
    triangle_dsc.p[1] = {1, 16};
    triangle_dsc.p[2] = {13, 8};
    triangle_dsc.color = COLOR_GREEN;
    triangle_dsc.opa = LV_OPA_COVER;

    lv_draw_triangle(&layer, &triangle_dsc);
    lv_canvas_finish_layer(play_btn_, &layer);
}
