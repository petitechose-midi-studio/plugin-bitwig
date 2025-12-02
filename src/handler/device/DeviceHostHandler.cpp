#include "DeviceHostHandler.hpp"

#include "DeviceConstants.hpp"
#include "DeviceInputHandler.hpp"
#include "MacroInputHandler.hpp"
#include "TrackConstants.hpp"

#include <array>
#include <string>
#include <vector>

#include "protocol/struct/DeviceChangeHeaderMessage.hpp"
#include "protocol/struct/DeviceChildrenMessage.hpp"
#include "protocol/struct/DeviceListMessage.hpp"
#include "protocol/struct/DeviceMacroDiscreteValuesMessage.hpp"
#include "protocol/struct/DeviceMacroNameChangeMessage.hpp"
#include "protocol/struct/DeviceMacroUpdateMessage.hpp"
#include "protocol/struct/DeviceMacroValueChangeMessage.hpp"
#include "protocol/struct/DevicePageChangeMessage.hpp"
#include "protocol/struct/DevicePageNamesMessage.hpp"
#include "protocol/struct/DeviceStateChangeMessage.hpp"
#include "protocol/struct/TrackChangeMessage.hpp"
#include "protocol/struct/TrackListMessage.hpp"
#include "ui/device/DeviceView.hpp"

