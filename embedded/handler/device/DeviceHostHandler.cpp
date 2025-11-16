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
#include "log/Macros.hpp"

namespace Plugin::Bitwig {

DeviceHostHandler::DeviceHostHandler(Protocol::Protocol& protocol, ControllerAPI& api,
                                     DeviceView& view, DeviceController& controller,
                                     DeviceInputHandler& inputHandler)
    : protocol_(protocol), api_(api), view_(view), view_controller_(controller), input_handler_(inputHandler) {
    setupProtocolCallbacks();
}

template<typename MacroArray>
void DeviceHostHandler::updateMacroEncoderPositions(const MacroArray& macros) {
    for (uint8_t i = 0; i < Device::PARAMETER_COUNT && i < macros.size(); i++) {
        // Cache parameter types for echo handling
        uint8_t paramIndex = macros[i].parameterIndex;
        if (paramIndex < Device::PARAMETER_COUNT) {
            parameterTypes_[paramIndex] = macros[i].parameterType;
        }

        EncoderID encoderId = DeviceInputHandler::getEncoderIdForParameter(paramIndex);
        if (encoderId != EncoderID(0)) {
            // Configure encoder quantization based on parameter type
            if (macros[i].parameterType == Device::Knob) {
                // Continuous parameter - emit all value changes
                api_.setEncoderContinuous(encoderId);
            } else {
                // Discrete parameter (Button or List) - quantize to discrete steps
                api_.setEncoderDiscreteSteps(encoderId, macros[i].discreteValueCount);
            }

            api_.setEncoderPosition(encoderId, macros[i].parameterValue);
        }
    }
}

DeviceHostHandler::DeviceSelectorData DeviceHostHandler::transformDeviceListMessage(
    const Protocol::DeviceListMessage& msg) {
    DeviceSelectorData data;

    if (msg.isNested) {
        data.names.push_back(Device::BACK_TO_PARENT_TEXT);
        data.states.push_back(false);
        data.hasSlots.push_back(false);
        data.hasLayers.push_back(false);
        data.hasDrums.push_back(false);
    }

    for (uint8_t i = 0; i < msg.deviceCount; i++) {
        data.names.push_back(std::string(msg.devices[i].deviceName.data()));
        data.states.push_back(msg.devices[i].isEnabled);

        uint8_t flags = Device::getChildTypeFlags(msg.devices[i].childrenTypes);
        data.hasSlots.push_back(flags & Device::Slots);
        data.hasLayers.push_back(flags & Device::Layers);
        data.hasDrums.push_back(flags & Device::Drums);
    }

    return data;
}

void DeviceHostHandler::setupProtocolCallbacks() {
    // Track context change handler (granular track updates)
    protocol_.onTrackChange = [this](const Protocol::TrackChangeMessage& msg) {
        api_.log(msg.toString());
        view_controller_.handleTrackChange(msg);
    };

    // Lightweight device header handler (optimized for speed)
    protocol_.onDeviceChangeHeader = [this](const Protocol::DeviceChangeHeaderMessage& msg) {
        api_.log(msg.toString());
        view_controller_.handleDeviceChangeHeader(msg);
    };

    // Individual macro update handler
    protocol_.onDeviceMacroUpdate = [this](const Protocol::DeviceMacroUpdateMessage& msg) {
        // api_.log(msg.toString());

        // Cache parameter type for echo handling
        if (msg.parameterIndex < Device::PARAMETER_COUNT) {
            parameterTypes_[msg.parameterIndex] = msg.parameterType;
        }

        // Configure encoder based on parameter type
        EncoderID encoderId = DeviceInputHandler::getEncoderIdForParameter(msg.parameterIndex);
        if (encoderId != EncoderID(0)) {
            if (msg.parameterType == Device::Knob) {
                api_.setEncoderContinuous(encoderId);
            } else {
                api_.setEncoderDiscreteSteps(encoderId, msg.discreteValueCount);
            }
            api_.setEncoderPosition(encoderId, msg.parameterValue);
        }

        // Update UI widget
        view_controller_.handleMacroUpdate(msg);
    };

    // Discrete values handler (for List parameters)
    protocol_.onDeviceMacroDiscreteValues =
        [this](const Protocol::DeviceMacroDiscreteValuesMessage& msg) {
            view_controller_.handleMacroDiscreteValues(msg);
        };

    protocol_.onDeviceMacroValueChange =
        [this](const Protocol::DeviceMacroValueChangeMessage& msg) {
            if (!msg.fromHost) {
                return;
            }

            uint8_t paramType = (msg.parameterIndex < Device::PARAMETER_COUNT)
                                    ? parameterTypes_[msg.parameterIndex]
                                    : Device::Knob;

            if (msg.isEcho) {
                // Echo of controller-initiated change
                // - Knob: skip (already applied optimistically)
                // - Button/List: apply (no optimistic update was done, need displayValue)
                if (paramType != Device::Knob) {
                    view_controller_.handleParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
                }
            } else {
                // Host-initiated change: apply to encoder and UI
                EncoderID encoderId = DeviceInputHandler::getEncoderIdForParameter(msg.parameterIndex);
                if (encoderId != EncoderID(0)) {
                    api_.setEncoderPosition(encoderId, msg.parameterValue);
                }

                view_controller_.handleParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue, msg.displayValue.c_str());
            }
        };

