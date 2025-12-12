#include "TransportHostHandler.hpp"

#include "protocol/struct/TransportPlayMessage.hpp"
#include "protocol/struct/TransportRecordMessage.hpp"
#include "protocol/struct/TransportStopMessage.hpp"
#include "protocol/struct/TransportTempoMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;

TransportHostHandler::TransportHostHandler(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();
}

void TransportHostHandler::setupProtocolCallbacks() {
    protocol_.onTransportPlay = [this](const TransportPlayMessage& msg) {
        state_.transport.playing.set(msg.isPlaying);
    };

    protocol_.onTransportRecord = [this](const TransportRecordMessage& msg) {
        state_.transport.recording.set(msg.isRecording);
    };

    protocol_.onTransportStop = [this](const TransportStopMessage&) {
        state_.transport.playing.set(false);
        state_.transport.recording.set(false);
    };

    protocol_.onTransportTempo = [this](const TransportTempoMessage& msg) {
        state_.transport.tempo.set(msg.tempo);
    };
}

}  // namespace bitwig::handler
