#include "RemoteControlHostHandler.hpp"

#include <algorithm>
#include <cmath>

#include "handler/InputUtils.hpp"
#include "protocol/MessageStructure.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;

RemoteControlHostHandler::RemoteControlHostHandler(state::BitwigState& state,
                                                   BitwigProtocol& protocol,
                                                   oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

void RemoteControlHostHandler::setupProtocolCallbacks() {
    protocol_.onDeviceRemoteControlUpdate = [this](const DeviceRemoteControlUpdateMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.remoteControlIndex];

        slot.type.set(msg.parameterType);
        slot.discreteCount.set(msg.discreteValueCount);
        slot.currentValueIndex.set(msg.currentValueIndex);
        slot.origin.set(msg.parameterOrigin);
        slot.displayValue.set(msg.displayValue.c_str());
        slot.name.set(msg.parameterName.c_str());
        slot.value.set(msg.parameterValue);
        slot.visible.set(msg.parameterExists);
        slot.loading.set(false);
        slot.hasAutomation.set(msg.hasAutomation);

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

    // Value state notification from host (confirmation with display value)
    protocol_.onRemoteControlValueState = [this](const RemoteControlValueStateMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) { return; }

        auto& slot = state_.parameters.slots[msg.remoteControlIndex];
        auto encoderId = getEncoderIdForParameter(msg.remoteControlIndex);

        // Update value and display
        slot.value.set(msg.parameterValue);
        slot.displayValue.set(msg.displayValue.c_str());

        // Update encoder position
        if (encoderId != EncoderID{0}) {
            encoders_.setPosition(encoderId, msg.parameterValue);
        }
    };

    protocol_.onDeviceRemoteControlNameChange = [this](const DeviceRemoteControlNameChangeMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;
        state_.parameters.slots[msg.remoteControlIndex].name.set(msg.parameterName.c_str());
    };

    protocol_.onDeviceRemoteControlOriginChange = [this](const DeviceRemoteControlOriginChangeMessage& msg) {
        if (msg.remoteControlIndex >= PARAMETER_COUNT) return;
        state_.parameters.slots[msg.remoteControlIndex].origin.set(msg.parameterOrigin);
    };

    // Combined batch: values + modulated values in single synchronized update
    protocol_.onDeviceRemoteControlsBatch =
        [this](const DeviceRemoteControlsBatchMessage& msg) {

            for (size_t i = 0; i < PARAMETER_COUNT; ++i) {
                auto& slot = state_.parameters.slots[i];

                // Update automation state from host (source of truth)
                bool hasAutomation = (msg.hasAutomationMask >> i) & 1;
                slot.hasAutomation.set(hasAutomation);

                // Update modulation offset (always, for ribbon display)
                // Store offset so ribbon follows optimistic value updates
                slot.modulationOffset.set(msg.modulatedValues[i] - slot.value.get());

                // Update value only if dirty in this batch
                if (msg.dirtyMask & (1 << i)) {
                    bool isEcho = msg.echoMask & (1 << i);

                    if (isEcho) {
                        // Echo: skip value update for KNOB (optimistic already applied)
                        if (slot.type.get() == ParameterType::KNOB) continue;
                    }

                    float value = msg.values[i];

                    // Update value
                    slot.value.set(value);

                    // For LIST/BUTTON parameters: update currentValueIndex and displayValue
                    auto paramType = slot.type.get();
                    if (paramType == ParameterType::LIST || paramType == ParameterType::BUTTON) {
                        int16_t count = slot.discreteCount.get();
                        if (count > 1) {
                            // Calculate index from normalized value
                            int index = static_cast<int>(std::round(value * (count - 1)));
                            index = std::clamp(index, 0, count - 1);
                            slot.currentValueIndex.set(static_cast<uint8_t>(index));
                        }
                        // Update displayValue from batch (sent by host with correct value from Bitwig)
                        if (!msg.displayValues[i].empty()) {
                            slot.displayValue.set(msg.displayValues[i].c_str());
                        }
                    }

                    // Update encoder position for non-echo
                    if (!isEcho) {
                        auto encoderId = getEncoderIdForParameter(i);
                        if (encoderId != EncoderID{0}) {
                            encoders_.setPosition(encoderId, value);
                        }
                    }
                }
            }
        };

    // isModulated state change - controls ribbon visibility
    protocol_.onDeviceRemoteControlIsModulatedChange =
        [this](const DeviceRemoteControlIsModulatedChangeMessage& msg) {
            if (msg.remoteControlIndex >= PARAMETER_COUNT) return;
            state_.parameters.slots[msg.remoteControlIndex].isModulated.set(msg.isModulated);
        };

    // Note: hasAutomation and automationActive are now updated via batch message
    // (hasAutomationMask and touchedMask fields) for perfect synchronization
}

}  // namespace bitwig::handler
