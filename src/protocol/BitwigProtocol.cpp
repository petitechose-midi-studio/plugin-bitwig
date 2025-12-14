#include "BitwigProtocol.hpp"

#include <oc/teensy/LogOutput.hpp>
#include <oc/log/Log.hpp>

#include "MessageStructure.hpp"

namespace bitwig {

using namespace Protocol;

void BitwigProtocol::requestHostStatus() {
    OC_LOG_INFO("[Protocol] << SEND: RequestHostStatus");
    send(RequestHostStatusMessage{});
}

void BitwigProtocol::requestDeviceList() {
    OC_LOG_INFO("[Protocol] << SEND: RequestDeviceList");
    send(RequestDeviceListMessage{});
}

void BitwigProtocol::requestPageNames() {
    OC_LOG_INFO("[Protocol] << SEND: RequestPageNames");
    send(RequestDevicePageNamesMessage{});
}

void BitwigProtocol::requestTrackList() {
    OC_LOG_INFO("[Protocol] << SEND: RequestTrackList");
    send(RequestTrackListMessage{});
}

void BitwigProtocol::selectDevice(uint8_t index) {
    OC_LOG_INFO("[Protocol] << SEND: SelectDevice({})", index);
    send(DeviceSelectByIndexMessage{index});
}

void BitwigProtocol::selectPage(uint8_t index) {
    OC_LOG_INFO("[Protocol] << SEND: SelectPage({})", index);
    send(DevicePageSelectByIndexMessage{index});
}

void BitwigProtocol::selectTrack(uint8_t index) {
    OC_LOG_INFO("[Protocol] << SEND: SelectTrack({})", index);
    send(TrackSelectByIndexMessage{index});
}

void BitwigProtocol::sendParameterValue(uint8_t paramIndex, float value) {
    OC_LOG_INFO("[Protocol] << SEND: ParameterValue(idx={}, val={})", paramIndex, value);
    send(DeviceMacroValueChangeMessage{paramIndex, value});
}

void BitwigProtocol::sendParameterTouch(uint8_t paramIndex, bool touched) {
    OC_LOG_INFO("[Protocol] << SEND: ParameterTouch(idx={}, touched={})", paramIndex, touched);
    send(DeviceMacroTouchMessage{paramIndex, touched});
}

void BitwigProtocol::togglePlay() {
    OC_LOG_INFO("[Protocol] << SEND: TransportPlay (toggle)");
    send(TransportPlayMessage{true});
}

void BitwigProtocol::toggleRecord() {
    OC_LOG_INFO("[Protocol] << SEND: TransportRecord (toggle)");
    send(TransportRecordMessage{true});
}

void BitwigProtocol::stop() {
    OC_LOG_INFO("[Protocol] << SEND: TransportStop");
    send(TransportStopMessage{});
}

}  // namespace bitwig
