#include "MidiHostHandler.hpp"

namespace bitwig::handler {

MidiHostHandler::MidiHostHandler(state::BitwigState& state, oc::api::MidiAPI& midi)
    : state_(state), midi_(midi) {
    setupMidiCallbacks();
}

void MidiHostHandler::setupMidiCallbacks() {
    midi_.onNoteOn([this](uint8_t /*channel*/, uint8_t /*note*/, uint8_t /*velocity*/) {
        state_.transport.midiInActive.set(true);
    });

    midi_.onNoteOff([this](uint8_t /*channel*/, uint8_t /*note*/, uint8_t /*velocity*/) {
        state_.transport.midiInActive.set(false);
    });
}

}  // namespace bitwig::handler
