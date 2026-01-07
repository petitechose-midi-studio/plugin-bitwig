#include "TransportHostHandler.hpp"

#include "protocol/MessageStructure.hpp"

namespace bitwig::handler {

using namespace Protocol;

TransportHostHandler::TransportHostHandler(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();
}

void TransportHostHandler::setupProtocolCallbacks() {
    protocol_.onTransportPlayingState = [this](const TransportPlayingStateMessage& msg) {
        state_.transport.playing.set(msg.isPlaying);
    };

    protocol_.onTransportRecordingState = [this](const TransportRecordingStateMessage& msg) {
        state_.transport.recording.set(msg.isRecording);
    };

    protocol_.onTransportTempoState = [this](const TransportTempoStateMessage& msg) {
        state_.transport.tempo.set(msg.tempo);
    };

    protocol_.onTransportAutomationOverrideActiveState =
        [this](const TransportAutomationOverrideActiveStateMessage& msg) {
            state_.transport.automationOverrideActive.set(msg.isAutomationOverrideActive);
        };
}

}  // namespace bitwig::handler
