#include "TransportBarController.hpp"

namespace Bitwig {

TransportBarController::TransportBarController(TransportBar& view)
    : view_(view) {
    // No sync() here - lazy init will happen on first state change
}

void TransportBarController::setPlaying(bool playing) {
    if (state_.playing != playing) {
        state_.playing = playing;
        sync();
    }
}

void TransportBarController::setRecording(bool recording) {
    if (state_.recording != recording) {
        state_.recording = recording;
        sync();
    }
}

void TransportBarController::setTempo(float bpm) {
    if (state_.tempo != bpm) {
        state_.tempo = bpm;
        sync();
    }
}

void TransportBarController::setMidiIn(bool active) {
    if (state_.midiInActive != active) {
        state_.midiInActive = active;
        sync();
    }
}

void TransportBarController::setMidiOut(bool active) {
    if (state_.midiOutActive != active) {
        state_.midiOutActive = active;
        sync();
    }
}

void TransportBarController::sync() {
    view_.render(TransportBarProps::fromState(state_));
}

}  // namespace Bitwig
