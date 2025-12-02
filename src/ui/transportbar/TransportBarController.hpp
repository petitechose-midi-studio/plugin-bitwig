#pragma once

#include "TransportBar.hpp"

#include "ui/state/TransportState.hpp"

namespace Bitwig {

class TransportBarController {
public:
    explicit TransportBarController(TransportBar& view);

    // State-based updates
    void setPlaying(bool playing);
    void setRecording(bool recording);
    void setTempo(float bpm);
    void setMidiIn(bool active);
    void setMidiOut(bool active);

    // Sync view with state
    void sync();
private:
    TransportBar& view_;
    TransportState state_;
};

}  // namespace Bitwig
