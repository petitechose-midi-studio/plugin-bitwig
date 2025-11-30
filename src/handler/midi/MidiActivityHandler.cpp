#include "MidiActivityHandler.hpp"
#include "api/ControllerAPI.hpp"
#include "ui/transportbar/TransportBarController.hpp"

namespace Bitwig {

MidiActivityHandler::MidiActivityHandler(ControllerAPI& api, TransportBarController& transportController)
    : api_(api), transportController_(transportController) {
    setupMidiCallbacks();
}

void MidiActivityHandler::setupMidiCallbacks() {
    api_.onNoteOn([this](uint8_t, uint8_t, uint8_t) {
        transportController_.setMidiIn(true);
    });

    api_.onNoteOff([this](uint8_t, uint8_t, uint8_t) {
        transportController_.setMidiIn(false);
    });
}

}  // namespace Bitwig
