#include "TransportBar.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/icon.hpp"

using namespace Theme;

namespace {
const lv_color_t COLOR_BACKGROUND = lv_color_hex(Color::BACKGROUND_FILL);
const lv_color_t COLOR_TEXT = lv_color_hex(Color::TEXT_LIGHT);
const lv_color_t COLOR_INACTIVE = lv_color_hex(Color::DATA_INACTIVE);
const lv_color_t COLOR_ACTIVE = lv_color_hex(Color::DATA_ACTIVE);
const lv_color_t COLOR_PLAY = lv_color_hex(Color::MACRO_5);
const lv_color_t COLOR_RECORD = lv_color_hex(Color::MACRO_1);
const lv_color_t COLOR_MIDI = lv_color_hex(Color::KNOB_VALUE_RIBBON);
}  // namespace

namespace Bitwig {

TransportBar::TransportBar(lv_obj_t* parent)
    : parent_(parent) {
    if (!parent_) return;

    createContainer(parent_);
    createTransportControls();
    createTempoDisplay();
}

TransportBar::~TransportBar() {
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void TransportBar::render(const TransportBarProps& props) {
    if (!container_) return;

    // Play state
    if (play_icon_) {
        lv_obj_set_style_text_color(play_icon_, props.playing ? COLOR_PLAY : COLOR_INACTIVE, 0);
    }

    // Record state
    if (record_icon_) {
        lv_obj_set_style_text_color(record_icon_, props.recording ? COLOR_RECORD : COLOR_INACTIVE, 0);
    }

    // Tempo
    if (bpm_label_) {
        lv_label_set_text_fmt(bpm_label_, "%.2f", static_cast<double>(props.tempo));
    }

    // MIDI indicators
    if (midi_in_indicator_) {
        midi_in_indicator_->setState(props.midiInActive ? ButtonIndicator::ACTIVE : ButtonIndicator::OFF);
    }
    if (midi_out_indicator_) {
        midi_out_indicator_->setState(props.midiOutActive ? ButtonIndicator::ACTIVE : ButtonIndicator::OFF);
    }

    // Visibility
    if (props.visible)
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
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
    return container_ && !lv_obj_has_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

void TransportBar::createContainer(lv_obj_t* parent) {
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, LV_PCT(100), Layout::TRANSPORT_BAR_HEIGHT);
    Style::setBgColor(container_, Color::BACKGROUND_FILL);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_pad_all(container_, 0, 0);
    lv_obj_set_scrollbar_mode(container_, LV_SCROLLBAR_MODE_OFF);

    // Grid: 3 columns (MIDI | Transport | Tempo)
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
}

void TransportBar::createTransportControls() {
    // Left cell: MIDI indicators
    lv_obj_t* midi_container = lv_obj_create(container_);
    lv_obj_set_grid_cell(midi_container, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    Style::applyTransparentContainer(midi_container);
    Style::applyFlexRow(midi_container, LV_FLEX_ALIGN_START, Layout::GAP_SM);

    midi_in_indicator_ = std::make_unique<ButtonIndicator>(midi_container, Layout::INDICATOR_SIZE);
    midi_in_indicator_->setCustomColor(ButtonIndicator::OFF, COLOR_MIDI);
    midi_in_indicator_->setCustomOpacity(ButtonIndicator::OFF, Opacity::FULL);
    midi_in_indicator_->setCustomColor(ButtonIndicator::ACTIVE, COLOR_ACTIVE);
    midi_in_indicator_->setCustomOpacity(ButtonIndicator::ACTIVE, Opacity::FULL);
    midi_in_indicator_->setState(ButtonIndicator::OFF);

    midi_out_indicator_ = std::make_unique<ButtonIndicator>(midi_container, Layout::INDICATOR_SIZE);
    midi_out_indicator_->setCustomColor(ButtonIndicator::OFF, COLOR_MIDI);
    midi_out_indicator_->setCustomOpacity(ButtonIndicator::OFF, Opacity::FULL);
    midi_out_indicator_->setCustomColor(ButtonIndicator::ACTIVE, COLOR_ACTIVE);
    midi_out_indicator_->setCustomOpacity(ButtonIndicator::ACTIVE, Opacity::FULL);
    midi_out_indicator_->setState(ButtonIndicator::OFF);

    // Center cell: Transport icons
    lv_obj_t* transport_container = lv_obj_create(container_);
    lv_obj_set_grid_cell(transport_container, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    Style::applyTransparentContainer(transport_container);
    Style::applyFlexRow(transport_container, LV_FLEX_ALIGN_CENTER, Layout::GAP_SM);

    play_icon_ = lv_label_create(transport_container);
    Icon::set(play_icon_, Icon::TRANSPORT_PLAY, Icon::L);
    lv_obj_set_style_text_color(play_icon_, COLOR_INACTIVE, 0);

    stop_icon_ = lv_label_create(transport_container);
    Icon::set(stop_icon_, Icon::TRANSPORT_STOP, Icon::L);
    lv_obj_set_style_text_color(stop_icon_, COLOR_INACTIVE, 0);

    record_icon_ = lv_label_create(transport_container);
    Icon::set(record_icon_, Icon::TRANSPORT_RECORD, Icon::L);
    lv_obj_set_style_text_color(record_icon_, COLOR_INACTIVE, 0);
}

void TransportBar::createTempoDisplay() {
    bpm_label_ = lv_label_create(container_);
    lv_obj_set_grid_cell(bpm_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_label_set_text(bpm_label_, "120.00");
    Style::setTextColor(bpm_label_, Color::TEXT_LIGHT);
    lv_obj_set_style_text_font(bpm_label_, fonts.tempo_label, 0);
}

}  // namespace Bitwig
