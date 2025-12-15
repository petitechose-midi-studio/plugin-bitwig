#include "HandlerInputTransport.hpp"

#include <oc/teensy/LogOutput.hpp>
#include <oc/log/Log.hpp>

#include "config/App.hpp"
#include "protocol/struct/TransportTempoMessage.hpp"

namespace bitwig::handler {

using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;

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

    OC_LOG_INFO("[TransportInput] Setting NAV encoder to RELATIVE mode");
    // Configure NAV encoder for Relative mode
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    // Play/Pause
    OC_LOG_INFO("[TransportInput] Binding BOTTOM_LEFT -> togglePlay");
    buttons_.button(ButtonID::BOTTOM_LEFT)
        .release()
        .then([this]() { togglePlay(); });

    // Stop
    OC_LOG_INFO("[TransportInput] Binding BOTTOM_CENTER -> stop");
    buttons_.button(ButtonID::BOTTOM_CENTER)
        .release()
        .then([this]() { stop(); });

    // Record
    OC_LOG_INFO("[TransportInput] Binding BOTTOM_RIGHT -> toggleRecord");
    buttons_.button(ButtonID::BOTTOM_RIGHT)
        .release()
        .then([this]() { toggleRecord(); });

    OC_LOG_INFO("[TransportInput] Binding NAV encoder -> adjustTempo");
    // Tempo - NAV encoder (global)
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .then([this](float delta) { adjustTempo(delta); });
}

void HandlerInputTransport::togglePlay() {
    OC_LOG_INFO("[TransportInput] >> togglePlay() called");
    // Optimistic update (will be confirmed by host)
    bool newState = !state_.transport.playing.get();
    OC_LOG_INFO("[TransportInput] Optimistic: playing {} -> {}", !newState, newState);
    state_.transport.playing.set(newState);

    // Send to host
    OC_LOG_INFO("[TransportInput] Sending protocol_.togglePlay()");
    protocol_.togglePlay();
    OC_LOG_INFO("[TransportInput] << togglePlay() done");
}

void HandlerInputTransport::stop() {
    OC_LOG_INFO("[TransportInput] >> stop() called");
    // Optimistic update
    OC_LOG_INFO("[TransportInput] Optimistic: playing=false, recording=false");
    state_.transport.playing.set(false);
    state_.transport.recording.set(false);

    // Send to host
    OC_LOG_INFO("[TransportInput] Sending protocol_.stop()");
    protocol_.stop();
    OC_LOG_INFO("[TransportInput] << stop() done");
}

void HandlerInputTransport::toggleRecord() {
    OC_LOG_INFO("[TransportInput] >> toggleRecord() called");
    // Optimistic update
    bool newState = !state_.transport.recording.get();
    OC_LOG_INFO("[TransportInput] Optimistic: recording {} -> {}", !newState, newState);
    state_.transport.recording.set(newState);

    // Send to host
    OC_LOG_INFO("[TransportInput] Sending protocol_.toggleRecord()");
    protocol_.toggleRecord();
    OC_LOG_INFO("[TransportInput] << toggleRecord() done");
}

void HandlerInputTransport::adjustTempo(float delta) {
    OC_LOG_INFO("[TransportInput] >> adjustTempo(delta={})", delta);
    // In Relative mode, encoder sends delta directly (±1)
    // Send to host - no optimistic update for tempo (host is authoritative)
    protocol_.send(Protocol::TransportTempoMessage{delta});
    OC_LOG_INFO("[TransportInput] << adjustTempo() done");
}

}  // namespace bitwig::handler
