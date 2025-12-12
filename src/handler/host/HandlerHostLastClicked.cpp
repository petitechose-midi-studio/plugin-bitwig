#include "HandlerHostLastClicked.hpp"

#include "config/App.hpp"
#include "handler/DeviceConstants.hpp"
#include "protocol/struct/LastClickedUpdateMessage.hpp"
#include "protocol/struct/LastClickedValueChangeMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace Bitwig::Device;
using Config::EncoderID;

HandlerHostLastClicked::HandlerHostLastClicked(state::BitwigState& state,
                                               BitwigProtocol& protocol,
                                               oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

void HandlerHostLastClicked::setupProtocolCallbacks() {
    protocol_.onLastClickedUpdate = [this](const LastClickedUpdateMessage& msg) {
        if (!msg.fromHost) return;
        handleLastClickedUpdate(msg);
    };

    protocol_.onLastClickedValueChange = [this](const LastClickedValueChangeMessage& msg) {
        if (!msg.fromHost) return;
        handleLastClickedValueChange(msg);
    };
}

void HandlerHostLastClicked::handleLastClickedUpdate(const LastClickedUpdateMessage& msg) {
    auto& lc = state_.lastClicked;

    lc.name.set(std::string(msg.parameterName.data()));
    lc.value.set(msg.parameterValue);
    lc.displayValue.set(std::string(msg.displayValue.data()));
    lc.parameterType.set(static_cast<state::ParameterType>(msg.parameterType));
    lc.discreteCount.set(msg.discreteValueCount);
    lc.exists.set(msg.parameterExists);

    if (!msg.parameterExists) return;

    // Configure encoder based on parameter type
    if (msg.parameterType == static_cast<uint8_t>(ParameterType::KNOB)) {
        encoders_.setContinuous(EncoderID::OPT);
    } else {
        encoders_.setDiscreteSteps(EncoderID::OPT, msg.discreteValueCount);
    }

    encoders_.setPosition(EncoderID::OPT, msg.parameterValue);
}

void HandlerHostLastClicked::handleLastClickedValueChange(const LastClickedValueChangeMessage& msg) {
    auto& lc = state_.lastClicked;

    lc.value.set(msg.parameterValue);
    lc.displayValue.set(std::string(msg.displayValue.data()));

    encoders_.setPosition(EncoderID::OPT, msg.parameterValue);
}

}  // namespace bitwig::handler
