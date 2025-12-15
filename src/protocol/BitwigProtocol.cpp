#include "BitwigProtocol.hpp"

#include "MessageStructure.hpp"

namespace bitwig {

using namespace Protocol;

void BitwigProtocol::requestHostStatus() {
    send(RequestHostStatusMessage{});
}

void BitwigProtocol::requestDeviceList() {
    send(RequestDeviceListMessage{});
}

void BitwigProtocol::requestPageNames() {
    send(RequestDevicePageNamesMessage{});
}

void BitwigProtocol::requestTrackList() {
    send(RequestTrackListMessage{});
}

void BitwigProtocol::selectDevice(uint8_t index) {
    send(DeviceSelectByIndexMessage{index});
}

void BitwigProtocol::selectPage(uint8_t index) {
    send(DevicePageSelectByIndexMessage{index});
}

void BitwigProtocol::selectTrack(uint8_t index) {
    send(TrackSelectByIndexMessage{index});
}

void BitwigProtocol::sendParameterValue(uint8_t paramIndex, float value) {
    send(DeviceMacroValueChangeMessage{paramIndex, value});
}

void BitwigProtocol::sendParameterTouch(uint8_t paramIndex, bool touched) {
    send(DeviceMacroTouchMessage{paramIndex, touched});
}

void BitwigProtocol::togglePlay() {
    send(TransportPlayMessage{true});
}

void BitwigProtocol::toggleRecord() {
    send(TransportRecordMessage{true});
}

void BitwigProtocol::stop() {
    send(TransportStopMessage{});
}

}  // namespace bitwig
