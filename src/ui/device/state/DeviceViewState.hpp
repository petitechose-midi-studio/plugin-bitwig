#pragma once

#include "ui/state/DeviceState.hpp"

namespace Bitwig {

/**
 * @brief Combined view state for DeviceView
 *
 * Composes DeviceState and track selector info.
 * This maintains backward compatibility while using the new state structure.
 */
struct DeviceViewState {
    // Re-export types from DeviceState for backward compatibility
    using DeviceInfo = DeviceState::DeviceInfo;
    using ParameterInfo = DeviceState::ParameterInfo;
    using PageSelectorInfo = DeviceState::PageSelectorInfo;
    using DeviceSelectorInfo = DeviceState::DeviceSelectorInfo;

    // Track selector info (kept here for now, can migrate to TrackState later)
    struct TrackSelectorInfo {
        std::vector<std::string> names;
        std::vector<bool> muteStates;
        std::vector<bool> soloStates;
        std::vector<uint8_t> trackTypes;
        std::vector<uint32_t> trackColors;
        int currentIndex = 0;
        bool visible = false;
    };

    // Device state (inline composition)
    DeviceInfo device;
    std::array<ParameterInfo, 8> parameters;
    PageSelectorInfo pageSelector;
    DeviceSelectorInfo deviceSelector;

    // Track state (to be migrated)
    TrackSelectorInfo trackSelector;

    // Combined dirty tracking
    struct Dirty {
        bool device = false;
        std::array<bool, 8> parameters = {false};
        bool pageSelector = false;
        bool deviceSelector = false;
        bool trackSelector = false;

        void clear() {
            device = false;
            parameters.fill(false);
            pageSelector = false;
            deviceSelector = false;
            trackSelector = false;
        }

        bool any() const {
            if (device || pageSelector || deviceSelector || trackSelector)
                return true;
            for (bool p : parameters)
                if (p)
                    return true;
            return false;
        }
    };

    Dirty dirty;
};

}  // namespace Bitwig
