#include "LastClickedHostHandler.hpp"

namespace Bitwig {

LastClickedHostHandler::LastClickedHostHandler(ControllerAPI& api, Protocol::Protocol& protocol,
                                               LastClickedState& state)
    : api_(api), protocol_(protocol), state_(state) {
    setupProtocolCallbacks();
}

void LastClickedHostHandler::setupProtocolCallbacks() {
    protocol_.onLastClickedUpdate = [this](const Protocol::LastClickedUpdateMessage& msg) {
        if (!msg.fromHost) return;
        handleLastClickedUpdate(msg);
    };

    protocol_.onLastClickedValueChange =
        [this](const Protocol::LastClickedValueChangeMessage& msg) {
            if (!msg.fromHost) return;
            handleLastClickedValueChange(msg);
        };
}

void LastClickedHostHandler::handleLastClickedUpdate(
    const Protocol::LastClickedUpdateMessage& msg) {
    state_.name = std::string(msg.parameterName.data());
    state_.value = msg.parameterValue;
    state_.displayValue = std::string(msg.displayValue.data());
    state_.parameterType = msg.parameterType;
    state_.discreteCount = msg.discreteValueCount;
    state_.exists = msg.parameterExists;

    if (!state_.exists) { return; }

    if (state_.parameterType == static_cast<uint8_t>(Device::ParameterType::KNOB)) {
        api_.setEncoderContinuous(EncoderID::OPT);
    } else {
        api_.setEncoderDiscreteSteps(EncoderID::OPT, state_.discreteCount);
    }

    api_.setEncoderPosition(EncoderID::OPT, state_.value);
}

void LastClickedHostHandler::handleLastClickedValueChange(
    const Protocol::LastClickedValueChangeMessage& msg) {
    state_.value = msg.parameterValue;
    state_.displayValue = std::string(msg.displayValue.data());
    api_.setEncoderPosition(EncoderID::OPT, msg.parameterValue);
}

}  // namespace Bitwig
