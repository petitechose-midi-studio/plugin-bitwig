#include "DeviceHostHandler.hpp"

#include <string>
#include <vector>

#include "../../protocol/struct/DeviceChangeHeaderMessage.hpp"
#include "../../protocol/struct/DeviceChangeMessage.hpp"
#include "../../protocol/struct/DeviceChildrenMessage.hpp"
#include "../../protocol/struct/DeviceListMessage.hpp"
#include "../../protocol/struct/TrackChangeMessage.hpp"
#include "../../protocol/struct/TrackListMessage.hpp"
#include "../../protocol/struct/DeviceMacroDiscreteValuesMessage.hpp"
#include "../../protocol/struct/DeviceMacroNameChangeMessage.hpp"
#include "../../protocol/struct/DeviceMacroUpdateMessage.hpp"
#include "../../protocol/struct/DeviceMacroValueChangeMessage.hpp"
#include "../../protocol/struct/DevicePageChangeMessage.hpp"
#include "../../protocol/struct/DevicePageNamesMessage.hpp"
#include "../../protocol/struct/DeviceStateChangeMessage.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "DeviceConstants.hpp"
#include "TrackConstants.hpp"
#include "DeviceInputHandler.hpp"
#include "MacroInputHandler.hpp"

namespace Bitwig
{

    DeviceHostHandler::DeviceHostHandler(Protocol::Protocol &protocol, ControllerAPI &api,
                                         DeviceView &view, DeviceInputHandler &inputHandler)
        : protocol_(protocol), api_(api), view_(view), input_handler_(inputHandler)
    {
        setupProtocolCallbacks();
    }

    int DeviceHostHandler::toDeviceDisplayIndex(int rawIndex) const
    {
        return view_.state().deviceSelector.isNested ? rawIndex + 1 : rawIndex;
    }

    int DeviceHostHandler::toTrackDisplayIndex(int rawIndex) const
    {
        return view_.state().trackSelector.isNested ? rawIndex + 1 : rawIndex;
    }

    template <typename MacroArray>
    void DeviceHostHandler::updateMacroEncoderPositions(const MacroArray &macros)
    {
        for (uint8_t i = 0; i < Device::PARAMETER_COUNT && i < macros.size(); i++)
        {
            uint8_t paramIndex = macros[i].parameterIndex;
            if (paramIndex < Device::PARAMETER_COUNT)
            {
                parameterTypes_[paramIndex] = macros[i].parameterType;
            }

            EncoderID encoderId = MacroInputHandler::getEncoderIdForParameter(paramIndex);
            if (encoderId != EncoderID(0))
            {
                if (macros[i].parameterType == Device::Knob)
                {
                    api_.setEncoderContinuous(encoderId);
                }
                else
                {
                    api_.setEncoderDiscreteSteps(encoderId, macros[i].discreteValueCount);
                }
                api_.setEncoderPosition(encoderId, macros[i].parameterValue);
            }
        }
    }

