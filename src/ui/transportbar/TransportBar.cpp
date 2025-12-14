#include "TransportBar.hpp"

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"

using namespace Theme;

namespace {
const lv_color_t COLOR_INACTIVE = lv_color_hex(Color::DATA_INACTIVE);
const lv_color_t COLOR_ACTIVE = lv_color_hex(Color::DATA_ACTIVE);
const lv_color_t COLOR_PLAY = lv_color_hex(Color::MACRO_5);
const lv_color_t COLOR_RECORD = lv_color_hex(Color::MACRO_1);
const lv_color_t COLOR_MIDI = lv_color_hex(Color::KNOB_VALUE_RIBBON);
}  // namespace

namespace Bitwig {

TransportBar::TransportBar(lv_obj_t* parent, bitwig::state::TransportState& state)
    : state_(state), parent_(parent) {
    if (!parent_) return;

    createContainer(parent_);
    createTransportControls();
    createTempoDisplay();
    setupBindings();
    render();  // Initial render
}

TransportBar::~TransportBar() {
    subs_.clear();
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void TransportBar::setupBindings() {
    subs_.push_back(state_.playing.subscribe([this](bool playing) { setPlayState(playing); }));
    subs_.push_back(
        state_.recording.subscribe([this](bool recording) { setRecordState(recording); }));
    subs_.push_back(state_.tempo.subscribe([this](float tempo) { setTempo(tempo); }));
    subs_.push_back(state_.midiInActive.subscribe([this](bool active) { setMidiIn(active); }));
    subs_.push_back(state_.midiOutActive.subscribe([this](bool active) { setMidiOut(active); }));
}

void TransportBar::render() {
    setPlayState(state_.playing.get());
    setRecordState(state_.recording.get());
    setTempo(state_.tempo.get());
    setMidiIn(state_.midiInActive.get());
    setMidiOut(state_.midiOutActive.get());
}

void TransportBar::setPlayState(bool playing) {
    if (play_icon_) {
        lv_obj_set_style_text_color(play_icon_, playing ? COLOR_PLAY : COLOR_INACTIVE,
                                    LV_STATE_DEFAULT);
    }
}

void TransportBar::setRecordState(bool recording) {
    if (record_icon_) {
        lv_obj_set_style_text_color(record_icon_, recording ? COLOR_RECORD : COLOR_INACTIVE,
                                    LV_STATE_DEFAULT);
    }
}

void TransportBar::setTempo(float tempo) {
    if (bpm_label_) { lv_label_set_text_fmt(bpm_label_, "%.2f", static_cast<double>(tempo)); }
}

void TransportBar::setMidiIn(bool active) {
    if (midi_in_indicator_) {
        midi_in_indicator_->setState(active ? StateIndicator::State::ACTIVE
                                            : StateIndicator::State::OFF);
    }
}

void TransportBar::setMidiOut(bool active) {
    if (midi_out_indicator_) {
        midi_out_indicator_->setState(active ? StateIndicator::State::ACTIVE
                                             : StateIndicator::State::OFF);
    }
}

void TransportBar::show() {
    if (container_) { lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN); }
}

void TransportBar::hide() {
    if (container_) { lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN); }
}

bool TransportBar::isVisible() const {
    return container_ && !lv_obj_has_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

void TransportBar::createContainer(lv_obj_t* parent) {
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, LV_PCT(100), Layout::TRANSPORT_BAR_HEIGHT);
    Style::setBgColor(container_, Color::BACKGROUND_FILL);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(container_, LV_SCROLLBAR_MODE_OFF);

    // Grid: 3 columns (MIDI | Transport | Tempo)
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
                                         LV_GRID_TEMPLATE_LAST};
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

    midi_in_indicator_ = std::make_unique<StateIndicator>(midi_container, Layout::INDICATOR_SIZE);
    midi_in_indicator_->color(StateIndicator::State::OFF, lv_color_to_u32(COLOR_MIDI))
        .opacity(StateIndicator::State::OFF, Opacity::FULL)
        .color(StateIndicator::State::ACTIVE, lv_color_to_u32(COLOR_ACTIVE))
        .opacity(StateIndicator::State::ACTIVE, Opacity::FULL);
    midi_in_indicator_->setState(StateIndicator::State::OFF);

    midi_out_indicator_ = std::make_unique<StateIndicator>(midi_container, Layout::INDICATOR_SIZE);
    midi_out_indicator_->color(StateIndicator::State::OFF, lv_color_to_u32(COLOR_MIDI))
        .opacity(StateIndicator::State::OFF, Opacity::FULL)
        .color(StateIndicator::State::ACTIVE, lv_color_to_u32(COLOR_ACTIVE))
        .opacity(StateIndicator::State::ACTIVE, Opacity::FULL);
    midi_out_indicator_->setState(StateIndicator::State::OFF);

    // Center cell: Transport icons
    lv_obj_t* transport_container = lv_obj_create(container_);
    lv_obj_set_grid_cell(transport_container, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_STRETCH, 0,
                         1);
    Style::applyTransparentContainer(transport_container);
    Style::applyFlexRow(transport_container, LV_FLEX_ALIGN_CENTER, Layout::GAP_SM);

    play_icon_ = lv_label_create(transport_container);
    Icon::set(play_icon_, Icon::TRANSPORT_PLAY, Icon::Size::L);
    lv_obj_set_style_text_color(play_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);

    stop_icon_ = lv_label_create(transport_container);
    Icon::set(stop_icon_, Icon::TRANSPORT_STOP, Icon::Size::L);
    lv_obj_set_style_text_color(stop_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);

    record_icon_ = lv_label_create(transport_container);
    Icon::set(record_icon_, Icon::TRANSPORT_RECORD, Icon::Size::L);
    lv_obj_set_style_text_color(record_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);
}

void TransportBar::createTempoDisplay() {
    bpm_label_ = lv_label_create(container_);
    lv_obj_set_grid_cell(bpm_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_label_set_text(bpm_label_, "120.00");
    Style::setTextColor(bpm_label_, Color::TEXT_LIGHT);
    lv_obj_set_style_text_font(bpm_label_, bitwig_fonts.page_label, LV_STATE_DEFAULT);
}

}  // namespace Bitwig
