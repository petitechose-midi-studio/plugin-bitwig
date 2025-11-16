#include "TransportBarController.hpp"
#include "log/Macros.hpp"

namespace Plugin::Bitwig {

TransportBarController::TransportBarController(TransportBar& transportBar)
    : transportBar_(transportBar) {}

void TransportBarController::setPlaying(bool playing) {
    transportBar_.setPlay(playing);
}

void TransportBarController::setRecording(bool recording) {
    transportBar_.setRecord(recording);
}

void TransportBarController::setTempo(float bpm) {
    transportBar_.setTempo(bpm);
}

void TransportBarController::flashMidiIn() {}

void TransportBarController::flashMidiOut() {}

} // namespace Plugin::Bitwig
