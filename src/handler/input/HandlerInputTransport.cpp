#include "HandlerInputTransport.hpp"

#include "config/App.hpp"
#include "protocol/MessageStructure.hpp"

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
    setupBindings();
}

void HandlerInputTransport::setupBindings() {
    // Global bindings (no scope) - lowest priority
    encoders_.setMode(EncoderID::NAV, oc::hal::EncoderMode::RELATIVE);

    buttons_.button(ButtonID::BOTTOM_LEFT)
        .release()
        .then([this]() { togglePlay(); });

    buttons_.button(ButtonID::BOTTOM_CENTER)
        .release()
        .then([this]() { stop(); });

    buttons_.button(ButtonID::BOTTOM_RIGHT)
        .release()
        .then([this]() { toggleRecord(); });

    encoders_.encoder(EncoderID::NAV)
        .turn()
        .then([this](float delta) { adjustTempo(delta); });
}

void HandlerInputTransport::togglePlay() {
    bool newState = !state_.transport.playing.get();
    state_.transport.playing.set(newState);
    protocol_.send(Protocol::TransportPlayMessage{newState});
}

void HandlerInputTransport::stop() {
    state_.transport.playing.set(false);
    state_.transport.recording.set(false);
    protocol_.send(Protocol::TransportStopMessage{});
}

void HandlerInputTransport::toggleRecord() {
    bool newState = !state_.transport.recording.get();
    state_.transport.recording.set(newState);
    protocol_.send(Protocol::TransportRecordMessage{newState});
}

void HandlerInputTransport::adjustTempo(float delta) {
    protocol_.send(Protocol::TransportTempoMessage{delta});
}

}  // namespace bitwig::handler
