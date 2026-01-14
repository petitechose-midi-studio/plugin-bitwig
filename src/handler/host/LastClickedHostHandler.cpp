#include "LastClickedHostHandler.hpp"

#include <config/App.hpp>
#include "handler/InputUtils.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;
using EncoderID = Config::EncoderID;

LastClickedHostHandler::LastClickedHostHandler(state::BitwigState& state,
                                               BitwigProtocol& protocol,
                                               oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

void LastClickedHostHandler::setupProtocolCallbacks() {
    protocol_.onLastClickedUpdate = [this](const LastClickedUpdateMessage& msg) {
        handleLastClickedUpdate(msg);
    };

    protocol_.onLastClickedValueState = [this](const LastClickedValueStateMessage& msg) {
        handleLastClickedValueState(msg);
    };
}

void LastClickedHostHandler::handleLastClickedUpdate(const LastClickedUpdateMessage& msg) {
    auto& lc = state_.lastClicked;

    lc.name.set(msg.parameterName);
    lc.value.set(msg.parameterValue);
    lc.displayValue.set(msg.displayValue);
    lc.parameterType.set(msg.parameterType);
    lc.discreteCount.set(msg.discreteValueCount);
    lc.exists.set(msg.parameterExists);

    if (!msg.parameterExists) return;

    configureEncoderForParameter(encoders_, EncoderID::OPT,
                                 msg.parameterType,
                                 msg.discreteValueCount,
                                 msg.parameterValue);
}

void LastClickedHostHandler::handleLastClickedValueState(const LastClickedValueStateMessage& msg) {
    auto& lc = state_.lastClicked;

    lc.value.set(msg.parameterValue);
    lc.displayValue.set(msg.displayValue);

    encoders_.setPosition(EncoderID::OPT, msg.parameterValue);
}

}  // namespace bitwig::handler
