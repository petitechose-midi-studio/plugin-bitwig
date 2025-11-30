#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <etl/string.h>
#include <etl/vector.h>

namespace Bitwig {

/**
 * @brief State for device, parameters, and page selection
 */
struct DeviceState {
    struct DeviceInfo {
        std::string name = "No Device";
        uint8_t deviceType = 0;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
        bool enabled = false;
        bool hasChildren = false;
        std::string pageName;
    };

    struct ParameterInfo {
        uint8_t type = 0;
        int16_t discreteCount = -1;
        etl::vector<etl::string<16>, 32> discreteValueNames;
        uint8_t currentValueIndex = 0;
        float origin = 0.0f;
        float value = 0.0f;
        std::string displayValue;
        std::string name;
        bool visible = true;
        bool loading = false;
        bool metadataSet = false;
    };

    struct PageSelectorInfo {
        std::vector<std::string> names;
        int currentIndex = 0;
        int selectedIndex = 0;
        bool visible = false;
    };

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
        bool showingChildren = false;
        bool showFooter = false;
        bool visible = false;
    };

    DeviceInfo device;
    std::array<ParameterInfo, 8> parameters;
    PageSelectorInfo pageSelector;
    DeviceSelectorInfo deviceSelector;

    // Dirty tracking
    struct Dirty {
        bool device = false;
        std::array<bool, 8> parameters = {false};
        bool pageSelector = false;
        bool deviceSelector = false;

        void clear() {
            device = false;
            parameters.fill(false);
            pageSelector = false;
            deviceSelector = false;
        }

        bool any() const {
            if (device || pageSelector || deviceSelector)
                return true;
            for (bool p : parameters)
                if (p)
                    return true;
            return false;
        }
    };

    Dirty dirty;

    void markDeviceDirty() { dirty.device = true; }
    void markParameterDirty(uint8_t index) {
        if (index < 8) dirty.parameters[index] = true;
    }
    void markPageSelectorDirty() { dirty.pageSelector = true; }
    void markDeviceSelectorDirty() { dirty.deviceSelector = true; }
    void clearDirty() { dirty.clear(); }
};

}  // namespace Bitwig
