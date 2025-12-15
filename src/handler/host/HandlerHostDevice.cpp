#include "HandlerHostDevice.hpp"

#include <array>
#include <vector>

#include <oc/log/Log.hpp>

#include "handler/InputUtils.hpp"
#include "handler/NestedIndexUtils.hpp"
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
#include "protocol/struct/TrackMuteMessage.hpp"
#include "protocol/struct/TrackSoloMessage.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;
constexpr auto BACK_TO_PARENT = BACK_TO_PARENT_TEXT;

HandlerHostDevice::HandlerHostDevice(state::BitwigState& state,
                                     BitwigProtocol& protocol,
                                     oc::api::EncoderAPI& encoders)
    : state_(state), protocol_(protocol), encoders_(encoders) {
    setupProtocolCallbacks();
}

template <typename MacroArray>
void HandlerHostDevice::updateMacroEncoderModes(const MacroArray& macros) {
    for (uint8_t i = 0; i < PARAMETER_COUNT && i < macros.size(); i++) {
        uint8_t paramIndex = macros[i].parameterIndex;
        if (paramIndex >= PARAMETER_COUNT) continue;

        // Update parameter type in state
        state_.parameters.slots[paramIndex].type.set(
            static_cast<state::ParameterType>(macros[i].parameterType));

        // Configure encoder mode
        auto encoderId = getEncoderIdForParameter(paramIndex);
        if (encoderId != EncoderID{0}) {
            configureEncoderForParameter(encoders_, encoderId,
                                         macros[i].parameterType,
                                         macros[i].discreteValueCount,
                                         macros[i].parameterValue);
        }
    }
}

