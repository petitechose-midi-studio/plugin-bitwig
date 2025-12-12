#include "HandlerInputTransport.hpp"

#include <oc/log/Log.hpp>

#include "config/App.hpp"
#include "protocol/struct/TransportTempoMessage.hpp"

namespace bitwig::handler {

using Config::ButtonID;
using Config::EncoderID;

HandlerInputTransport::HandlerInputTransport(state::BitwigState& state,
                                             BitwigProtocol& protocol,
                                             oc::api::EncoderAPI& encoders,
                                             oc::api::ButtonAPI& buttons)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , buttons_(buttons) {
    OC_LOG_INFO("[TransportInput] Creating handler...");
    setupBindings();
    OC_LOG_INFO("[TransportInput] Bindings created");
}

void HandlerInputTransport::setupBindings() {
    // === GLOBAL BINDINGS (no scope) ===
    // Transport bindings are global - lowest priority.
    // They only fire if no scoped binding captures the input.
    // This matches the original behavior.

    // Configure NAV encoder for Relative mode
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    // Play/Pause - BOTTOM_LEFT (global)
    buttons_.button(ButtonID::BOTTOM_LEFT)
        .press()
        .then([this]() { togglePlay(); });

    // Stop - BOTTOM_CENTER (global)
    buttons_.button(ButtonID::BOTTOM_CENTER)
        .press()
        .then([this]() { stop(); });

    // Record - BOTTOM_RIGHT (global)
    buttons_.button(ButtonID::BOTTOM_RIGHT)
        .press()
        .then([this]() { toggleRecord(); });

    // Tempo - NAV encoder (global)
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .then([this](float delta) { adjustTempo(delta); });
}

void HandlerInputTransport::togglePlay() {
    // Optimistic update (will be confirmed by host)
    bool newState = !state_.transport.playing.get();
    state_.transport.playing.set(newState);

    // Send to host
    protocol_.togglePlay();
}

void HandlerInputTransport::stop() {
    // Optimistic update
    state_.transport.playing.set(false);
    state_.transport.recording.set(false);

    // Send to host
    protocol_.stop();
}

void HandlerInputTransport::toggleRecord() {
    // Optimistic update
    bool newState = !state_.transport.recording.get();
    state_.transport.recording.set(newState);

    // Send to host
    protocol_.toggleRecord();
}

void HandlerInputTransport::adjustTempo(float delta) {
    // In Relative mode, encoder sends delta directly (±1)
    // Send to host - no optimistic update for tempo (host is authoritative)
    protocol_.send(Protocol::TransportTempoMessage{delta});
}

}  // namespace bitwig::handler
