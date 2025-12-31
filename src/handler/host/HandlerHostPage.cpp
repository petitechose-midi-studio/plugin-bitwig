#include "HandlerHostPage.hpp"

#include <array>
#include <vector>

#include "handler/InputUtils.hpp"
#include "protocol/MessageStructure.hpp"
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
        state_.parameters.slots[paramIndex].type.set(remoteControls[i].parameterType);

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
    // Windowed page names (accumulates in cache)
    protocol_.onDevicePageNamesWindow = [this](const DevicePageNamesWindowMessage& msg) {
        OC_LOG_DEBUG("[Page] Window c={}", msg.devicePageCount);

        // Update total count
        state_.pageSelector.totalCount.set(msg.devicePageCount);

        // Accumulate names at absolute indices
        uint8_t startIdx = msg.pageStartIndex;
        for (uint8_t i = 0; i < msg.pageNames.size(); i++) {
            if (msg.pageNames[i].empty()) break;  // End of valid data
            uint8_t absoluteIdx = startIdx + i;
            if (absoluteIdx < MAX_PAGES) {
                state_.pageSelector.names.setAt(absoluteIdx, msg.pageNames[i]);
            }
        }

        // Update loadedUpTo (highest index we've received)
        uint8_t newLoadedUpTo = startIdx + LIST_WINDOW_SIZE;
        if (newLoadedUpTo > msg.devicePageCount) {
            newLoadedUpTo = msg.devicePageCount;  // Cap at total
        }
        if (newLoadedUpTo > state_.pageSelector.loadedUpTo.get()) {
            state_.pageSelector.loadedUpTo.set(newLoadedUpTo);
        }

        // Update selected index ONLY on first window (not on prefetch)
        // This prevents cursor jumps when user is navigating
        if (startIdx == 0) {
            state_.pageSelector.selectedIndex.set(msg.devicePageIndex);
        }
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
            slot.type.set(rc.parameterType);

            // Set remaining display properties
            slot.displayValue.set(rc.displayValue.c_str());
            slot.value.set(rc.parameterValue);
            slot.name.set(rc.parameterName.c_str());
            slot.visible.set(rc.parameterExists);
            slot.loading.set(false);
            slot.metadataSet.set(true);

            // Modulation state - controls ribbon visibility
            // Store offset (not absolute value) so ribbon follows optimistic updates
            slot.isModulated.set(rc.isModulated);
            slot.modulationOffset.set(rc.modulatedValue - rc.parameterValue);

            // Automation state
            slot.hasAutomation.set(rc.hasAutomation);
        }
    };
}

}  // namespace bitwig::handler
