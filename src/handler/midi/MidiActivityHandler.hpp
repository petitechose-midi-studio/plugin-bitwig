#pragma once

#include <cstdint>

class ControllerAPI;

namespace Bitwig {

class TransportBarController;

/**
 * @brief Handler for MIDI activity indication
 *
 * Observes MIDI note events and updates TransportBar MIDI indicator.
 * Not a typical InputHandler (doesn't send protocol commands),
 * but an event observer for visual feedback.
 */
class MidiActivityHandler {
public:
    MidiActivityHandler(ControllerAPI& api, TransportBarController& transportController);
    ~MidiActivityHandler() = default;

private:
    void setupMidiCallbacks();

    ControllerAPI& api_;
    TransportBarController& transport_controller_;
};

}  // namespace Bitwig
