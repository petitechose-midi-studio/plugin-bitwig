#pragma once

/**
 * @file MidiHostHandler.hpp
 * @brief Handles MIDI activity -> updates BitwigState
 *
 * HostHandler pattern: MIDI events -> State updates
 *
 * Observes MIDI note events and updates state for visual feedback.
 * Not a typical HostHandler (doesn't use protocol callbacks),
 * but follows the same pattern of updating state from external events.
 */

#include <cstdint>

#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief MIDI activity handler (MIDI events -> State)
 */
class MidiHostHandler {
public:
    explicit MidiHostHandler(state::BitwigState& state);
    ~MidiHostHandler() = default;

    MidiHostHandler(const MidiHostHandler&) = delete;
    MidiHostHandler& operator=(const MidiHostHandler&) = delete;

    void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);

private:
    state::BitwigState& state_;
};

}  // namespace bitwig::handler
