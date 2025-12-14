#include "HandlerHostTransport.hpp"

#include <oc/teensy/LogOutput.hpp>
#include <oc/log/Log.hpp>

#include "protocol/struct/TransportPlayMessage.hpp"
#include "protocol/struct/TransportRecordMessage.hpp"
#include "protocol/struct/TransportStopMessage.hpp"
#include "protocol/struct/TransportTempoMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;

HandlerHostTransport::HandlerHostTransport(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    OC_LOG_INFO("[HostTransport] Setting up protocol callbacks...");
    setupProtocolCallbacks();
    OC_LOG_INFO("[HostTransport] Callbacks registered");
}

void HandlerHostTransport::setupProtocolCallbacks() {
    protocol_.onTransportPlay = [this](const TransportPlayMessage& msg) {
        OC_LOG_INFO("[HostTransport] >> onTransportPlay: isPlaying={}", msg.isPlaying);
        state_.transport.playing.set(msg.isPlaying);
        OC_LOG_INFO("[HostTransport] << state.transport.playing updated");
    };

    protocol_.onTransportRecord = [this](const TransportRecordMessage& msg) {
        OC_LOG_INFO("[HostTransport] >> onTransportRecord: isRecording={}", msg.isRecording);
        state_.transport.recording.set(msg.isRecording);
        OC_LOG_INFO("[HostTransport] << state.transport.recording updated");
    };

    protocol_.onTransportStop = [this](const TransportStopMessage&) {
        OC_LOG_INFO("[HostTransport] >> onTransportStop");
        state_.transport.playing.set(false);
        state_.transport.recording.set(false);
        OC_LOG_INFO("[HostTransport] << playing=false, recording=false");
    };

    protocol_.onTransportTempo = [this](const TransportTempoMessage& msg) {
        OC_LOG_INFO("[HostTransport] >> onTransportTempo: tempo={}", msg.tempo);
        state_.transport.tempo.set(msg.tempo);
        OC_LOG_INFO("[HostTransport] << state.transport.tempo updated");
    };
}

}  // namespace bitwig::handler