namespace Bitwig {

DeviceHostHandler::DeviceHostHandler(Protocol::Protocol &protocol, ControllerAPI &api,
                                     DeviceView &view, DeviceInputHandler &inputHandler)
    : protocol_(protocol), api_(api), view_(view), input_handler_(inputHandler) {
    setupProtocolCallbacks();
}

int DeviceHostHandler::toDeviceDisplayIndex(int rawIndex) const {
    return view_.state().deviceSelector.isNested ? rawIndex + 1 : rawIndex;
}

int DeviceHostHandler::toTrackDisplayIndex(int rawIndex) const {
    return view_.state().trackSelector.isNested ? rawIndex + 1 : rawIndex;
}

template <typename MacroArray>
void DeviceHostHandler::updateMacroEncoderPositions(const MacroArray &macros) {
    for (uint8_t i = 0; i < Device::PARAMETER_COUNT && i < macros.size(); i++) {
        uint8_t param_index = macros[i].parameterIndex;
        if (param_index < Device::PARAMETER_COUNT) {
            parameter_types_[param_index] = macros[i].parameterType;
        }

        EncoderID encoder_id = MacroInputHandler::getEncoderIdForParameter(param_index);
        if (encoder_id != EncoderID(0)) {
            if (macros[i].parameterType == static_cast<uint8_t>(Device::ParameterType::KNOB)) {
                api_.setEncoderContinuous(encoder_id);
            } else {
                api_.setEncoderDiscreteSteps(encoder_id, macros[i].discreteValueCount);
            }
            api_.setEncoderPosition(encoder_id, macros[i].parameterValue);
        }
    }
}

void DeviceHostHandler::setupProtocolCallbacks() {
    // =============================================================================
    // Track
    // =============================================================================

    protocol_.onTrackChange = [this](const Protocol::TrackChangeMessage &msg) {
        auto &track = view_.state().currentTrack;
        track.name = msg.trackName.c_str();
        track.color = msg.color;
        track.trackType = msg.trackType;
        view_.state().dirty.deviceSelector = true;
        view_.sync();
    };

    protocol_.onTrackList = [this](const Protocol::TrackListMessage &msg) {
        if (!msg.fromHost) return;

        input_handler_.setTrackListState(msg.trackCount, msg.trackIndex, msg.isNested);

        if (!input_handler_.isTrackListRequested()) return;

        std::vector<std::string> names;
        std::vector<bool> mute_states;
        std::vector<bool> solo_states;
        std::vector<uint8_t> track_types;
        std::vector<uint32_t> track_colors;

        if (msg.isNested) {
            names.push_back(Track::BACK_TO_PARENT_TEXT);
            mute_states.push_back(false);
            solo_states.push_back(false);
            track_types.push_back(0);
            track_colors.push_back(0xFFFFFF);
        }

        for (uint8_t i = 0; i < msg.trackCount; i++) {
            names.push_back(std::string(msg.tracks[i].trackName.data()));
            mute_states.push_back(msg.tracks[i].isMute);
            solo_states.push_back(msg.tracks[i].isSolo);
            track_types.push_back(msg.tracks[i].trackType);
            track_colors.push_back(msg.tracks[i].color);
        }

        auto &ts = view_.state().trackSelector;
        ts.names = names;
        ts.muteStates = mute_states;
        ts.soloStates = solo_states;
        ts.trackTypes = track_types;
        ts.trackColors = track_colors;
        ts.currentIndex = msg.isNested ? msg.trackIndex + 1 : msg.trackIndex;
        ts.activeTrackIndex = msg.trackIndex;
        ts.isNested = msg.isNested;
        ts.visible = true;

        view_.state().deviceSelector.visible = false;
        view_.state().dirty.deviceSelector = true;
        view_.state().dirty.trackSelector = true;
        view_.sync();
    };

    protocol_.onTrackMute = [this](const Protocol::TrackMuteMessage &msg) {
        if (!msg.fromHost) return;

        int display_index = toTrackDisplayIndex(msg.trackIndex);
        auto &mute_states = view_.state().trackSelector.muteStates;
        if (display_index >= 0 && display_index < static_cast<int>(mute_states.size())) {
            mute_states[display_index] = msg.isMute;
            view_.state().dirty.trackSelector = true;
            view_.sync();
        }
    };

    protocol_.onTrackSolo = [this](const Protocol::TrackSoloMessage &msg) {
        if (!msg.fromHost) return;

        int display_index = toTrackDisplayIndex(msg.trackIndex);
        auto &solo_states = view_.state().trackSelector.soloStates;
        if (display_index >= 0 && display_index < static_cast<int>(solo_states.size())) {
            solo_states[display_index] = msg.isSolo;
            view_.state().dirty.trackSelector = true;
            view_.sync();
        }
    };

    // =============================================================================
    // Device
    // =============================================================================

    protocol_.onDeviceChangeHeader = [this](const Protocol::DeviceChangeHeaderMessage &msg) {
        bool has_children = (msg.childrenTypes[0] | msg.childrenTypes[1] | msg.childrenTypes[2] |
                             msg.childrenTypes[3]) != 0;

        auto &device = view_.state().device;
        device.name = msg.deviceName.c_str();
        device.deviceType = msg.deviceType;
        device.enabled = msg.isEnabled;
        device.pageName = msg.pageInfo.devicePageName.c_str();
        device.hasChildren = has_children;
        view_.state().dirty.device = true;

        view_.setAllWidgetsLoading(true);
    };

    protocol_.onDeviceStateChange = [this](const Protocol::DeviceStateChangeMessage &msg) {
        int active_index = view_.state().deviceSelector.activeDeviceIndex;
        if (static_cast<int>(msg.deviceIndex) == active_index) {
            view_.state().device.enabled = msg.isEnabled;
            view_.state().dirty.device = true;
            view_.sync();
        }

        int display_index = toDeviceDisplayIndex(msg.deviceIndex);
        view_.updateDeviceState(display_index, msg.isEnabled);
    };

    protocol_.onDeviceList = [this](const Protocol::DeviceListMessage &msg) {
        if (!msg.fromHost) return;

        std::array<std::array<uint8_t, Device::MAX_CHILD_TYPES>, Device::MAX_DEVICES>
            all_children_types;
        for (uint8_t i = 0; i < msg.deviceCount && i < Device::MAX_DEVICES; i++) {
            all_children_types[i] = msg.devices[i].childrenTypes;
        }

        input_handler_.setDeviceListState(msg.deviceCount, msg.deviceIndex, msg.isNested,
                                          all_children_types.data(), msg.deviceCount);

        if (msg.deviceIndex < msg.deviceCount) {
            uint8_t flags = Device::getChildTypeFlags(msg.devices[msg.deviceIndex].childrenTypes);
            bool has_children = (flags & (static_cast<uint8_t>(Device::ChildType::SLOTS) |
                                          static_cast<uint8_t>(Device::ChildType::LAYERS) |
                                          static_cast<uint8_t>(Device::ChildType::DRUMS))) != 0;
            view_.state().device.hasChildren = has_children;
            view_.state().deviceSelector.activeDeviceIndex = msg.deviceIndex;
            view_.state().dirty.device = true;
        }

        if (!input_handler_.isDeviceListRequested()) {
            view_.sync();
            return;
        }

        std::vector<std::string> names;
        std::vector<uint8_t> types;
        std::vector<bool> states, has_slots, has_layers, has_drums;

        if (msg.isNested) {
            names.push_back(Device::BACK_TO_PARENT_TEXT);
            types.push_back(0);
            states.push_back(false);
            has_slots.push_back(false);
            has_layers.push_back(false);
            has_drums.push_back(false);
        }

        for (uint8_t i = 0; i < msg.deviceCount; i++) {
            names.push_back(std::string(msg.devices[i].deviceName.data()));
            types.push_back(msg.devices[i].deviceType);
            states.push_back(msg.devices[i].isEnabled);

            uint8_t flags = Device::getChildTypeFlags(msg.devices[i].childrenTypes);
            has_slots.push_back(flags & static_cast<uint8_t>(Device::ChildType::SLOTS));
            has_layers.push_back(flags & static_cast<uint8_t>(Device::ChildType::LAYERS));
            has_drums.push_back(flags & static_cast<uint8_t>(Device::ChildType::DRUMS));
        }

        auto &ds = view_.state().deviceSelector;
        ds.names = names;
        ds.deviceTypes = types;
        ds.deviceStates = states;
        ds.hasSlots = has_slots;
        ds.hasLayers = has_layers;
        ds.hasDrums = has_drums;
        ds.currentIndex = msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex;
        ds.isNested = msg.isNested;
        ds.showingChildren = false;
        ds.showFooter = true;
        ds.visible = true;

        view_.state().dirty.deviceSelector = true;
        view_.sync();
    };

    protocol_.onDeviceChildren = [this](const Protocol::DeviceChildrenMessage &msg) {
        if (!msg.fromHost) return;

        std::vector<std::string> formatted_items;
        std::vector<uint8_t> item_types;
        std::vector<uint8_t> child_indices;

        formatted_items.push_back(Device::BACK_TO_PARENT_TEXT);
        item_types.push_back(0);

        for (uint8_t i = 0; i < msg.childrenCount; i++) {
            formatted_items.push_back(std::string(msg.children[i].childName.data()));
            item_types.push_back(msg.children[i].itemType);
            child_indices.push_back(msg.children[i].childIndex);
        }

        std::vector<uint8_t> child_item_types(item_types.begin() + 1, item_types.end());
        input_handler_.setDeviceChildrenState(msg.deviceIndex, 0, msg.childrenCount,
                                              child_item_types, child_indices);

        auto &ds = view_.state().deviceSelector;
        ds.childrenNames = formatted_items;
        ds.childrenTypes = item_types;
        ds.showingChildren = true;
        ds.showFooter = false;
        ds.visible = true;
        view_.state().dirty.deviceSelector = true;
        view_.sync();
    };

    // =============================================================================
    // Page
    // =============================================================================

    protocol_.onDevicePageNames = [this](const Protocol::DevicePageNamesMessage &msg) {
        if (!msg.fromHost) return;

        input_handler_.setPageSelectionState(msg.devicePageCount, msg.devicePageIndex);

        std::vector<std::string> page_names;
        page_names.reserve(msg.devicePageCount);
        for (uint8_t i = 0; i < msg.devicePageCount; i++) {
            page_names.push_back(std::string(msg.pageNames[i].data()));
        }

        auto &ps = view_.state().pageSelector;
        ps.names = page_names;
        ps.selectedIndex = msg.devicePageIndex;
        ps.visible = true;
        view_.state().dirty.pageSelector = true;
        view_.sync();
    };

    protocol_.onDevicePageChange = [this](const Protocol::DevicePageChangeMessage &msg) {
        updateMacroEncoderPositions(msg.macros);

        view_.state().device.pageName = msg.pageInfo.devicePageName.c_str();
        view_.state().dirty.device = true;

        for (uint8_t i = 0; i < 8 && i < msg.macros.size(); i++) {
            view_.setParameterTypeAndMetadata(
                i, msg.macros[i].parameterType, msg.macros[i].discreteValueCount,
                msg.macros[i].discreteValueNames, msg.macros[i].currentValueIndex,
                msg.macros[i].parameterOrigin, msg.macros[i].displayValue.c_str());

            view_.setParameterValueWithDisplay(i, msg.macros[i].parameterValue,
                                               msg.macros[i].displayValue.c_str());
            view_.setParameterName(i, msg.macros[i].parameterName.c_str());
            view_.setParameterVisible(i, msg.macros[i].parameterExists);
        }
    };

    // =============================================================================
    // Macro Parameters
    // =============================================================================

    protocol_.onDeviceMacroUpdate = [this](const Protocol::DeviceMacroUpdateMessage &msg) {
        if (msg.parameterIndex < Device::PARAMETER_COUNT) {
            parameter_types_[msg.parameterIndex] = msg.parameterType;
        }

        EncoderID encoder_id = MacroInputHandler::getEncoderIdForParameter(msg.parameterIndex);
        if (encoder_id != EncoderID(0)) {
            if (msg.parameterType == static_cast<uint8_t>(Device::ParameterType::KNOB)) {
                api_.setEncoderContinuous(encoder_id);
            } else {
                api_.setEncoderDiscreteSteps(encoder_id, msg.discreteValueCount);
            }
            api_.setEncoderPosition(encoder_id, msg.parameterValue);
        }

        view_.setParameterTypeAndMetadata(msg.parameterIndex, msg.parameterType,
                                          msg.discreteValueCount, {}, msg.currentValueIndex,
                                          msg.parameterOrigin, msg.displayValue.c_str());

        view_.setParameterName(msg.parameterIndex, msg.parameterName.c_str());
        view_.setParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue,
                                           msg.displayValue.c_str());
        view_.setParameterVisible(msg.parameterIndex, msg.parameterExists);
        view_.setParameterLoading(msg.parameterIndex, false);
    };

