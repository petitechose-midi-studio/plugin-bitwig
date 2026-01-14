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

#include <oc/api/MidiAPI.hpp>

#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief MIDI activity handler (MIDI events -> State)
 */
class MidiHostHandler {
public:
    MidiHostHandler(state::BitwigState& state, oc::api::MidiAPI& midi);
    ~MidiHostHandler() = default;

    // Non-copyable
    MidiHostHandler(const MidiHostHandler&) = delete;
    MidiHostHandler& operator=(const MidiHostHandler&) = delete;

private:
    void setupMidiCallbacks();

    state::BitwigState& state_;
    oc::api::MidiAPI& midi_;
};

}  // namespace bitwig::handler
