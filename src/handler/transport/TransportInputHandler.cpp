#include "TransportInputHandler.hpp"

namespace Bitwig {

TransportInputHandler::TransportInputHandler(ControllerAPI &api, Protocol::Protocol &protocol)
    : api_(api), protocol_(protocol) {
    setupInputBindings();
}

void TransportInputHandler::setupInputBindings() {
    // Configure tempo encoder for Relative mode
    api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::RELATIVE);

    api_.onReleased(ButtonID::BOTTOM_LEFT, [this]() { togglePlay(); });

    api_.onReleased(ButtonID::BOTTOM_RIGHT, [this]() { toggleRecord(); });

    api_.onReleased(ButtonID::BOTTOM_CENTER, [this]() { stop(); });

    api_.onTurned(EncoderID::NAV, [this](float delta) { adjustTempo(delta); });
}

void TransportInputHandler::togglePlay() {
    // Send toggle - Bitwig will return actual state via TransportHostHandler
    protocol_.send(Protocol::TransportPlayMessage{true});
}

void TransportInputHandler::toggleRecord() {
    // Send toggle - Bitwig will return actual state via TransportHostHandler
    protocol_.send(Protocol::TransportRecordMessage{true});
}

void TransportInputHandler::stop() { protocol_.send(Protocol::TransportStopMessage{}); }

void TransportInputHandler::adjustTempo(float delta) {
    protocol_.send(Protocol::TransportTempoMessage{delta});
}

}  // namespace Bitwig
