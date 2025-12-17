#include "BitwigProtocol.hpp"

#include "MessageStructure.hpp"

namespace bitwig {

using namespace Protocol;

void BitwigProtocol::requestHostStatus() {
    send(RequestHostStatusMessage{});
}

void BitwigProtocol::requestDeviceList() {
    // Use windowed loading for device list
    send(RequestDeviceListWindowMessage{0});
}

void BitwigProtocol::requestPageNames() {
    // Use windowed loading for page names
    send(RequestDevicePageNamesWindowMessage{0});
}

void BitwigProtocol::requestTrackList() {
    // Use windowed loading for track list
    send(RequestTrackListWindowMessage{0});
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
    send(DeviceRemoteControlValueChangeMessage{paramIndex, value});
}

void BitwigProtocol::sendParameterTouch(uint8_t paramIndex, bool touched) {
    send(DeviceRemoteControlTouchMessage{paramIndex, touched});
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