void HandlerHostDevice::setupProtocolCallbacks() {
    // =========================================================================
    // Track
    // =========================================================================

    protocol_.onTrackChange = [this](const TrackChangeMessage& msg) {
        state_.currentTrack.name.set(msg.trackName.c_str());
        state_.currentTrack.color.set(msg.color);
        state_.currentTrack.trackType.set(static_cast<state::TrackType>(msg.trackType));
    };

    protocol_.onTrackList = [this](const TrackListMessage& msg) {
        if (!msg.fromHost) return;

        // Update navigation state
        state_.trackSelector.isNested.set(msg.isNested);
        state_.trackSelector.activeTrackIndex.set(msg.trackIndex);

        // Build temporary lists for bulk data
        std::vector<std::string> names;
        std::vector<uint8_t> trackTypes;
        std::vector<uint32_t> trackColors;

        uint8_t displayIndex = 0;
        if (msg.isNested) {
            names.push_back(BACK_TO_PARENT);
            trackTypes.push_back(0);
            trackColors.push_back(0xFFFFFF);
            state_.trackSelector.muteStates[displayIndex].set(false);
            state_.trackSelector.soloStates[displayIndex].set(false);
            displayIndex++;
        }

        for (uint8_t i = 0; i < msg.trackCount && displayIndex < MAX_TRACKS; i++) {
            names.push_back(std::string(msg.tracks[i].trackName.data()));
            trackTypes.push_back(msg.tracks[i].trackType);
            trackColors.push_back(msg.tracks[i].color);
            state_.trackSelector.muteStates[displayIndex].set(msg.tracks[i].isMute);
            state_.trackSelector.soloStates[displayIndex].set(msg.tracks[i].isSolo);
            displayIndex++;
        }

        // Update SignalVectors (bulk data)
        state_.trackSelector.names.set(names.data(), names.size());
        state_.trackSelector.trackTypes.set(trackTypes.data(), trackTypes.size());
        state_.trackSelector.trackColors.set(trackColors.data(), trackColors.size());

        state_.trackSelector.currentIndex.set(msg.isNested ? msg.trackIndex + 1 : msg.trackIndex);

        // Show track selector, hide device selector to prevent scope conflicts
        state_.deviceSelector.visible.set(false);
        state_.trackSelector.visible.set(true);
    };

    protocol_.onTrackMute = [this](const TrackMuteMessage& msg) {
        if (!msg.fromHost) return;

        int displayIndex = utils::toDisplayIndex(msg.trackIndex, state_.trackSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_TRACKS) {
            state_.trackSelector.muteStates[displayIndex].set(msg.isMute);
        }
    };

    protocol_.onTrackSolo = [this](const TrackSoloMessage& msg) {
        if (!msg.fromHost) return;

        int displayIndex = utils::toDisplayIndex(msg.trackIndex, state_.trackSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_TRACKS) {
            state_.trackSelector.soloStates[displayIndex].set(msg.isSolo);
        }
    };

    // =========================================================================
    // Device
    // =========================================================================

    protocol_.onDeviceChangeHeader = [this](const DeviceChangeHeaderMessage& msg) {
        bool hasChildren = (msg.childrenTypes[0] | msg.childrenTypes[1] |
                           msg.childrenTypes[2] | msg.childrenTypes[3]) != 0;

        state_.device.name.set(msg.deviceName.c_str());
        state_.device.deviceType.set(static_cast<state::DeviceType>(msg.deviceType));
        state_.device.enabled.set(msg.isEnabled);
        state_.device.pageName.set(msg.pageInfo.devicePageName.c_str());
        state_.device.hasChildren.set(hasChildren);

        // Mark all parameters as loading
        for (uint8_t i = 0; i < PARAMETER_COUNT; i++) {
            state_.parameters.slots[i].loading.set(true);
        }
    };

    protocol_.onDeviceStateChange = [this](const DeviceStateChangeMessage& msg) {
        int activeIndex = state_.deviceSelector.activeDeviceIndex.get();
        if (static_cast<int>(msg.deviceIndex) == activeIndex) {
            state_.device.enabled.set(msg.isEnabled);
        }

        int displayIndex = utils::toDisplayIndex(msg.deviceIndex, state_.deviceSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_DEVICES) {
            state_.deviceSelector.deviceStates[displayIndex].set(msg.isEnabled);
        }
    };

    protocol_.onDeviceList = [this](const DeviceListMessage& msg) {
        if (!msg.fromHost) return;

        // Update active device info
        if (msg.deviceIndex < msg.deviceCount) {
            uint8_t flags = getChildTypeFlags(msg.devices[msg.deviceIndex].childrenTypes);
            bool hasChildren = (flags & (static_cast<uint8_t>(ChildType::SLOTS) |
                                         static_cast<uint8_t>(ChildType::LAYERS) |
                                         static_cast<uint8_t>(ChildType::DRUMS))) != 0;
            state_.device.hasChildren.set(hasChildren);
            state_.deviceSelector.activeDeviceIndex.set(msg.deviceIndex);
        }

        // Update navigation state
        state_.deviceSelector.isNested.set(msg.isNested);

        // Build temporary lists for bulk data
        std::vector<std::string> names;
        std::vector<uint8_t> deviceTypes;
        std::array<bool, MAX_DEVICES> hasSlots{};
        std::array<bool, MAX_DEVICES> hasLayers{};
        std::array<bool, MAX_DEVICES> hasDrums{};

        uint8_t displayIndex = 0;
        if (msg.isNested) {
            names.push_back(BACK_TO_PARENT);
            deviceTypes.push_back(0);
            hasSlots[displayIndex] = false;
            hasLayers[displayIndex] = false;
            hasDrums[displayIndex] = false;
            state_.deviceSelector.deviceStates[displayIndex].set(false);
            displayIndex++;
        }

        for (uint8_t i = 0; i < msg.deviceCount && displayIndex < MAX_DEVICES; i++) {
            OC_LOG_DEBUG("[DeviceList] {} -> deviceType={}",
                        msg.devices[i].deviceName.c_str(), msg.devices[i].deviceType);
            names.push_back(std::string(msg.devices[i].deviceName.data()));
            deviceTypes.push_back(msg.devices[i].deviceType);
            state_.deviceSelector.deviceStates[displayIndex].set(msg.devices[i].isEnabled);

            uint8_t flags = getChildTypeFlags(msg.devices[i].childrenTypes);
            hasSlots[displayIndex] = flags & static_cast<uint8_t>(ChildType::SLOTS);
            hasLayers[displayIndex] = flags & static_cast<uint8_t>(ChildType::LAYERS);
            hasDrums[displayIndex] = flags & static_cast<uint8_t>(ChildType::DRUMS);
            displayIndex++;
        }

        // Update SignalVectors (bulk data)
        state_.deviceSelector.names.set(names.data(), names.size());
        state_.deviceSelector.deviceTypes.set(deviceTypes.data(), deviceTypes.size());
        state_.deviceSelector.hasSlots.set(hasSlots.data(), displayIndex);
        state_.deviceSelector.hasLayers.set(hasLayers.data(), displayIndex);
        state_.deviceSelector.hasDrums.set(hasDrums.data(), displayIndex);

        state_.deviceSelector.currentIndex.set(msg.isNested ? msg.deviceIndex + 1 : msg.deviceIndex);
        state_.deviceSelector.showingChildren.set(false);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };

    protocol_.onDeviceChildren = [this](const DeviceChildrenMessage& msg) {
        if (!msg.fromHost) return;

        std::vector<std::string> names;
        std::vector<uint8_t> types;

        names.push_back(BACK_TO_PARENT_TEXT);
        types.push_back(0);

        for (uint8_t i = 0; i < msg.childrenCount; i++) {
            names.push_back(std::string(msg.children[i].childName.data()));
            types.push_back(msg.children[i].itemType);
        }

        state_.deviceSelector.childrenNames.set(names.data(), names.size());
        state_.deviceSelector.childrenTypes.set(types.data(), types.size());
        state_.deviceSelector.showingChildren.set(true);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };

    // =========================================================================
    // Page
    // =========================================================================

    protocol_.onDevicePageNames = [this](const DevicePageNamesMessage& msg) {
        if (!msg.fromHost) return;

        std::vector<std::string> names;
        for (uint8_t i = 0; i < msg.devicePageCount; i++) {
            names.push_back(std::string(msg.pageNames[i].data()));
        }

        state_.pageSelector.names.set(names.data(), names.size());
        state_.pageSelector.selectedIndex.set(msg.devicePageIndex);
    };

    protocol_.onDevicePageChange = [this](const DevicePageChangeMessage& msg) {
        updateMacroEncoderModes(msg.macros);

        state_.device.pageName.set(msg.pageInfo.devicePageName.c_str());

        // Local buffer for discrete values (stack allocated, safe in single-threaded context)
        std::array<std::string, state::MAX_DISCRETE_VALUES> tempDiscreteValues;

        for (uint8_t i = 0; i < PARAMETER_COUNT && i < msg.macros.size(); i++) {
            auto& slot = state_.parameters.slots[i];
            const auto& macro = msg.macros[i];

            // IMPORTANT: Set metadata BEFORE type!
            // When type changes, ensureWidgetForType() reads these values to create the widget.
            // If type is set first, discreteCount would still have old/default value.
            slot.discreteCount.set(macro.discreteValueCount);
            slot.currentValueIndex.set(macro.currentValueIndex);
            slot.origin.set(macro.parameterOrigin);

            // Update discrete values BEFORE type (needed for LIST widgets)
            size_t count = 0;
            for (const auto& dv : macro.discreteValueNames) {
                if (!dv.empty() && count < state::MAX_DISCRETE_VALUES) {
                    tempDiscreteValues[count++] = dv;
                }
            }
            slot.discreteValues.set(tempDiscreteValues.data(), count);

            // NOW set type - this triggers widget creation with correct metadata
            slot.type.set(static_cast<state::ParameterType>(macro.parameterType));

            // Set remaining display properties
            slot.displayValue.set(macro.displayValue.c_str());
            slot.value.set(macro.parameterValue);
            slot.name.set(macro.parameterName.c_str());
            slot.visible.set(macro.parameterExists);
            slot.loading.set(false);
            slot.metadataSet.set(true);
        }
    };

    // =========================================================================
    // Macro Parameters
    // =========================================================================

    protocol_.onDeviceMacroUpdate = [this](const DeviceMacroUpdateMessage& msg) {
        if (msg.parameterIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.parameterIndex];

        slot.type.set(static_cast<state::ParameterType>(msg.parameterType));
        slot.discreteCount.set(msg.discreteValueCount);
        slot.currentValueIndex.set(msg.currentValueIndex);
        slot.origin.set(msg.parameterOrigin);
        slot.displayValue.set(msg.displayValue.c_str());
        slot.name.set(msg.parameterName.c_str());
        slot.value.set(msg.parameterValue);
        slot.visible.set(msg.parameterExists);
        slot.loading.set(false);

        // Configure encoder
        auto encoderId = getEncoderIdForParameter(msg.parameterIndex);
        if (encoderId != EncoderID{0}) {
            configureEncoderForParameter(encoders_, encoderId,
                                         msg.parameterType,
                                         msg.discreteValueCount,
                                         msg.parameterValue);
        }
    };

    protocol_.onDeviceMacroDiscreteValues = [this](const DeviceMacroDiscreteValuesMessage& msg) {
        if (msg.parameterIndex >= PARAMETER_COUNT) return;

        auto& slot = state_.parameters.slots[msg.parameterIndex];

        // Local buffer for discrete values (stack allocated, safe in single-threaded context)
        std::array<std::string, state::MAX_DISCRETE_VALUES> tempValues;
        size_t count = 0;
        for (const auto& dv : msg.discreteValueNames) {
            if (!dv.empty() && count < state::MAX_DISCRETE_VALUES) { tempValues[count++] = dv; }
        }
        slot.discreteValues.set(tempValues.data(), count);
        slot.currentValueIndex.set(msg.currentValueIndex);
    };

    protocol_.onDeviceMacroValueChange = [this](const DeviceMacroValueChangeMessage& msg) {
        OC_LOG_DEBUG("[HostDevice] onMacroValueChange: idx={} val={:.3f} echo={} fromHost={}",
                     msg.parameterIndex, msg.parameterValue, msg.isEcho, msg.fromHost);

        if (!msg.fromHost) {
            OC_LOG_DEBUG("[HostDevice] onMacroValueChange: skipped (not from host)");
            return;
        }
        if (msg.parameterIndex >= PARAMETER_COUNT) {
            OC_LOG_DEBUG("[HostDevice] onMacroValueChange: skipped (index {} >= {})",
                         msg.parameterIndex, PARAMETER_COUNT);
            return;
        }

        auto& slot = state_.parameters.slots[msg.parameterIndex];
        auto currentType = slot.type.get();

        if (msg.isEcho) {
            // Echo: only update display value for non-knob parameters
            if (currentType != state::ParameterType::KNOB) {
                OC_LOG_DEBUG("[HostDevice] onMacroValueChange: echo update (non-knob type={})",
                             static_cast<int>(currentType));
                slot.value.set(msg.parameterValue);
                slot.displayValue.set(msg.displayValue.c_str());
            } else {
                OC_LOG_DEBUG("[HostDevice] onMacroValueChange: echo skipped (knob)");
            }
        } else {
            // External change: update value and encoder position
            OC_LOG_DEBUG("[HostDevice] onMacroValueChange: external update -> state");
            slot.value.set(msg.parameterValue);
            slot.displayValue.set(msg.displayValue.c_str());

            auto encoderId = getEncoderIdForParameter(msg.parameterIndex);
            if (encoderId != EncoderID{0}) {
                encoders_.setPosition(encoderId, msg.parameterValue);
            }
        }
    };

    protocol_.onDeviceMacroNameChange = [this](const DeviceMacroNameChangeMessage& msg) {
        OC_LOG_DEBUG("[HostDevice] onMacroNameChange: idx={} name='{}'",
                     msg.parameterIndex, msg.parameterName.c_str());

        if (msg.parameterIndex >= PARAMETER_COUNT) return;
        state_.parameters.slots[msg.parameterIndex].name.set(msg.parameterName.c_str());
    };
}

}  // namespace bitwig::handler