    protocol_.onDevicePageChange = [this](const Protocol::DevicePageChangeMessage& msg) {
        updateMacroEncoderPositions(msg.macros);
        view_controller_.handlePageChange(msg);
    };

    protocol_.onDeviceMacroNameChange = [this](const Protocol::DeviceMacroNameChangeMessage& msg) {
        view_controller_.handleParameterName(msg.parameterIndex, msg.parameterName.c_str());
    };

    protocol_.onDeviceStateChange = [this](const Protocol::DeviceStateChangeMessage& msg) {
        // Update main device view state indicator
        view_controller_.handleDeviceState(msg.isEnabled);

        // Update device list overlay if visible
        view_controller_.handleDeviceStateAtIndex(msg.deviceIndex, msg.isEnabled);
    };

    protocol_.onDevicePageNames = [this](const Protocol::DevicePageNamesMessage& msg) {
        if (!msg.fromHost) return;

        input_handler_.setPageSelectionState(msg.devicePageCount, msg.devicePageIndex);

        std::vector<std::string> pageNames;
        pageNames.reserve(msg.devicePageCount);
        for (uint8_t i = 0; i < msg.devicePageCount; i++) {
            pageNames.push_back(std::string(msg.pageNames[i].data()));
        }

        view_controller_.handleShowPageSelector(pageNames, msg.devicePageIndex);
    };

    protocol_.onDeviceList = [this](const Protocol::DeviceListMessage& msg) {
        if (!msg.fromHost) return;

        // Update input handler state
        etl::array<etl::array<uint8_t, Device::MAX_CHILD_TYPES>, Device::MAX_DEVICES> allChildrenTypes;
        for (uint8_t i = 0; i < msg.deviceCount && i < Device::MAX_DEVICES; i++) {
            allChildrenTypes[i] = msg.devices[i].childrenTypes;
        }

        input_handler_.setDeviceListState(msg.deviceCount,
                                          msg.deviceIndex,
                                          msg.isNested,
                                          allChildrenTypes.data(),
                                          msg.deviceCount);

        // Only display if explicitly requested by user
        if (!input_handler_.isDeviceListRequested()) {
            return;
        }

        // Transform and display
        DeviceSelectorData data = transformDeviceListMessage(msg);
        int displayIndex = msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex;

        view_controller_.handleShowDeviceSelectorWithIndicators(data.names,
                                                                displayIndex,
                                                                data.states,
                                                                data.hasSlots,
                                                                data.hasLayers,
                                                                data.hasDrums);
    };

    protocol_.onDeviceChildren = [this](const Protocol::DeviceChildrenMessage& msg) {
        if (!msg.fromHost) return;

        input_handler_.setDeviceChildrenState(msg.deviceIndex, msg.childType, msg.childrenCount);

        std::vector<std::string> formattedItems;
        formattedItems.push_back(Device::BACK_TO_PARENT_TEXT);

        for (uint8_t i = 0; i < msg.childrenCount; i++) {
            std::string formatted = std::string(msg.children[i].childName.data());
            formattedItems.push_back(formatted);
        }

        view_controller_.handleShowDeviceChildren(formattedItems);
    };

    // Track list handler
    protocol_.onTrackList = [this](const Protocol::TrackListMessage& msg) {
        if (!msg.fromHost) return;

        LOGF("[DeviceHostHandler] Received TRACK_LIST: count=%u, index=%u, nested=%u",
            msg.trackCount, msg.trackIndex, msg.isNested);

        // Log track details
        for (uint8_t i = 0; i < msg.trackCount; i++) {
            std::string name = std::string(msg.tracks[i].trackName.data());
            LOGF("[DeviceHostHandler] Track %u: %s (activated=%u, mute=%u, solo=%u, group=%u)",
                i, name.c_str(), msg.tracks[i].isActivated, msg.tracks[i].isMute,
                msg.tracks[i].isSolo, msg.tracks[i].isGroup);
        }

        // Update input handler state
        input_handler_.setTrackListState(msg.trackCount, msg.trackIndex, msg.isNested);

        // Delegate to controller for display
        view_controller_.handleTrackList(msg);
    };

    // Track mute state handler
    protocol_.onTrackMute = [this](const Protocol::TrackMuteMessage& msg) {
        if (!msg.fromHost) return;

        LOGF("[DeviceHostHandler] Received TRACK_MUTE: track=%u, isMute=%u",
            msg.trackIndex, msg.isMute);

        // Update view
        view_controller_.handleTrackMuteState(msg.trackIndex, msg.isMute);
    };

    // Track solo state handler
    protocol_.onTrackSolo = [this](const Protocol::TrackSoloMessage& msg) {
        if (!msg.fromHost) return;

        LOGF("[DeviceHostHandler] Received TRACK_SOLO: track=%u, isSolo=%u",
            msg.trackIndex, msg.isSolo);

        // Update view
        view_controller_.handleTrackSoloState(msg.trackIndex, msg.isSolo);
    };
}

} // namespace Plugin::Bitwig
