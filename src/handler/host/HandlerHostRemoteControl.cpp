#include "HandlerHostRemoteControl.hpp"

#include "handler/InputUtils.hpp"
#include "protocol/struct/DeviceRemoteControlDiscreteValuesMessage.hpp"
#include "protocol/struct/DeviceRemoteControlIsModulatedChangeMessage.hpp"
#include "protocol/struct/DeviceRemoteControlModulatedValueChangeMessage.hpp"
#include "protocol/struct/DeviceRemoteControlNameChangeMessage.hpp"
#include "protocol/struct/DeviceRemoteControlOriginChangeMessage.hpp"
#include "protocol/struct/DeviceRemoteControlsModulatedValuesBatchMessage.hpp"
#include "protocol/struct/DeviceRemoteControlUpdateMessage.hpp"
#include "protocol/struct/DeviceRemoteControlValueChangeMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;

HandlerHostRemoteControl::HandlerHostRemoteControl(state::BitwigState& state,
                                                   BitwigProtocol& protocol,
                                                   oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

void HandlerHostRemoteControl::setupProtocolCallbacks() {
    protocol_.onDeviceRemoteControlUpdate = [this](const DeviceRemoteControlUpdateMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.remoteControlIndex];

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
        auto encoderId = getEncoderIdForParameter(msg.remoteControlIndex);
        if (encoderId != EncoderID{0}) {
            configureEncoderForParameter(encoders_, encoderId,
                                         msg.parameterType,
                                         msg.discreteValueCount,
                                         msg.parameterValue);
        }
    };

    protocol_.onDeviceRemoteControlDiscreteValues = [this](const DeviceRemoteControlDiscreteValuesMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.remoteControlIndex];

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

    protocol_.onDeviceRemoteControlValueChange = [this](const DeviceRemoteControlValueChangeMessage& msg) {
        if (!msg.fromHost) { return; }
        if (msg.remoteControlIndex >= PARAMETER_COUNT) { return; }

        auto& slot = state_.parameters.slots[msg.remoteControlIndex];
        auto currentType = slot.type.get();
        auto encoderId = getEncoderIdForParameter(msg.remoteControlIndex);

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

    protocol_.onDeviceRemoteControlNameChange = [this](const DeviceRemoteControlNameChangeMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;
        state_.parameters.slots[msg.remoteControlIndex].name.set(msg.parameterName.c_str());
    };

    protocol_.onDeviceRemoteControlOriginChange = [this](const DeviceRemoteControlOriginChangeMessage& msg) {
        if (!msg.fromHost) return;
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;
        state_.parameters.slots[msg.remoteControlIndex].origin.set(msg.parameterOrigin);
    };

    // Batch modulated values - updates all 8 ribbon offsets at once
    protocol_.onDeviceRemoteControlsModulatedValuesBatch =
        [this](const DeviceRemoteControlsModulatedValuesBatchMessage& msg) {
            for (size_t i = 0; i < msg.modulatedValues.size() && i < PARAMETER_COUNT; ++i) {
                auto& slot = state_.parameters.slots[i];
                // Store offset so ribbon follows optimistic value updates
                slot.modulationOffset.set(msg.modulatedValues[i] - slot.value.get());
            }
        };

    // isModulated state change - controls ribbon visibility
    protocol_.onDeviceRemoteControlIsModulatedChange =
        [this](const DeviceRemoteControlIsModulatedChangeMessage& msg) {
            if (msg.remoteControlIndex >= PARAMETER_COUNT) return;
            state_.parameters.slots[msg.remoteControlIndex].isModulated.set(msg.isModulated);
        };
}

}  // namespace bitwig::handler
