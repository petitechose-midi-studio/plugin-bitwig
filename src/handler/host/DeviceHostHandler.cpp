#include "DeviceHostHandler.hpp"

#include <array>
#include <vector>

#include <oc/log/Log.hpp>

#include "handler/NestedIndexUtils.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;
DeviceHostHandler::DeviceHostHandler(state::DeviceInfoState& device, state::ParameterState& parameters,
                                     state::PageSelectorState& pageSelector,
                                     state::DeviceSelectorState& deviceSelector, BitwigProtocol& protocol,
                                     const char* backToParentLabel)
    : device_(device), parameters_(parameters), pageSelector_(pageSelector),
      deviceSelector_(deviceSelector), protocol_(protocol), backToParentLabel_(backToParentLabel) {
    setupProtocolCallbacks();
}

void DeviceHostHandler::setupProtocolCallbacks() {
    // =========================================================================
    // Device
    // =========================================================================

    protocol_.onDeviceChangeHeader = [this](const DeviceChangeHeaderMessage& msg) {
        bool hasChildren = (msg.childrenTypes[0] | msg.childrenTypes[1] |
                           msg.childrenTypes[2] | msg.childrenTypes[3]) != 0;

        device_.name.set(msg.deviceName.c_str());
        device_.deviceType.set(msg.deviceType);
        device_.enabled.set(msg.isEnabled);
        device_.pageName.set(msg.pageInfo.devicePageName.c_str());
        device_.hasChildren.set(hasChildren);

        // Mark all parameters as loading
        for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
            parameters_.slots[i].loading.set(true);
        }

        // Reset page selector state for new device (windowed loading)
        pageSelector_.names.clear();
        pageSelector_.totalCount.set(0);
        pageSelector_.loadedUpTo.set(0);

        // Preload first window of page names for immediate availability
        OC_LOG_INFO("[DeviceHostHandler] Sending RequestDevicePageNamesWindow(0)");
        protocol_.requestDevicePageNamesWindow(0);
    };

    protocol_.onDeviceEnabledState = [this](const DeviceEnabledStateMessage& msg) {
        int activeIndex = deviceSelector_.activeDeviceIndex.get();
        if (static_cast<int>(msg.deviceIndex) == activeIndex) {
            device_.enabled.set(msg.isEnabled);
        }

        int displayIndex = utils::toDisplayIndex(msg.deviceIndex, deviceSelector_.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_DEVICES) {
            deviceSelector_.deviceStates[displayIndex].set(msg.isEnabled);
        }
    };

    // Windowed device list (accumulates in cache)
    protocol_.onDeviceListWindow = [this](const DeviceListWindowMessage& msg) {

        // Mark loading complete (host responded)
        deviceSelector_.loading.set(false);

        // Update total count
        deviceSelector_.totalCount.set(msg.deviceCount);

        // Update navigation state
        deviceSelector_.isNested.set(msg.isNested);

        uint8_t startIdx = msg.deviceStartIndex;

        // On first window, resize to truncate old data if new list is shorter
        if (startIdx == 0) {
            uint8_t displaySize = msg.deviceCount + (msg.isNested ? 1 : 0);
            deviceSelector_.names.resize(displaySize);
            deviceSelector_.deviceTypes.resize(displaySize);
            deviceSelector_.hasSlots.resize(displaySize);
            deviceSelector_.hasLayers.resize(displaySize);
            deviceSelector_.hasDrums.resize(displaySize);
            deviceSelector_.loadedUpTo.set(0);  // Reset for new list
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
            deviceSelector_.names.setAt(displayIdx, dev.deviceName);
            deviceSelector_.deviceTypes.setAt(displayIdx, dev.deviceType);
            deviceSelector_.deviceStates[displayIdx].set(dev.isEnabled);

            uint8_t flags = getChildTypeFlags(dev.childrenTypes);
            deviceSelector_.hasSlots.setAt(displayIdx, (flags & CHILD_TYPE_SLOTS) != 0);
            deviceSelector_.hasLayers.setAt(displayIdx, (flags & CHILD_TYPE_LAYERS) != 0);
            deviceSelector_.hasDrums.setAt(displayIdx, (flags & CHILD_TYPE_DRUMS) != 0);
        }

        // Add back button if nested and this is first window
        if (msg.isNested && startIdx == 0) {
            deviceSelector_.names.setAt(0, backToParentLabel_);
            deviceSelector_.deviceTypes.setAt(0, DeviceType::UNKNOWN);
            deviceSelector_.deviceStates[0].set(false);
            deviceSelector_.hasSlots.setAt(0, false);
            deviceSelector_.hasLayers.setAt(0, false);
            deviceSelector_.hasDrums.setAt(0, false);
        }

        // Update loadedUpTo (highest index we've received)
        uint8_t newLoadedUpTo = startIdx + LIST_WINDOW_SIZE;
        if (newLoadedUpTo > msg.deviceCount) {
            newLoadedUpTo = msg.deviceCount;  // Cap at total
        }
        if (newLoadedUpTo > deviceSelector_.loadedUpTo.get()) {
            deviceSelector_.loadedUpTo.set(newLoadedUpTo);
        }

        // Update current selection ONLY on first window (not on prefetch)
        // This prevents cursor jumps when user is navigating
        if (startIdx == 0) {
            deviceSelector_.currentIndex.set(msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex);
        }
        deviceSelector_.activeDeviceIndex.set(msg.deviceIndex);
        deviceSelector_.showingChildren.set(false);

        // Auto-prefetch if currentIndex is beyond loaded data
        // This handles case where device selector opens with cursor already far in list
        uint8_t currentLoadedUpTo = deviceSelector_.loadedUpTo.get();
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
            device_.hasChildren.set(hasChildren);
        }
    };

    protocol_.onDeviceChildren = [this](const DeviceChildrenMessage& msg) {
        std::vector<std::string> names;
        std::vector<uint8_t> types;

        names.push_back(backToParentLabel_);
        types.push_back(0);

        for (uint8_t i = 0; i < msg.childrenCount; i++) {
            names.emplace_back(msg.children[i].childName);
            types.push_back(msg.children[i].itemType);
        }

        deviceSelector_.childrenNames.set(names.data(), names.size());
        deviceSelector_.childrenTypes.set(types.data(), types.size());
        deviceSelector_.currentIndex.set(1);  // Reset to first child (index 0 = back button)
        deviceSelector_.showingChildren.set(true);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };
}

}  // namespace bitwig::handler
