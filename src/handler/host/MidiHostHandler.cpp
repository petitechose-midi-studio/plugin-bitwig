#include "MidiHostHandler.hpp"

namespace bitwig::handler {

MidiHostHandler::MidiHostHandler(state::BitwigState& state)
    : state_(state) {}

void MidiHostHandler::onNoteOn(uint8_t /*channel*/, uint8_t /*note*/, uint8_t /*velocity*/) {
    state_.transport.midiInActive.set(true);
}

void MidiHostHandler::onNoteOff(uint8_t /*channel*/, uint8_t /*note*/, uint8_t /*velocity*/) {
    state_.transport.midiInActive.set(false);
}

}  // namespace bitwig::handler
