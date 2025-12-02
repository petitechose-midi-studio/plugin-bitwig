#pragma once

#include <cstdint>

#include <array>
#include <string>
#include <vector>

namespace Bitwig {

/**
 * @brief Complete state for DeviceView including device, parameters, selectors and track info
 *
 * Single source of truth for all device-related UI state.
 * Used by DeviceView and accessed by handlers via view_.state().
 */
struct DeviceState {
    // =========================================================================
    // Device Info
    // =========================================================================
    struct DeviceInfo {
        std::string name = "No Device";
        uint8_t deviceType = 0;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
        bool enabled = false;
        bool hasChildren = false;
        std::string pageName;
    };

    // =========================================================================
    // Parameter Info
    // =========================================================================
    struct ParameterInfo {
        uint8_t type = 0;
        int16_t discreteCount = -1;
        std::vector<std::string> discreteValueNames;
        uint8_t currentValueIndex = 0;
        float origin = 0.0f;
        float value = 0.0f;
        std::string displayValue;
        std::string name;
        bool visible = true;
        bool loading = false;
        bool metadataSet = false;
    };

    // =========================================================================
    // Page Selector
    // =========================================================================
    struct PageSelectorInfo {
        std::vector<std::string> names;
        int selectedIndex = 0;
        bool visible = false;
    };

    // =========================================================================
    // Device Selector
    // =========================================================================
    struct DeviceSelectorInfo {
        std::vector<std::string> names;
        std::vector<uint8_t> deviceTypes;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
        std::vector<bool> deviceStates;
        std::vector<bool> hasSlots;
        std::vector<bool> hasLayers;
        std::vector<bool> hasDrums;
        std::vector<std::string> childrenNames;
        std::vector<uint8_t> childrenTypes;
        int currentIndex = 0;
        int activeDeviceIndex = 0;
        bool isNested = false;
        bool showingChildren = false;
        bool showFooter = false;
        bool visible = false;
    };

    // =========================================================================
    // Current Track (the track whose devices are displayed)
    // =========================================================================
    struct CurrentTrackInfo {
        std::string name;
        uint32_t color = 0xFFFFFF;
        uint8_t trackType = 0;
    };

    // =========================================================================
    // Track Selector
    // =========================================================================
    struct TrackSelectorInfo {
        std::vector<std::string> names;
        std::vector<bool> muteStates;
        std::vector<bool> soloStates;
        std::vector<uint8_t> trackTypes;
        std::vector<uint32_t> trackColors;
        int currentIndex = 0;
        int activeTrackIndex = 0;
        bool isNested = false;
        bool visible = false;
    };

    // =========================================================================
    // State Data
    // =========================================================================
    DeviceInfo device;
    std::array<ParameterInfo, 8> parameters;
    PageSelectorInfo pageSelector;
    DeviceSelectorInfo deviceSelector;
    CurrentTrackInfo currentTrack;
    TrackSelectorInfo trackSelector;

    // =========================================================================
    // Dirty Tracking (for efficient UI updates)
    // =========================================================================
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
            if (device || pageSelector || deviceSelector || trackSelector) return true;
            for (bool p : parameters)
                if (p) return true;
            return false;
        }
    };

    Dirty dirty;
};

}  // namespace Bitwig
