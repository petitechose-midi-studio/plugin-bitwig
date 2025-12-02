#include "TransportHostHandler.hpp"

#include "ui/transportbar/TransportBarController.hpp"

namespace Bitwig {

TransportHostHandler::TransportHostHandler(ControllerAPI &api, Protocol::Protocol &protocol,
                                           TransportBarController &controller)
    : api_(api), protocol_(protocol), view_controller_(controller) {
    setupProtocolCallbacks();
}

void TransportHostHandler::setupProtocolCallbacks() {
    protocol_.onTransportPlay = [this](const Protocol::TransportPlayMessage &msg) {
        view_controller_.setPlaying(msg.isPlaying);
    };

    protocol_.onTransportRecord = [this](const Protocol::TransportRecordMessage &msg) {
        view_controller_.setRecording(msg.isRecording);
    };

    protocol_.onTransportStop = [this](const Protocol::TransportStopMessage &msg) {
        view_controller_.setPlaying(false);
        view_controller_.setRecording(false);
    };

    protocol_.onTransportTempo = [this](const Protocol::TransportTempoMessage &msg) {
        // Update display (encoder position not synced in Relative mode)
        view_controller_.setTempo(msg.tempo);
    };
}

}  // namespace Bitwig
