#include "MidiInputHandler.hpp"
#include "api/ControllerAPI.hpp"
#include "ui/transportbar/TransportBarController.hpp"

namespace Bitwig {

MidiInputHandler::MidiInputHandler(ControllerAPI& api, TransportBarController& transportController)
    : api_(api), transportController_(transportController) {
    setupCallbacks();
}

void MidiInputHandler::setupCallbacks() {
    api_.onNoteOn([this](uint8_t, uint8_t, uint8_t) {
        transportController_.flashMidiIn();
    });

    api_.onNoteOff([this](uint8_t, uint8_t, uint8_t) {
        transportController_.flashMidiIn();
    });
}

}  // namespace Bitwig