    void DeviceHostHandler::setupProtocolCallbacks()
    {
        // =============================================================================
        // Track
        // =============================================================================

        protocol_.onTrackChange = [this](const Protocol::TrackChangeMessage &msg)
        {
            auto &track = view_.state().currentTrack;
            track.name = msg.trackName.c_str();
            track.color = msg.color;
            track.trackType = msg.trackType;
            view_.state().dirty.deviceSelector = true;
            view_.sync();
        };

        protocol_.onTrackList = [this](const Protocol::TrackListMessage &msg)
        {
            if (!msg.fromHost)
                return;

            input_handler_.setTrackListState(msg.trackCount, msg.trackIndex, msg.isNested);

            if (!input_handler_.isTrackListRequested())
                return;

            std::vector<std::string> names;
            std::vector<bool> muteStates;
            std::vector<bool> soloStates;
            std::vector<uint8_t> trackTypes;
            std::vector<uint32_t> trackColors;

            if (msg.isNested)
            {
                names.push_back(Track::BACK_TO_PARENT_TEXT);
                muteStates.push_back(false);
                soloStates.push_back(false);
                trackTypes.push_back(0);
                trackColors.push_back(0xFFFFFF);
            }

            for (uint8_t i = 0; i < msg.trackCount; i++)
            {
                names.push_back(std::string(msg.tracks[i].trackName.data()));
                muteStates.push_back(msg.tracks[i].isMute);
                soloStates.push_back(msg.tracks[i].isSolo);
                trackTypes.push_back(msg.tracks[i].trackType);
                trackColors.push_back(msg.tracks[i].color);
            }

            auto &ts = view_.state().trackSelector;
            ts.names = names;
            ts.muteStates = muteStates;
            ts.soloStates = soloStates;
            ts.trackTypes = trackTypes;
            ts.trackColors = trackColors;
            ts.currentIndex = msg.isNested ? msg.trackIndex + 1 : msg.trackIndex;
            ts.activeTrackIndex = msg.trackIndex;
            ts.isNested = msg.isNested;
            ts.visible = true;

            view_.state().deviceSelector.visible = false;
            view_.state().dirty.deviceSelector = true;
            view_.state().dirty.trackSelector = true;
            view_.sync();
        };

        protocol_.onTrackMute = [this](const Protocol::TrackMuteMessage &msg)
        {
            if (!msg.fromHost)
                return;

            int displayIndex = toTrackDisplayIndex(msg.trackIndex);
            auto &muteStates = view_.state().trackSelector.muteStates;
            if (displayIndex >= 0 && displayIndex < static_cast<int>(muteStates.size()))
            {
                muteStates[displayIndex] = msg.isMute;
                view_.state().dirty.trackSelector = true;
                view_.sync();
            }
        };

        protocol_.onTrackSolo = [this](const Protocol::TrackSoloMessage &msg)
        {
            if (!msg.fromHost)
                return;

            int displayIndex = toTrackDisplayIndex(msg.trackIndex);
            auto &soloStates = view_.state().trackSelector.soloStates;
            if (displayIndex >= 0 && displayIndex < static_cast<int>(soloStates.size()))
            {
                soloStates[displayIndex] = msg.isSolo;
                view_.state().dirty.trackSelector = true;
                view_.sync();
            }
        };

        // =============================================================================
        // Device
        // =============================================================================

        protocol_.onDeviceChangeHeader = [this](const Protocol::DeviceChangeHeaderMessage &msg)
        {
            bool hasChildren = (msg.childrenTypes[0] | msg.childrenTypes[1] |
                                msg.childrenTypes[2] | msg.childrenTypes[3]) != 0;

            auto &device = view_.state().device;
            device.name = msg.deviceName.c_str();
            device.deviceType = msg.deviceType;
            device.enabled = msg.isEnabled;
            device.pageName = msg.pageInfo.devicePageName.c_str();
            device.hasChildren = hasChildren;
            view_.state().dirty.device = true;

            view_.setAllWidgetsLoading(true);
        };

        protocol_.onDeviceStateChange = [this](const Protocol::DeviceStateChangeMessage &msg)
        {
            int activeIndex = view_.state().deviceSelector.activeDeviceIndex;
            if (static_cast<int>(msg.deviceIndex) == activeIndex)
            {
                view_.state().device.enabled = msg.isEnabled;
                view_.state().dirty.device = true;
                view_.sync();
            }

            int displayIndex = toDeviceDisplayIndex(msg.deviceIndex);
            view_.updateDeviceState(displayIndex, msg.isEnabled);
        };

        protocol_.onDeviceList = [this](const Protocol::DeviceListMessage &msg)
        {
            if (!msg.fromHost)
                return;

            etl::array<etl::array<uint8_t, Device::MAX_CHILD_TYPES>, Device::MAX_DEVICES> allChildrenTypes;
            for (uint8_t i = 0; i < msg.deviceCount && i < Device::MAX_DEVICES; i++)
            {
                allChildrenTypes[i] = msg.devices[i].childrenTypes;
            }

            input_handler_.setDeviceListState(msg.deviceCount, msg.deviceIndex, msg.isNested,
                                              allChildrenTypes.data(), msg.deviceCount);

            if (msg.deviceIndex < msg.deviceCount)
            {
                uint8_t flags = Device::getChildTypeFlags(msg.devices[msg.deviceIndex].childrenTypes);
                bool hasChildren = (flags & (Device::Slots | Device::Layers | Device::Drums)) != 0;
                view_.state().device.hasChildren = hasChildren;
                view_.state().deviceSelector.activeDeviceIndex = msg.deviceIndex;
                view_.state().dirty.device = true;
            }

            if (!input_handler_.isDeviceListRequested())
            {
                view_.sync();
                return;
            }

            std::vector<std::string> names;
            std::vector<uint8_t> types;
            std::vector<bool> states, hasSlots, hasLayers, hasDrums;

            if (msg.isNested)
            {
                names.push_back(Device::BACK_TO_PARENT_TEXT);
                types.push_back(0);
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

            auto &ds = view_.state().deviceSelector;
            ds.names = names;
            ds.deviceTypes = types;
            ds.deviceStates = states;
            ds.hasSlots = hasSlots;
            ds.hasLayers = hasLayers;
            ds.hasDrums = hasDrums;
            ds.currentIndex = msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex;
            ds.isNested = msg.isNested;
            ds.showingChildren = false;
            ds.showFooter = true;
            ds.visible = true;

            view_.state().dirty.deviceSelector = true;
            view_.sync();
        };

        protocol_.onDeviceChildren = [this](const Protocol::DeviceChildrenMessage &msg)
        {
            if (!msg.fromHost)
                return;

            std::vector<std::string> formattedItems;
            std::vector<uint8_t> itemTypes;
            std::vector<uint8_t> childIndices;

            formattedItems.push_back(Device::BACK_TO_PARENT_TEXT);
            itemTypes.push_back(0);

            for (uint8_t i = 0; i < msg.childrenCount; i++)
            {
                formattedItems.push_back(std::string(msg.children[i].childName.data()));
                itemTypes.push_back(msg.children[i].itemType);
                childIndices.push_back(msg.children[i].childIndex);
            }

            std::vector<uint8_t> childItemTypes(itemTypes.begin() + 1, itemTypes.end());
            input_handler_.setDeviceChildrenState(msg.deviceIndex, 0, msg.childrenCount, childItemTypes, childIndices);

            auto &ds = view_.state().deviceSelector;
            ds.childrenNames = formattedItems;
            ds.childrenTypes = itemTypes;
            ds.showingChildren = true;
            ds.showFooter = false;
            ds.visible = true;
            view_.state().dirty.deviceSelector = true;
            view_.sync();
        };

        // =============================================================================
        // Page
        // =============================================================================

        protocol_.onDevicePageNames = [this](const Protocol::DevicePageNamesMessage &msg)
        {
            if (!msg.fromHost)
                return;

            input_handler_.setPageSelectionState(msg.devicePageCount, msg.devicePageIndex);

            std::vector<std::string> pageNames;
            pageNames.reserve(msg.devicePageCount);
            for (uint8_t i = 0; i < msg.devicePageCount; i++)
            {
                pageNames.push_back(std::string(msg.pageNames[i].data()));
            }

            auto &ps = view_.state().pageSelector;
            ps.names = pageNames;
            ps.selectedIndex = msg.devicePageIndex;
            ps.visible = true;
            view_.state().dirty.pageSelector = true;
            view_.sync();
        };

        protocol_.onDevicePageChange = [this](const Protocol::DevicePageChangeMessage &msg)
        {
            updateMacroEncoderPositions(msg.macros);

            view_.state().device.pageName = msg.pageInfo.devicePageName.c_str();
            view_.state().dirty.device = true;

            for (uint8_t i = 0; i < 8 && i < msg.macros.size(); i++)
            {
                view_.setParameterTypeAndMetadata(
                    i,
                    msg.macros[i].parameterType,
                    msg.macros[i].discreteValueCount,
                    msg.macros[i].discreteValueNames,
                    msg.macros[i].currentValueIndex,
                    msg.macros[i].parameterOrigin,
                    msg.macros[i].displayValue.c_str());

                view_.setParameterValueWithDisplay(i, msg.macros[i].parameterValue, msg.macros[i].displayValue.c_str());
                view_.setParameterName(i, msg.macros[i].parameterName.c_str());
                view_.setParameterVisible(i, msg.macros[i].parameterExists);
            }
        };

        // =============================================================================
        // Macro Parameters
        // =============================================================================

        protocol_.onDeviceMacroUpdate = [this](const Protocol::DeviceMacroUpdateMessage &msg)
        {
            if (msg.parameterIndex < Device::PARAMETER_COUNT)
            {
                parameterTypes_[msg.parameterIndex] = msg.parameterType;
            }

            EncoderID encoderId = MacroInputHandler::getEncoderIdForParameter(msg.parameterIndex);
            if (encoderId != EncoderID(0))
            {
                if (msg.parameterType == Device::Knob)
                {
                    api_.setEncoderContinuous(encoderId);
                }
                else
                {
                    api_.setEncoderDiscreteSteps(encoderId, msg.discreteValueCount);
                }
                api_.setEncoderPosition(encoderId, msg.parameterValue);
            }

            view_.setParameterTypeAndMetadata(
                msg.parameterIndex,
                msg.parameterType,
                msg.discreteValueCount,
                {},
                msg.currentValueIndex,
                msg.parameterOrigin,
                msg.displayValue.c_str());

            view_.setParameterName(msg.parameterIndex, msg.parameterName.c_str());
            view_.setParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
            view_.setParameterVisible(msg.parameterIndex, msg.parameterExists);
            view_.setParameterLoading(msg.parameterIndex, false);
        };

        protocol_.onDeviceMacroDiscreteValues = [this](const Protocol::DeviceMacroDiscreteValuesMessage &msg)
        {
            view_.setParameterDiscreteValues(msg.parameterIndex, msg.discreteValueNames, msg.currentValueIndex);
        };

        protocol_.onDeviceMacroValueChange = [this](const Protocol::DeviceMacroValueChangeMessage &msg)
        {
            if (!msg.fromHost)
                return;

            uint8_t paramType = (msg.parameterIndex < Device::PARAMETER_COUNT)
                                    ? parameterTypes_[msg.parameterIndex]
                                    : Device::Knob;

            if (msg.isEcho)
            {
                if (paramType != Device::Knob)
                {
                    view_.setParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
                }
            }
            else
            {
                EncoderID encoderId = MacroInputHandler::getEncoderIdForParameter(msg.parameterIndex);
                if (encoderId != EncoderID(0))
                {
                    api_.setEncoderPosition(encoderId, msg.parameterValue);
                }
                view_.setParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
            }
        };

        protocol_.onDeviceMacroNameChange = [this](const Protocol::DeviceMacroNameChangeMessage &msg)
        {
            view_.setParameterName(msg.parameterIndex, msg.parameterName.c_str());
        };
    }

} // namespace Bitwig
