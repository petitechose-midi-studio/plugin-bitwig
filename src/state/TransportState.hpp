#pragma once

/**
 * @file TransportState.hpp
 * @brief Signal-based state for transport (play/record/tempo)
 */

#include <oc/state/Signal.hpp>

namespace bitwig::state {

using oc::state::Signal;

/**
 * @brief State for DAW transport
 */
struct TransportState {
    Signal<bool> playing{false};
    Signal<bool> recording{false};
    Signal<float> tempo{120.0f};

    // MIDI activity indicators (pulsed by MidiActivityHandler)
    Signal<bool> midiInActive{false};
    Signal<bool> midiOutActive{false};

    // Automation override: true when any parameter is touched/overriding automation
    Signal<bool> automationOverrideActive{false};

    void reset() {
        playing.set(false);
        recording.set(false);
        tempo.set(120.0f);
        midiInActive.set(false);
        midiOutActive.set(false);
        automationOverrideActive.set(false);
    }
};

}  // namespace bitwig::state
