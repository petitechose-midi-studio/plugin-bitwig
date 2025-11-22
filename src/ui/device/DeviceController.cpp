#include "DeviceController.hpp"

#include "../../protocol/struct/DeviceChangeHeaderMessage.hpp"
#include "../../protocol/struct/DeviceMacroUpdateMessage.hpp"
#include "../../protocol/struct/DeviceMacroDiscreteValuesMessage.hpp"
#include "../../protocol/struct/DevicePageChangeMessage.hpp"
#include "../../protocol/struct/TrackChangeMessage.hpp"
#include "../../protocol/struct/TrackListMessage.hpp"
#include "../../protocol/struct/DeviceListMessage.hpp"
#include "../../handler/device/DeviceConstants.hpp"
#include "DeviceView.hpp"
#include "component/DeviceSelector.hpp"
#include "component/PageSelector.hpp"

namespace Bitwig
{

    DeviceController::DeviceController(DeviceView &view) : view_(view) {}

    void DeviceController::handleParameterValue(uint8_t paramIndex, float normalizedValue)
    {
        view_.setParameterValue(paramIndex, normalizedValue);
    }

    void DeviceController::handleParameterValueWithDisplay(uint8_t paramIndex, float normalizedValue, const char *displayValue)
    {
        view_.setParameterValueWithDisplay(paramIndex, normalizedValue, displayValue);
    }

    void DeviceController::handleParameterName(uint8_t paramIndex, const char *name)
    {
        view_.setParameterName(paramIndex, name);
    }

    void DeviceController::handleDeviceState(bool enabled)
    {
        view_.setDeviceEnabled(enabled);
    }

    void DeviceController::handleDeviceStateAtIndex(uint8_t deviceIndex, bool enabled)
    {
        view_.updateDeviceState(toDisplayIndex(deviceIndex), enabled);
    }

    void DeviceController::handleTrackChange(const Protocol::TrackChangeMessage &msg)
    {
        view_.setTrackName(msg.trackName.c_str());
        view_.setTrackColor(msg.color);
    }

    void DeviceController::handleDeviceChangeHeader(const Protocol::DeviceChangeHeaderMessage &msg)
    {
        view_.setDeviceName(msg.deviceName.c_str());
        view_.setDeviceEnabled(msg.isEnabled);
        view_.setDevicePageName(msg.pageInfo.devicePageName.c_str());
        view_.setDevicePageInfo(msg.pageInfo.devicePageIndex, msg.pageInfo.devicePageCount);
        view_.setAllWidgetsLoading(true);
    }

    void DeviceController::handleMacroUpdate(const Protocol::DeviceMacroUpdateMessage &msg)
    {
        if (msg.parameterIndex >= 8)
            return;
        uint8_t i = msg.parameterIndex;

        view_.setParameterTypeAndMetadata(
            i,
            msg.parameterType,       // Widget type (0=Knob, 1=Button, 2=List)
            msg.discreteValueCount,  // Discrete value count
            {},                      // Empty discrete names (will come via handleMacroDiscreteValues)
            msg.currentValueIndex,   // Current index for encoder navigation
            msg.parameterOrigin,     // Knob origin (0.0 or 0.5)
            msg.displayValue.c_str() // Display value
        );

        // Set values
        view_.setParameterName(i, msg.parameterName.c_str());
        view_.setParameterValueWithDisplay(i, msg.parameterValue, msg.displayValue.c_str());
        view_.setParameterVisible(i, msg.parameterExists);
        view_.setParameterLoading(i, false);
    }

    void DeviceController::handleMacroDiscreteValues(const Protocol::DeviceMacroDiscreteValuesMessage &msg)
    {
        if (msg.parameterIndex >= 8)
            return;
        view_.setParameterDiscreteValues(msg.parameterIndex, msg.discreteValueNames, msg.currentValueIndex);
    }

    void DeviceController::handlePageChange(const Protocol::DevicePageChangeMessage &msg)
    {
        view_.setDevicePageName(msg.pageInfo.devicePageName.c_str());
        view_.setDevicePageInfo(msg.pageInfo.devicePageIndex, msg.pageInfo.devicePageCount);

        for (uint8_t i = 0; i < 8 && i < msg.macros.size(); i++)
        {
            view_.setParameterTypeAndMetadata(
                i,
                msg.macros[i].parameterType,       // Widget type detected by Bitwig (0=Knob, 1=Button, 2=List)
                msg.macros[i].discreteValueCount,  // Discrete value count
                msg.macros[i].discreteValueNames,  // Array of discrete value names
                msg.macros[i].currentValueIndex,   // Current index in discreteValueNames
                msg.macros[i].parameterOrigin,     // Knob origin (0.0 or 0.5)
                msg.macros[i].displayValue.c_str() // Initial display value
            );

            // Set initial value with display text
            view_.setParameterValueWithDisplay(i, msg.macros[i].parameterValue, msg.macros[i].displayValue.c_str());
            view_.setParameterName(i, msg.macros[i].parameterName.c_str());
            view_.setParameterVisible(i, msg.macros[i].parameterExists);
        }
    }

