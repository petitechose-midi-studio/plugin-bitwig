#include "TransportInputHandler.hpp"

#include "protocol/struct/TransportPlayMessage.hpp"
#include "protocol/struct/TransportRecordMessage.hpp"
#include "protocol/struct/TransportStopMessage.hpp"
#include "protocol/struct/TransportTempoMessage.hpp"

namespace bitwig::handler {

TransportInputHandler::TransportInputHandler(BitwigProtocol& protocol)
    : protocol_(protocol) {}

void TransportInputHandler::togglePlay() {
    protocol_.togglePlay();
}

void TransportInputHandler::toggleRecord() {
    protocol_.toggleRecord();
}

void TransportInputHandler::stop() {
    protocol_.stop();
}

void TransportInputHandler::adjustTempo(float delta) {
    protocol_.send(Protocol::TransportTempoMessage{delta});
}

}  // namespace bitwig::handler