    protocol_.onDeviceMacroDiscreteValues =
        [this](const Protocol::DeviceMacroDiscreteValuesMessage &msg) {
            view_.setParameterDiscreteValues(msg.parameterIndex, msg.discreteValueNames,
                                             msg.currentValueIndex);
        };

    protocol_.onDeviceMacroValueChange = [this](
                                             const Protocol::DeviceMacroValueChangeMessage &msg) {
        if (!msg.fromHost) return;

        uint8_t param_type = (msg.parameterIndex < Device::PARAMETER_COUNT)
                                 ? parameter_types_[msg.parameterIndex]
                                 : static_cast<uint8_t>(Device::ParameterType::KNOB);

        if (msg.isEcho) {
            if (param_type != static_cast<uint8_t>(Device::ParameterType::KNOB)) {
                view_.setParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue,
                                                   msg.displayValue.c_str());
            }
        } else {
            EncoderID encoder_id = MacroInputHandler::getEncoderIdForParameter(msg.parameterIndex);
            if (encoder_id != EncoderID(0)) {
                api_.setEncoderPosition(encoder_id, msg.parameterValue);
            }
            view_.setParameterValueWithDisplay(msg.parameterIndex, msg.parameterValue,
                                               msg.displayValue.c_str());
        }
    };

    protocol_.onDeviceMacroNameChange = [this](const Protocol::DeviceMacroNameChangeMessage &msg) {
        view_.setParameterName(msg.parameterIndex, msg.parameterName.c_str());
    };
}

}  // namespace Bitwig
