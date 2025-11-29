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
#include "../../protocol/struct/DevicePageSelectByIndexMessage.hpp"
#include "../../protocol/struct/DeviceStateChangeMessage.hpp"
#include "../../protocol/struct/RequestDevicePageNamesMessage.hpp"
#include "../../ui/device/DeviceController.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "DeviceConstants.hpp"
#include "TrackConstants.hpp"
#include "DeviceInputHandler.hpp"
#include "MacroInputHandler.hpp"

namespace Bitwig
{

    DeviceHostHandler::DeviceHostHandler(Protocol::Protocol &protocol, ControllerAPI &api,
                                         DeviceView &view, DeviceController &controller,
                                         DeviceInputHandler &inputHandler)
        : protocol_(protocol), api_(api), view_(view), view_controller_(controller), input_handler_(inputHandler)
    {
        setupProtocolCallbacks();
    }

    template <typename MacroArray>
    void DeviceHostHandler::updateMacroEncoderPositions(const MacroArray &macros)
    {
        for (uint8_t i = 0; i < Device::PARAMETER_COUNT && i < macros.size(); i++)
        {
            // Cache parameter types for echo handling
            uint8_t paramIndex = macros[i].parameterIndex;
            if (paramIndex < Device::PARAMETER_COUNT)
            {
                parameterTypes_[paramIndex] = macros[i].parameterType;
            }

            EncoderID encoderId = MacroInputHandler::getEncoderIdForParameter(paramIndex);
            if (encoderId != EncoderID(0))
            {
                // Configure encoder quantization based on parameter type
                if (macros[i].parameterType == Device::Knob)
                {
                    // Continuous parameter - emit all value changes
                    api_.setEncoderContinuous(encoderId);
                }
                else
                {
                    // Discrete parameter (Button or List) - quantize to discrete steps
                    api_.setEncoderDiscreteSteps(encoderId, macros[i].discreteValueCount);
                }

                api_.setEncoderPosition(encoderId, macros[i].parameterValue);
            }
        }
    }

    void DeviceHostHandler::setupProtocolCallbacks()
    {
        // Track context change handler (granular track updates)
        protocol_.onTrackChange = [this](const Protocol::TrackChangeMessage &msg)
        {
            view_controller_.handleTrackChange(msg);
        };

        protocol_.onDeviceChangeHeader = [this](const Protocol::DeviceChangeHeaderMessage &msg)
        {
            view_controller_.handleDeviceChangeHeader(msg);
        };

        protocol_.onDeviceMacroUpdate = [this](const Protocol::DeviceMacroUpdateMessage &msg)
        {

            // Cache parameter type for echo handling
            if (msg.parameterIndex < Device::PARAMETER_COUNT)
            {
                parameterTypes_[msg.parameterIndex] = msg.parameterType;
            }

            // Configure encoder based on parameter type
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

            // Update UI widget
            view_controller_.handleMacroUpdate(msg);
        };

        // Discrete values handler (for List parameters)
        protocol_.onDeviceMacroDiscreteValues =
            [this](const Protocol::DeviceMacroDiscreteValuesMessage &msg)
        {
            view_controller_.handleMacroDiscreteValues(msg);
        };

        protocol_.onDeviceMacroValueChange =
            [this](const Protocol::DeviceMacroValueChangeMessage &msg)
        {
            if (!msg.fromHost)
            {
                return;
            }

            uint8_t paramType = (msg.parameterIndex < Device::PARAMETER_COUNT)
                                    ? parameterTypes_[msg.parameterIndex]
                                    : Device::Knob;

            if (msg.isEcho)
            {
                // Echo of controller-initiated change
                // - Knob: skip (already applied optimistically)
                // - Button/List: apply (no optimistic update was done, need displayValue)
                if (paramType != Device::Knob)
                {
                    view_controller_.handleParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
                }
            }
            else
            {
                // Host-initiated change: apply to encoder and UI
                EncoderID encoderId = MacroInputHandler::getEncoderIdForParameter(msg.parameterIndex);
                if (encoderId != EncoderID(0))
                {
                    api_.setEncoderPosition(encoderId, msg.parameterValue);
                }

                view_controller_.handleParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
            }
        };

        protocol_.onDevicePageChange = [this](const Protocol::DevicePageChangeMessage &msg)
        {
            updateMacroEncoderPositions(msg.macros);
            view_controller_.handlePageChange(msg);
        };

        protocol_.onDeviceMacroNameChange = [this](const Protocol::DeviceMacroNameChangeMessage &msg)
        {
            view_controller_.handleParameterName(msg.parameterIndex, msg.parameterName.c_str());
        };

        protocol_.onDeviceStateChange = [this](const Protocol::DeviceStateChangeMessage &msg)
        {
            // Update main device view state indicator (only if it's the current device)
            view_controller_.handleDeviceState(msg.deviceIndex, msg.isEnabled);

            // Update device list overlay if visible
            view_controller_.handleDeviceStateAtIndex(msg.deviceIndex, msg.isEnabled);
        };

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

            view_controller_.handleShowPageSelector(pageNames, msg.devicePageIndex);
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

            input_handler_.setDeviceListState(msg.deviceCount,
                                              msg.deviceIndex,
                                              msg.isNested,
                                              allChildrenTypes.data(),
                                              msg.deviceCount);

            // Always update current device index and hasChildren state for top bar
            if (msg.deviceIndex < msg.deviceCount)
            {
                uint8_t flags = Device::getChildTypeFlags(msg.devices[msg.deviceIndex].childrenTypes);
                bool hasChildren = (flags & (Device::Slots | Device::Layers | Device::Drums)) != 0;
                view_controller_.handleCurrentDeviceInfo(msg.deviceIndex, hasChildren);
            }

            if (!input_handler_.isDeviceListRequested())
            {
                return;
            }

            view_controller_.handleDeviceList(msg);
        };

