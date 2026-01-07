#include "HandlerInputTransport.hpp"

#include "config/App.hpp"
#include "protocol/MessageStructure.hpp"

namespace bitwig::handler {

using ButtonID = Config::ButtonID;
using EncoderID = Config::EncoderID;

HandlerInputTransport::HandlerInputTransport(state::BitwigState& state,
                                             BitwigProtocol& protocol,
                                             core::api::InputAPI& input)
    : state_(state)
    , protocol_(protocol)
    , input_(input) {
    setupBindings();
}

void HandlerInputTransport::setupBindings() {
    // Global bindings (no scope) - lowest priority
    input_.encoders.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    input_.buttons.button(ButtonID::BOTTOM_LEFT)
        .release()
        .then([this]() { togglePlay(); });

    input_.buttons.button(ButtonID::BOTTOM_CENTER)
        .release()
        .then([this]() { stop(); });

    input_.buttons.button(ButtonID::BOTTOM_RIGHT)
        .release()
        .then([this]() { toggleRecord(); });

    input_.encoders.encoder(EncoderID::NAV)
        .turn()
        .then([this](float delta) { adjustTempo(delta); });
}

void HandlerInputTransport::togglePlay() {
    bool newState = !state_.transport.playing.get();
    state_.transport.playing.set(newState);
    protocol_.transportPlay(newState);
}

void HandlerInputTransport::stop() {
    state_.transport.playing.set(false);
    state_.transport.recording.set(false);
    protocol_.transportStop();
}

void HandlerInputTransport::toggleRecord() {
    bool newState = !state_.transport.recording.get();
    state_.transport.recording.set(newState);
    protocol_.transportRecord(newState);
}

void HandlerInputTransport::adjustTempo(float delta) {
    protocol_.transportTempo(delta);
}

}  // namespace bitwig::handler