    void DeviceController::handleShowPageSelector(const std::vector<std::string> &pageNames, int currentIndex)
    {
        view_.showPageSelector(pageNames, currentIndex);
    }

    void DeviceController::handlePageSelectorSetIndex(int index)
    {
        view_.setPageSelectorIndex(index);
    }

    void DeviceController::handlePageSelectorConfirm()
    {
        view_.hidePageSelector();
    }

    int DeviceController::getPageSelectorSelectedIndex() const
    {
        return view_.getPageSelectorIndex();
    }

    void DeviceController::handleDeviceList(const Protocol::DeviceListMessage &msg)
    {
        std::vector<std::string> names;
        std::vector<bool> states, hasSlots, hasLayers, hasDrums;

        if (msg.isNested)
        {
            names.push_back(Device::BACK_TO_PARENT_TEXT);
            states.push_back(false);
            hasSlots.push_back(false);
            hasLayers.push_back(false);
            hasDrums.push_back(false);
        }

        for (uint8_t i = 0; i < msg.deviceCount; i++)
        {
            names.push_back(std::string(msg.devices[i].deviceName.data()));
            states.push_back(msg.devices[i].isEnabled);

            uint8_t flags = Device::getChildTypeFlags(msg.devices[i].childrenTypes);
            hasSlots.push_back(flags & Device::Slots);
            hasLayers.push_back(flags & Device::Layers);
            hasDrums.push_back(flags & Device::Drums);
        }

        is_device_nested_ = msg.isNested;

        view_.showDeviceList(names, toDisplayIndex(msg.deviceIndex), states, hasSlots, hasLayers, hasDrums);
    }

    void DeviceController::handleShowDeviceSelectorWithIndicators(
        const std::vector<std::string> &names, int currentIndex,
        const std::vector<bool> &deviceStates, const std::vector<bool> &hasSlots,
        const std::vector<bool> &hasLayers, const std::vector<bool> &hasDrums)
    {
        is_device_nested_ = (!names.empty() && names[0] == Device::BACK_TO_PARENT_TEXT);
        view_.showDeviceList(names, currentIndex, deviceStates, hasSlots, hasLayers, hasDrums);
    }

    void DeviceController::handleShowDeviceChildren(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes)
    {
        view_.showDeviceChildren(items, itemTypes);
    }

    void DeviceController::handleDeviceSelectorSetIndex(int index)
    {
        view_.setDeviceSelectorIndex(index);
    }

    void DeviceController::handleDeviceSelectorConfirm()
    {
        view_.hideDeviceSelector();
    }

    int DeviceController::getDeviceSelectorSelectedIndex() const
    {
        return view_.getDeviceSelectorIndex();
    }

    void DeviceController::handleTrackList(const Protocol::TrackListMessage &msg)
    {
        std::vector<std::string> trackNames;
        std::vector<bool> muteStates;
        std::vector<bool> soloStates;
        std::vector<bool> groupStates;

        if (msg.isNested)
        {
            trackNames.push_back("Back to parent");
            muteStates.push_back(false);
            soloStates.push_back(false);
            groupStates.push_back(false);
        }

        for (uint8_t i = 0; i < msg.trackCount; i++)
        {
            trackNames.push_back(std::string(msg.tracks[i].trackName.data()));
            muteStates.push_back(msg.tracks[i].isMute);
            soloStates.push_back(msg.tracks[i].isSolo);
            groupStates.push_back(msg.tracks[i].isGroup);
        }

        is_track_nested_ = msg.isNested;

        view_.showTrackList(trackNames, toTrackDisplayIndex(msg.trackIndex), muteStates, soloStates, groupStates);
    }

    void DeviceController::handleTrackMuteState(uint8_t trackIndex, bool isMuted)
    {
        view_.updateTrackMuteState(toTrackDisplayIndex(trackIndex), isMuted);
    }

    void DeviceController::handleTrackSoloState(uint8_t trackIndex, bool isSoloed)
    {
        view_.updateTrackSoloState(toTrackDisplayIndex(trackIndex), isSoloed);
    }

} // namespace Bitwig
