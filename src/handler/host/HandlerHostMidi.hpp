#pragma once

/**
 * @file HandlerHostMidi.hpp
 * @brief Handles MIDI activity -> updates BitwigState
 *
 * HostHandler pattern: MIDI events -> State updates
 *
 * Observes MIDI note events and updates state for visual feedback.
 * Not a typical HostHandler (doesn't use protocol callbacks),
 * but follows the same pattern of updating state from external events.
 */

#include <oc/api/MidiAPI.hpp>

#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief MIDI activity handler (MIDI events -> State)
 */
class HandlerHostMidi {
public:
    HandlerHostMidi(state::BitwigState& state, oc::api::MidiAPI& midi);
    ~HandlerHostMidi() = default;

    // Non-copyable
    HandlerHostMidi(const HandlerHostMidi&) = delete;
    HandlerHostMidi& operator=(const HandlerHostMidi&) = delete;

private:
    void setupMidiCallbacks();

    state::BitwigState& state_;
    oc::api::MidiAPI& midi_;
};

}  // namespace bitwig::handler
