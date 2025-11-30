#pragma once

#include <cstdint>

namespace Bitwig {

/**
 * @brief Transport state for play/record/tempo display
 */
struct TransportState {
    bool playing = false;
    bool recording = false;
    float tempo = 120.0f;

    // MIDI activity indicators
    bool midiInActive = false;
    bool midiOutActive = false;

    // Dirty tracking
    bool dirty = false;

    void markDirty() { dirty = true; }
    void clearDirty() { dirty = false; }
};

/**
 * @brief Props for TransportBar rendering
 */
struct TransportBarProps {
    bool playing = false;
    bool recording = false;
    float tempo = 120.0f;
    bool midiInActive = false;
    bool midiOutActive = false;
    bool visible = true;

    static TransportBarProps fromState(const TransportState& state) {
        return {
            .playing = state.playing,
            .recording = state.recording,
            .tempo = state.tempo,
            .midiInActive = state.midiInActive,
            .midiOutActive = state.midiOutActive,
            .visible = true
        };
    }
};

}  // namespace Bitwig
