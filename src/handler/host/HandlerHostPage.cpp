#include "HandlerHostPage.hpp"

#include <array>
#include <vector>

#include "handler/InputUtils.hpp"
#include "protocol/struct/DevicePageChangeMessage.hpp"
#include "protocol/struct/DevicePageNamesMessage.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;

HandlerHostPage::HandlerHostPage(state::BitwigState& state,
                                 BitwigProtocol& protocol,
                                 oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

template <typename RemoteControlArray>
void HandlerHostPage::updateRemoteControlEncoderModes(const RemoteControlArray& remoteControls) {
    for (uint8_t i = 0; i < PARAMETER_COUNT && i < remoteControls.size(); i++) {
        uint8_t paramIndex = remoteControls[i].remoteControlIndex;
        if (paramIndex >= PARAMETER_COUNT) continue;

        // Update parameter type in state
        state_.parameters.slots[paramIndex].type.set(
            static_cast<state::ParameterType>(remoteControls[i].parameterType));

        // Configure encoder mode
        auto encoderId = getEncoderIdForParameter(paramIndex);
        if (encoderId != EncoderID{0}) {
            configureEncoderForParameter(encoders_, encoderId,
                                         remoteControls[i].parameterType,
                                         remoteControls[i].discreteValueCount,
                                         remoteControls[i].parameterValue);
        }
    }
}

void HandlerHostPage::setupProtocolCallbacks() {
    protocol_.onDevicePageNames = [this](const DevicePageNamesMessage& msg) {
        if (!msg.fromHost) return;

        std::vector<std::string> names;
        for (uint8_t i = 0; i < msg.devicePageCount; i++) {
            names.push_back(std::string(msg.pageNames[i].data()));
        }

        state_.pageSelector.names.set(names.data(), names.size());
        state_.pageSelector.selectedIndex.set(msg.devicePageIndex);
    };

    protocol_.onDevicePageChange = [this](const DevicePageChangeMessage& msg) {
        updateRemoteControlEncoderModes(msg.remoteControls);

        state_.device.pageName.set(msg.pageInfo.devicePageName.c_str());

        // Local buffer for discrete values (stack allocated, safe in single-threaded context)
        std::array<std::string, state::MAX_DISCRETE_VALUES> tempDiscreteValues;

        for (uint8_t i = 0; i < PARAMETER_COUNT && i < msg.remoteControls.size(); i++) {
            auto& slot = state_.parameters.slots[i];
            const auto& rc = msg.remoteControls[i];

            // IMPORTANT: Set metadata BEFORE type!
            // When type changes, ensureWidgetForType() reads these values to create the widget.
            // If type is set first, discreteCount would still have old/default value.
            slot.discreteCount.set(rc.discreteValueCount);
            slot.currentValueIndex.set(rc.currentValueIndex);
            slot.origin.set(rc.parameterOrigin);

            // Update discrete values BEFORE type (needed for LIST widgets)
            size_t count = 0;
            for (const auto& dv : rc.discreteValueNames) {
                if (!dv.empty() && count < state::MAX_DISCRETE_VALUES) {
                    tempDiscreteValues[count++] = dv;
                }
            }
            slot.discreteValues.set(tempDiscreteValues.data(), count);

            // NOW set type - this triggers widget creation with correct metadata
            slot.type.set(static_cast<state::ParameterType>(rc.parameterType));

            // Set remaining display properties
            slot.displayValue.set(rc.displayValue.c_str());
            slot.value.set(rc.parameterValue);
            slot.name.set(rc.parameterName.c_str());
            slot.visible.set(rc.parameterExists);
            slot.loading.set(false);
            slot.metadataSet.set(true);
        }
    };
}

}  // namespace bitwig::handler
