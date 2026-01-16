#include "TransportBar.hpp"

#include <oc/state/Bind.hpp>
#include <oc/ui/lvgl/style/StyleBuilder.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/BitwigIcons.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;
namespace style = oc::ui::lvgl::style;
namespace icons = bitwig::icons;

namespace {
const lv_color_t COLOR_INACTIVE = lv_color_hex(color::DATA_INACTIVE);
const lv_color_t COLOR_ACTIVE = lv_color_hex(color::DATA_ACTIVE);
const lv_color_t COLOR_PLAY = lv_color_hex(color::MACRO_5);
const lv_color_t COLOR_RECORD = lv_color_hex(color::MACRO_1);
const lv_color_t COLOR_MIDI = lv_color_hex(color::KNOB_VALUE_RIBBON);
const lv_color_t COLOR_AUTOMATION_OVERRIDE = lv_color_hex(color::AUTOMATION_OVERRIDE);
}  // namespace

namespace bitwig::ui {

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
    using oc::state::bind;
    bind(subs_)
        .on(state_.playing, [this](bool playing) { setPlayState(playing); })
        .on(state_.recording, [this](bool recording) { setRecordState(recording); })
        .on(state_.tempo, [this](float tempo) { setTempo(tempo); })
        .on(state_.midiInActive, [this](bool active) { setMidiIn(active); })
        .on(state_.midiOutActive, [this](bool active) { setMidiOut(active); })
        .on(state_.automationOverrideActive, [this](bool active) { setAutomationOverride(active); });
}

void TransportBar::render() {
    setPlayState(state_.playing.get());
    setRecordState(state_.recording.get());
    setTempo(state_.tempo.get());
    setMidiIn(state_.midiInActive.get());
    setMidiOut(state_.midiOutActive.get());
    setAutomationOverride(state_.automationOverrideActive.get());
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
    if (bpm_label_) {
        bpm_label_->setText(tempo, 2);
    }
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

void TransportBar::setAutomationOverride(bool active) {
    if (automation_override_icon_) {
        lv_obj_set_style_text_color(automation_override_icon_,
                                    active ? COLOR_AUTOMATION_OVERRIDE : COLOR_INACTIVE,
                                    LV_STATE_DEFAULT);
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
    lv_obj_set_size(container_, LV_PCT(100), layout::TRANSPORT_BAR_HEIGHT);
    style::apply(container_).bgColor(color::BACKGROUND_FILL);
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
    style::apply(midi_container).transparent().noScroll().flexRow(LV_FLEX_ALIGN_START, layout::GAP_SM);

    midi_in_indicator_ = std::make_unique<StateIndicator>(midi_container, layout::INDICATOR_SIZE);
    midi_in_indicator_->color(StateIndicator::State::OFF, lv_color_to_u32(COLOR_MIDI))
        .opacity(StateIndicator::State::OFF, opacity::FULL)
        .color(StateIndicator::State::ACTIVE, lv_color_to_u32(COLOR_ACTIVE))
        .opacity(StateIndicator::State::ACTIVE, opacity::FULL);
    midi_in_indicator_->setState(StateIndicator::State::OFF);

    midi_out_indicator_ = std::make_unique<StateIndicator>(midi_container, layout::INDICATOR_SIZE);
    midi_out_indicator_->color(StateIndicator::State::OFF, lv_color_to_u32(COLOR_MIDI))
        .opacity(StateIndicator::State::OFF, opacity::FULL)
        .color(StateIndicator::State::ACTIVE, lv_color_to_u32(COLOR_ACTIVE))
        .opacity(StateIndicator::State::ACTIVE, opacity::FULL);
    midi_out_indicator_->setState(StateIndicator::State::OFF);

    // Center cell: Transport icons
    lv_obj_t* transport_container = lv_obj_create(container_);
    lv_obj_set_grid_cell(transport_container, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_STRETCH, 0,
                         1);
    style::apply(transport_container).transparent().noScroll().flexRow(LV_FLEX_ALIGN_CENTER, layout::GAP_SM);

    play_icon_ = lv_label_create(transport_container);
    icons::set(play_icon_, icons::TRANSPORT_PLAY, icons::Size::L);
    lv_obj_set_style_text_color(play_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);

    stop_icon_ = lv_label_create(transport_container);
    icons::set(stop_icon_, icons::TRANSPORT_STOP, icons::Size::L);
    lv_obj_set_style_text_color(stop_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);

    record_icon_ = lv_label_create(transport_container);
    icons::set(record_icon_, icons::TRANSPORT_RECORD, icons::Size::L);
    lv_obj_set_style_text_color(record_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);

    automation_override_icon_ = lv_label_create(transport_container);
    icons::set(automation_override_icon_, icons::TRANSPORT_AUTOMATION_ABORT, icons::Size::L);
    lv_obj_set_style_text_color(automation_override_icon_, COLOR_INACTIVE, LV_STATE_DEFAULT);
}

void TransportBar::createTempoDisplay() {
    bpm_label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    bpm_label_->alignment(LV_TEXT_ALIGN_RIGHT)
              .gridCell(2, 1, 0, 1)
              .autoScroll(false)  // Désactiver auto-scroll pour debug
              .color(color::TEXT_LIGHT)
              .font(bitwig_fonts.page_label)
              .ownsLvglObjects(false);

    bpm_label_->setText("120.00");
}

}  // namespace bitwig::ui
