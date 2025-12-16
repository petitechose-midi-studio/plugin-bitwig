#include "HandlerHostMacro.hpp"

#include <array>

#include "handler/InputUtils.hpp"
#include "protocol/struct/DeviceMacroDiscreteValuesMessage.hpp"
#include "protocol/struct/DeviceMacroNameChangeMessage.hpp"
#include "protocol/struct/DeviceMacroUpdateMessage.hpp"
#include "protocol/struct/DeviceMacroValueChangeMessage.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;

HandlerHostMacro::HandlerHostMacro(state::BitwigState& state,
                                   BitwigProtocol& protocol,
                                   oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

void HandlerHostMacro::setupProtocolCallbacks() {
    protocol_.onDeviceMacroUpdate = [this](const DeviceMacroUpdateMessage& msg) {
        if (msg.parameterIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.parameterIndex];

        slot.type.set(static_cast<state::ParameterType>(msg.parameterType));
        slot.discreteCount.set(msg.discreteValueCount);
        slot.currentValueIndex.set(msg.currentValueIndex);
        slot.origin.set(msg.parameterOrigin);
        slot.displayValue.set(msg.displayValue.c_str());
        slot.name.set(msg.parameterName.c_str());
        slot.value.set(msg.parameterValue);
        slot.visible.set(msg.parameterExists);
        slot.loading.set(false);

        // Configure encoder
        auto encoderId = getEncoderIdForParameter(msg.parameterIndex);
        if (encoderId != EncoderID{0}) {
            configureEncoderForParameter(encoders_, encoderId,
                                         msg.parameterType,
                                         msg.discreteValueCount,
                                         msg.parameterValue);
        }
    };

    protocol_.onDeviceMacroDiscreteValues = [this](const DeviceMacroDiscreteValuesMessage& msg) {
        if (msg.parameterIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.parameterIndex];

        // Local buffer for discrete values (stack allocated, safe in single-threaded context)
        std::array<std::string, state::MAX_DISCRETE_VALUES> tempValues;
        size_t count = 0;
        for (const auto& dv : msg.discreteValueNames) {
            if (!dv.empty() && count < state::MAX_DISCRETE_VALUES) {
                tempValues[count++] = dv;
            }
        }
        slot.discreteValues.set(tempValues.data(), count);
        slot.currentValueIndex.set(msg.currentValueIndex);
    };

    protocol_.onDeviceMacroValueChange = [this](const DeviceMacroValueChangeMessage& msg) {
        if (!msg.fromHost) { return; }
        if (msg.parameterIndex >= PARAMETER_COUNT) { return; }

        auto& slot = state_.parameters.slots[msg.parameterIndex];
        auto currentType = slot.type.get();
        auto encoderId = getEncoderIdForParameter(msg.parameterIndex);

        if (msg.isEcho) {
            // Echo: only update display value for non-knob parameters
            if (currentType != state::ParameterType::KNOB) {
                slot.value.set(msg.parameterValue);
                slot.displayValue.set(msg.displayValue.c_str());
            }
        } else {
            // External change: update value and encoder position
            slot.value.set(msg.parameterValue);
            slot.displayValue.set(msg.displayValue.c_str());

            if (encoderId != EncoderID{0}) {
                encoders_.setPosition(encoderId, msg.parameterValue);
            }
        }
    };

    protocol_.onDeviceMacroNameChange = [this](const DeviceMacroNameChangeMessage& msg) {
        if (msg.parameterIndex >= PARAMETER_COUNT) return;
        state_.parameters.slots[msg.parameterIndex].name.set(msg.parameterName.c_str());
    };
}

}  // namespace bitwig::handler