        protocol_.onDeviceChildren = [this](const Protocol::DeviceChildrenMessage &msg)
        {
            if (!msg.fromHost)
                return;

            std::vector<std::string> formattedItems;
            std::vector<uint8_t> itemTypes;
            std::vector<uint8_t> childIndices;

            formattedItems.push_back(Device::BACK_TO_PARENT_TEXT);
            itemTypes.push_back(0); // BACK button has type 0

            for (uint8_t i = 0; i < msg.childrenCount; i++)
            {
                std::string formatted = std::string(msg.children[i].childName.data());
                formattedItems.push_back(formatted);
                itemTypes.push_back(msg.children[i].itemType);
                childIndices.push_back(msg.children[i].childIndex);  // Store real childIndex (midiNote for drums)
            }

            // Pass itemTypes and childIndices to input handler (excluding BACK button at index 0)
            std::vector<uint8_t> childItemTypes(itemTypes.begin() + 1, itemTypes.end());
            input_handler_.setDeviceChildrenState(msg.deviceIndex, 0, msg.childrenCount, childItemTypes, childIndices);

            view_controller_.handleShowDeviceChildren(formattedItems, itemTypes);
        };

        // Track list handler
        protocol_.onTrackList = [this](const Protocol::TrackListMessage &msg)
        {
            if (!msg.fromHost)
                return;

            // Update input handler state
            input_handler_.setTrackListState(msg.trackCount, msg.trackIndex, msg.isNested);

            // Only display if explicitly requested by user
            if (!input_handler_.isTrackListRequested())
            {
                return;
            }

            // Delegate to controller for display
            view_controller_.handleTrackList(msg);
        };

        // Track mute state handler
        protocol_.onTrackMute = [this](const Protocol::TrackMuteMessage &msg)
        {
            if (!msg.fromHost)
                return;

            // Update view
            view_controller_.handleTrackMuteState(msg.trackIndex, msg.isMute);
        };

        // Track solo state handler
        protocol_.onTrackSolo = [this](const Protocol::TrackSoloMessage &msg)
        {
            if (!msg.fromHost)
                return;

            // Update view
            view_controller_.handleTrackSoloState(msg.trackIndex, msg.isSolo);
        };
    }

} // namespace Bitwig
