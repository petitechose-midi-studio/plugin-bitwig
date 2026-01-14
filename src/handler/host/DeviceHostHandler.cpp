#include "DeviceHostHandler.hpp"

#include <array>
#include <vector>

#include <oc/log/Log.hpp>

#include "handler/NestedIndexUtils.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;
constexpr auto BACK_TO_PARENT = BACK_TO_PARENT_TEXT;

DeviceHostHandler::DeviceHostHandler(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();
}

void DeviceHostHandler::setupProtocolCallbacks() {
    // =========================================================================
    // Device
    // =========================================================================

    protocol_.onDeviceChangeHeader = [this](const DeviceChangeHeaderMessage& msg) {
        bool hasChildren = (msg.childrenTypes[0] | msg.childrenTypes[1] |
                           msg.childrenTypes[2] | msg.childrenTypes[3]) != 0;

        state_.device.name.set(msg.deviceName.c_str());
        state_.device.deviceType.set(msg.deviceType);
        state_.device.enabled.set(msg.isEnabled);
        state_.device.pageName.set(msg.pageInfo.devicePageName.c_str());
        state_.device.hasChildren.set(hasChildren);

        // Mark all parameters as loading
        for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
            state_.parameters.slots[i].loading.set(true);
        }

        // Reset page selector state for new device (windowed loading)
        state_.pageSelector.names.clear();
        state_.pageSelector.totalCount.set(0);
        state_.pageSelector.loadedUpTo.set(0);

        // Preload first window of page names for immediate availability
        OC_LOG_INFO("[DeviceHostHandler] Sending RequestDevicePageNamesWindow(0)");
        protocol_.requestDevicePageNamesWindow(0);
    };

    protocol_.onDeviceEnabledState = [this](const DeviceEnabledStateMessage& msg) {
        int activeIndex = state_.deviceSelector.activeDeviceIndex.get();
        if (static_cast<int>(msg.deviceIndex) == activeIndex) {
            state_.device.enabled.set(msg.isEnabled);
        }

        int displayIndex = utils::toDisplayIndex(msg.deviceIndex, state_.deviceSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_DEVICES) {
            state_.deviceSelector.deviceStates[displayIndex].set(msg.isEnabled);
        }
    };

    // Windowed device list (accumulates in cache)
    protocol_.onDeviceListWindow = [this](const DeviceListWindowMessage& msg) {

        // Mark loading complete (host responded)
        state_.deviceSelector.loading.set(false);

        // Update total count
        state_.deviceSelector.totalCount.set(msg.deviceCount);

        // Update navigation state
        state_.deviceSelector.isNested.set(msg.isNested);

        uint8_t startIdx = msg.deviceStartIndex;

        // On first window, resize to truncate old data if new list is shorter
        if (startIdx == 0) {
            uint8_t displaySize = msg.deviceCount + (msg.isNested ? 1 : 0);
            state_.deviceSelector.names.resize(displaySize);
            state_.deviceSelector.deviceTypes.resize(displaySize);
            state_.deviceSelector.hasSlots.resize(displaySize);
            state_.deviceSelector.hasLayers.resize(displaySize);
            state_.deviceSelector.hasDrums.resize(displaySize);
            state_.deviceSelector.loadedUpTo.set(0);  // Reset for new list
        }

        // Accumulate data at absolute indices
        for (uint8_t i = 0; i < LIST_WINDOW_SIZE; i++) {
            const auto& dev = msg.devices[i];
            if (dev.deviceName.empty()) break;  // End of valid data

            uint8_t absoluteIdx = startIdx + i;

            // Calculate display index (accounts for back button if nested)
            uint8_t displayIdx = msg.isNested ? absoluteIdx + 1 : absoluteIdx;
            if (displayIdx >= MAX_DEVICES) continue;

            // Accumulate at display index
            state_.deviceSelector.names.setAt(displayIdx, dev.deviceName);
            state_.deviceSelector.deviceTypes.setAt(displayIdx, dev.deviceType);
            state_.deviceSelector.deviceStates[displayIdx].set(dev.isEnabled);

            uint8_t flags = getChildTypeFlags(dev.childrenTypes);
            state_.deviceSelector.hasSlots.setAt(displayIdx, (flags & CHILD_TYPE_SLOTS) != 0);
            state_.deviceSelector.hasLayers.setAt(displayIdx, (flags & CHILD_TYPE_LAYERS) != 0);
            state_.deviceSelector.hasDrums.setAt(displayIdx, (flags & CHILD_TYPE_DRUMS) != 0);
        }

        // Add back button if nested and this is first window
        if (msg.isNested && startIdx == 0) {
            state_.deviceSelector.names.setAt(0, BACK_TO_PARENT);
            state_.deviceSelector.deviceTypes.setAt(0, DeviceType::UNKNOWN);
            state_.deviceSelector.deviceStates[0].set(false);
            state_.deviceSelector.hasSlots.setAt(0, false);
            state_.deviceSelector.hasLayers.setAt(0, false);
            state_.deviceSelector.hasDrums.setAt(0, false);
        }

        // Update loadedUpTo (highest index we've received)
        uint8_t newLoadedUpTo = startIdx + LIST_WINDOW_SIZE;
        if (newLoadedUpTo > msg.deviceCount) {
            newLoadedUpTo = msg.deviceCount;  // Cap at total
        }
        if (newLoadedUpTo > state_.deviceSelector.loadedUpTo.get()) {
            state_.deviceSelector.loadedUpTo.set(newLoadedUpTo);
        }

        // Update current selection ONLY on first window (not on prefetch)
        // This prevents cursor jumps when user is navigating
        if (startIdx == 0) {
            state_.deviceSelector.currentIndex.set(msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex);
        }
        state_.deviceSelector.activeDeviceIndex.set(msg.deviceIndex);
        state_.deviceSelector.showingChildren.set(false);

        // Auto-prefetch if currentIndex is beyond loaded data
        // This handles case where device selector opens with cursor already far in list
        uint8_t currentLoadedUpTo = state_.deviceSelector.loadedUpTo.get();
        if (msg.deviceIndex >= currentLoadedUpTo &&
            currentLoadedUpTo < msg.deviceCount) {
            // Request next window to cover current selection
            protocol_.requestDeviceListWindow(currentLoadedUpTo);
        }

        // Update hasChildren for active device
        if (msg.deviceIndex < msg.deviceCount && msg.deviceIndex >= startIdx &&
            msg.deviceIndex < startIdx + LIST_WINDOW_SIZE) {
            uint8_t localIdx = msg.deviceIndex - startIdx;
            uint8_t flags = getChildTypeFlags(msg.devices[localIdx].childrenTypes);
            bool hasChildren = (flags & (CHILD_TYPE_SLOTS | CHILD_TYPE_LAYERS | CHILD_TYPE_DRUMS)) != 0;
            state_.device.hasChildren.set(hasChildren);
        }
    };

    protocol_.onDeviceChildren = [this](const DeviceChildrenMessage& msg) {
        std::vector<std::string> names;
        std::vector<uint8_t> types;

        names.push_back(BACK_TO_PARENT_TEXT);
        types.push_back(0);

        for (uint8_t i = 0; i < msg.childrenCount; i++) {
            names.emplace_back(msg.children[i].childName);
            types.push_back(msg.children[i].itemType);
        }

        state_.deviceSelector.childrenNames.set(names.data(), names.size());
        state_.deviceSelector.childrenTypes.set(types.data(), types.size());
        state_.deviceSelector.currentIndex.set(1);  // Reset to first child (index 0 = back button)
        state_.deviceSelector.showingChildren.set(true);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };
}

}  // namespace bitwig::handler
