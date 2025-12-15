#include "HandlerHostDevice.hpp"

#include <array>
#include <vector>

#include <oc/log/Log.hpp>

#include "handler/NestedIndexUtils.hpp"
#include "protocol/struct/DeviceChangeHeaderMessage.hpp"
#include "protocol/struct/DeviceChildrenMessage.hpp"
#include "protocol/struct/DeviceListMessage.hpp"
#include "protocol/struct/DeviceStateChangeMessage.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;
constexpr auto BACK_TO_PARENT = BACK_TO_PARENT_TEXT;

HandlerHostDevice::HandlerHostDevice(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();
}

void HandlerHostDevice::setupProtocolCallbacks() {
    // =========================================================================
    // Device
    // =========================================================================

    protocol_.onDeviceChangeHeader = [this](const DeviceChangeHeaderMessage& msg) {
        bool hasChildren = (msg.childrenTypes[0] | msg.childrenTypes[1] |
                           msg.childrenTypes[2] | msg.childrenTypes[3]) != 0;

        state_.device.name.set(msg.deviceName.c_str());
        state_.device.deviceType.set(static_cast<state::DeviceType>(msg.deviceType));
        state_.device.enabled.set(msg.isEnabled);
        state_.device.pageName.set(msg.pageInfo.devicePageName.c_str());
        state_.device.hasChildren.set(hasChildren);

        // Mark all parameters as loading
        for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
            state_.parameters.slots[i].loading.set(true);
        }
    };

    protocol_.onDeviceStateChange = [this](const DeviceStateChangeMessage& msg) {
        int activeIndex = state_.deviceSelector.activeDeviceIndex.get();
        if (static_cast<int>(msg.deviceIndex) == activeIndex) {
            state_.device.enabled.set(msg.isEnabled);
        }

        int displayIndex = utils::toDisplayIndex(msg.deviceIndex, state_.deviceSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_DEVICES) {
            state_.deviceSelector.deviceStates[displayIndex].set(msg.isEnabled);
        }
    };

    protocol_.onDeviceList = [this](const DeviceListMessage& msg) {
        if (!msg.fromHost) return;

        // Update active device info
        if (msg.deviceIndex < msg.deviceCount) {
            uint8_t flags = getChildTypeFlags(msg.devices[msg.deviceIndex].childrenTypes);
            bool hasChildren = (flags & (static_cast<uint8_t>(ChildType::SLOTS) |
                                         static_cast<uint8_t>(ChildType::LAYERS) |
                                         static_cast<uint8_t>(ChildType::DRUMS))) != 0;
            state_.device.hasChildren.set(hasChildren);
            state_.deviceSelector.activeDeviceIndex.set(msg.deviceIndex);
        }

        // Update navigation state
        state_.deviceSelector.isNested.set(msg.isNested);

        // Build temporary lists for bulk data
        std::vector<std::string> names;
        std::vector<uint8_t> deviceTypes;
        std::array<bool, MAX_DEVICES> hasSlots{};
        std::array<bool, MAX_DEVICES> hasLayers{};
        std::array<bool, MAX_DEVICES> hasDrums{};

        uint8_t displayIndex = 0;
        if (msg.isNested) {
            names.push_back(BACK_TO_PARENT);
            deviceTypes.push_back(0);
            hasSlots[displayIndex] = false;
            hasLayers[displayIndex] = false;
            hasDrums[displayIndex] = false;
            state_.deviceSelector.deviceStates[displayIndex].set(false);
            displayIndex++;
        }

        for (uint8_t i = 0; i < msg.deviceCount && displayIndex < MAX_DEVICES; i++) {
            OC_LOG_DEBUG("[DeviceList] {} -> deviceType={}",
                        msg.devices[i].deviceName.c_str(), msg.devices[i].deviceType);
            names.push_back(std::string(msg.devices[i].deviceName.data()));
            deviceTypes.push_back(msg.devices[i].deviceType);
            state_.deviceSelector.deviceStates[displayIndex].set(msg.devices[i].isEnabled);

            uint8_t flags = getChildTypeFlags(msg.devices[i].childrenTypes);
            hasSlots[displayIndex] = flags & static_cast<uint8_t>(ChildType::SLOTS);
            hasLayers[displayIndex] = flags & static_cast<uint8_t>(ChildType::LAYERS);
            hasDrums[displayIndex] = flags & static_cast<uint8_t>(ChildType::DRUMS);
            displayIndex++;
        }

        // Update SignalVectors (bulk data)
        state_.deviceSelector.names.set(names.data(), names.size());
        state_.deviceSelector.deviceTypes.set(deviceTypes.data(), deviceTypes.size());
        state_.deviceSelector.hasSlots.set(hasSlots.data(), displayIndex);
        state_.deviceSelector.hasLayers.set(hasLayers.data(), displayIndex);
        state_.deviceSelector.hasDrums.set(hasDrums.data(), displayIndex);

        state_.deviceSelector.currentIndex.set(msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex);
        state_.deviceSelector.showingChildren.set(false);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };

    protocol_.onDeviceChildren = [this](const DeviceChildrenMessage& msg) {
        if (!msg.fromHost) return;

        std::vector<std::string> names;
        std::vector<uint8_t> types;

        names.push_back(BACK_TO_PARENT_TEXT);
        types.push_back(0);

        for (uint8_t i = 0; i < msg.childrenCount; i++) {
            names.push_back(std::string(msg.children[i].childName.data()));
            types.push_back(msg.children[i].itemType);
        }

        state_.deviceSelector.childrenNames.set(names.data(), names.size());
        state_.deviceSelector.childrenTypes.set(types.data(), types.size());
        state_.deviceSelector.showingChildren.set(true);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };
}

}  // namespace bitwig::handler
