#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include <etl/string.h>
#include <etl/vector.h>

namespace Bitwig
{

struct DeviceViewState
{
    struct DeviceInfo
    {
        std::string name = "No Device";
        bool enabled = false;
        bool hasChildren = false;
        std::string pageName;
    };

    struct ParameterInfo
    {
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

    struct PageSelectorInfo
    {
        std::vector<std::string> names;
        int currentIndex = 0;
        int selectedIndex = 0;
        bool visible = false;
    };

    struct DeviceSelectorInfo
    {
        std::vector<std::string> names;
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

    struct TrackSelectorInfo
    {
        std::vector<std::string> names;
        std::vector<bool> muteStates;
        std::vector<bool> soloStates;
        std::vector<uint8_t> trackTypes;
        std::vector<uint32_t> trackColors;
        int currentIndex = 0;
        bool visible = false;
    };

    DeviceInfo device;
    std::array<ParameterInfo, 8> parameters;
    PageSelectorInfo pageSelector;
    DeviceSelectorInfo deviceSelector;
    TrackSelectorInfo trackSelector;

    struct Dirty
    {
        bool device = false;
        std::array<bool, 8> parameters = {false};
        bool pageSelector = false;
        bool deviceSelector = false;
        bool trackSelector = false;

        void clear()
        {
            device = false;
            parameters.fill(false);
            pageSelector = false;
            deviceSelector = false;
            trackSelector = false;
        }

        bool any() const
        {
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

} // namespace Bitwig
