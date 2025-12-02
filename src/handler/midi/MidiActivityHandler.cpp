#include "MidiActivityHandler.hpp"

#include "api/ControllerAPI.hpp"
#include "ui/transportbar/TransportBarController.hpp"

namespace Bitwig {

MidiActivityHandler::MidiActivityHandler(ControllerAPI& api,
                                         TransportBarController& transportController)
    : api_(api), transport_controller_(transportController) {
    setupMidiCallbacks();
}

void MidiActivityHandler::setupMidiCallbacks() {
    api_.onNoteOn([this](uint8_t, uint8_t, uint8_t) { transport_controller_.setMidiIn(true); });

    api_.onNoteOff([this](uint8_t, uint8_t, uint8_t) { transport_controller_.setMidiIn(false); });
}

}  // namespace Bitwig
