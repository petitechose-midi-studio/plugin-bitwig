#pragma once

#include "TransportBar.hpp"

namespace Bitwig
{

    class TransportBarController
    {
    public:
        explicit TransportBarController(TransportBar &transportBar);

        void setPlaying(bool playing);
        void setRecording(bool recording);
        void setTempo(float bpm);

        void flashMidiIn();
        void flashMidiOut();

    private:
        TransportBar &transportBar_;
    };

} // namespace Bitwig
