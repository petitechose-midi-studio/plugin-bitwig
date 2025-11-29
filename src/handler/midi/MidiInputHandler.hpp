#pragma once

#include <cstdint>

class ControllerAPI;

namespace Bitwig {

class TransportBarController;

/**
 * @brief Handler for incoming MIDI messages
 *
 * Flashes MIDI indicator on TransportBar when notes are received
 */
class MidiInputHandler {
public:
    MidiInputHandler(ControllerAPI& api, TransportBarController& transportController);
    ~MidiInputHandler() = default;

private:
    void setupCallbacks();

    ControllerAPI& api_;
    TransportBarController& transportController_;
};

}  // namespace Bitwig
