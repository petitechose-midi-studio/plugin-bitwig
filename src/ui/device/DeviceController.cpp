#include "DeviceController.hpp"

#include "../../protocol/struct/DeviceChangeHeaderMessage.hpp"
#include "../../protocol/struct/DeviceMacroUpdateMessage.hpp"
#include "../../protocol/struct/DeviceMacroDiscreteValuesMessage.hpp"
#include "../../protocol/struct/DevicePageChangeMessage.hpp"
#include "../../protocol/struct/TrackChangeMessage.hpp"
#include "../../protocol/struct/TrackListMessage.hpp"
#include "../../protocol/struct/DeviceListMessage.hpp"
#include "../../handler/device/DeviceConstants.hpp"
#include "../../handler/device/TrackConstants.hpp"
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

    void DeviceController::handleDeviceState(uint8_t deviceIndex, bool enabled)
    {
        // Only update top bar if this is the currently selected device
        if (static_cast<int>(deviceIndex) == current_device_index_)
        {
            view_.state().device.enabled = enabled;
            view_.state().dirty.device = true;
            view_.sync();
        }
    }

    void DeviceController::handleDeviceStateAtIndex(uint8_t deviceIndex, bool enabled)
    {
        view_.updateDeviceState(toDisplayIndex(deviceIndex), enabled);
    }

    void DeviceController::handleTrackChange(const Protocol::TrackChangeMessage &msg)
    {
        auto& track = view_.state().currentTrack;
        track.name = msg.trackName.c_str();
        track.color = msg.color;
        track.trackType = msg.trackType;
        view_.state().dirty.deviceSelector = true;
        view_.sync();
    }

    void DeviceController::handleDeviceChangeHeader(const Protocol::DeviceChangeHeaderMessage &msg)
    {
        // Update hasChildren: true if any childrenTypes element is non-zero
        bool hasChildren = (msg.childrenTypes[0] | msg.childrenTypes[1] |
                           msg.childrenTypes[2] | msg.childrenTypes[3]) != 0;
        current_device_has_children_ = hasChildren;

        // Batch update device state (single sync instead of 6)
        auto& device = view_.state().device;
        device.name = msg.deviceName.c_str();
        device.deviceType = msg.deviceType;
        device.enabled = msg.isEnabled;
        device.pageName = msg.pageInfo.devicePageName.c_str();
        device.hasChildren = hasChildren;
        view_.state().dirty.device = true;

        view_.setAllWidgetsLoading(true);
    }

    void DeviceController::handleCurrentDeviceInfo(uint8_t deviceIndex, bool hasChildren)
    {
        current_device_index_ = deviceIndex;
        current_device_has_children_ = hasChildren;
        view_.state().device.hasChildren = hasChildren;
        view_.state().dirty.device = true;
        view_.sync();
    }

    void DeviceController::handleMacroUpdate(const Protocol::DeviceMacroUpdateMessage &msg)
    {
        if (msg.parameterIndex >= 8)
            return;
        uint8_t i = msg.parameterIndex;

        view_.setParameterTypeAndMetadata(
            i,
            msg.parameterType,       // Widget type (Device::Knob, Device::Button, Device::List)
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
        view_.state().device.pageName = msg.pageInfo.devicePageName.c_str();
        view_.state().dirty.device = true;
        view_.sync();

        for (uint8_t i = 0; i < 8 && i < msg.macros.size(); i++)
        {
            view_.setParameterTypeAndMetadata(
                i,
                msg.macros[i].parameterType,       // Widget type (Device::Knob, Device::Button, Device::List)
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

    void DeviceController::handlePageSelectorConfirm()
    {
        view_.state().pageSelector.visible = false;
        view_.state().dirty.pageSelector = true;
        view_.sync();
    }

    int DeviceController::getPageSelectorSelectedIndex() const
    {
        return view_.getPageSelectorIndex();
    }

    void DeviceController::handleDeviceList(const Protocol::DeviceListMessage &msg)
    {
        std::vector<std::string> names;
        std::vector<uint8_t> types;
        std::vector<bool> states, hasSlots, hasLayers, hasDrums;

        if (msg.isNested)
        {
            names.push_back(Device::BACK_TO_PARENT_TEXT);
            types.push_back(0);  // Unknown type for back button
            states.push_back(false);
            hasSlots.push_back(false);
            hasLayers.push_back(false);
            hasDrums.push_back(false);
        }

        for (uint8_t i = 0; i < msg.deviceCount; i++)
        {
            names.push_back(std::string(msg.devices[i].deviceName.data()));
            types.push_back(msg.devices[i].deviceType);
            states.push_back(msg.devices[i].isEnabled);

            uint8_t flags = Device::getChildTypeFlags(msg.devices[i].childrenTypes);
            hasSlots.push_back(flags & Device::Slots);
            hasLayers.push_back(flags & Device::Layers);
            hasDrums.push_back(flags & Device::Drums);
        }

        is_device_nested_ = msg.isNested;
        current_device_index_ = msg.deviceIndex;

        // Update current device's hasChildren state for top bar folder icon
        if (msg.deviceIndex < msg.deviceCount)
        {
            uint8_t flags = Device::getChildTypeFlags(msg.devices[msg.deviceIndex].childrenTypes);
            current_device_has_children_ = (flags & (Device::Slots | Device::Layers | Device::Drums)) != 0;
            view_.state().device.hasChildren = current_device_has_children_;
            view_.state().dirty.device = true;
            view_.sync();
        }

        view_.showDeviceList(names, toDisplayIndex(msg.deviceIndex), types, states, hasSlots, hasLayers, hasDrums);
    }

    void DeviceController::handleShowDeviceSelectorWithIndicators(
        const std::vector<std::string> &names, int currentIndex,
        const std::vector<bool> &deviceStates, const std::vector<bool> &hasSlots,
        const std::vector<bool> &hasLayers, const std::vector<bool> &hasDrums)
    {
        is_device_nested_ = (!names.empty() && names[0] == Device::BACK_TO_PARENT_TEXT);
        // Pass empty deviceTypes - this legacy method doesn't have type info
        std::vector<uint8_t> emptyTypes(names.size(), 0);
        view_.showDeviceList(names, currentIndex, emptyTypes, deviceStates, hasSlots, hasLayers, hasDrums);
    }

    void DeviceController::handleShowDeviceChildren(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes)
    {
        view_.showDeviceChildren(items, itemTypes);
    }

    void DeviceController::handleDeviceSelectorConfirm()
    {
        view_.state().deviceSelector.visible = false;
        view_.state().dirty.deviceSelector = true;
        view_.sync();
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
        std::vector<uint8_t> trackTypes;
        std::vector<uint32_t> trackColors;

        if (msg.isNested)
        {
            trackNames.push_back(Track::BACK_TO_PARENT_TEXT);
            muteStates.push_back(false);
            soloStates.push_back(false);
            trackTypes.push_back(0); // Default to Audio type for back button
            trackColors.push_back(0xFFFFFF);
        }

        for (uint8_t i = 0; i < msg.trackCount; i++)
        {
            trackNames.push_back(std::string(msg.tracks[i].trackName.data()));
            muteStates.push_back(msg.tracks[i].isMute);
            soloStates.push_back(msg.tracks[i].isSolo);
            trackTypes.push_back(msg.tracks[i].trackType);
            trackColors.push_back(msg.tracks[i].color);
        }

        is_track_nested_ = msg.isNested;

        // Hide device selector just before showing track selector - no visual gap
        view_.state().deviceSelector.visible = false;
        view_.state().dirty.deviceSelector = true;
        view_.showTrackList(trackNames, toTrackDisplayIndex(msg.trackIndex), muteStates, soloStates, trackTypes, trackColors);
    }

    void DeviceController::handleTrackMuteState(uint8_t trackIndex, bool isMuted)
    {
        int displayIndex = toTrackDisplayIndex(trackIndex);
        auto& muteStates = view_.state().trackSelector.muteStates;
        if (displayIndex < 0 || displayIndex >= static_cast<int>(muteStates.size()))
            return;

        muteStates[displayIndex] = isMuted;
        view_.state().dirty.trackSelector = true;
        view_.sync();
    }

    void DeviceController::handleTrackSoloState(uint8_t trackIndex, bool isSoloed)
    {
        int displayIndex = toTrackDisplayIndex(trackIndex);
        auto& soloStates = view_.state().trackSelector.soloStates;
        if (displayIndex < 0 || displayIndex >= static_cast<int>(soloStates.size()))
            return;

        soloStates[displayIndex] = isSoloed;
        view_.state().dirty.trackSelector = true;
        view_.sync();
    }

} // namespace Bitwig
