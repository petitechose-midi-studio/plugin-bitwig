#include "DeviceInputHandler.hpp"
#include "DeviceConstants.hpp"
#include "TrackInputHandler.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../ui/device/DeviceController.hpp"
#include "../../ui/device/component/DeviceSelector.hpp"
#include "../../protocol/struct/DeviceMacroValueChangeMessage.hpp"
#include "../../protocol/struct/DeviceMacroTouchMessage.hpp"
#include "../../protocol/struct/DeviceStateChangeMessage.hpp"
#include "../../protocol/struct/RequestDevicePageNamesMessage.hpp"
#include "../../protocol/struct/DevicePageSelectByIndexMessage.hpp"
#include "../../protocol/struct/DevicePageNamesMessage.hpp"
#include "../../protocol/struct/RequestDeviceListMessage.hpp"
#include "../../protocol/struct/DeviceSelectByIndexMessage.hpp"
#include "../../protocol/struct/RequestDeviceChildrenMessage.hpp"
#include "../../protocol/struct/EnterDeviceChildMessage.hpp"
#include "../../protocol/struct/ExitToParentMessage.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    DeviceInputHandler::DeviceInputHandler(ControllerAPI &api, DeviceView &view,
                                           DeviceController &controller, Protocol::Protocol &protocol,
                                           lv_obj_t *scope)
        : api_(api), view_(view), scope_(scope), view_controller_(controller), protocol_(protocol)
    {
        setupBindings();

        // Create TrackInputHandler (manages TrackListSelector interactions)
        trackInputHandler_ = std::make_unique<TrackInputHandler>(api, view, controller, protocol, scope);
    }

    DeviceInputHandler::~DeviceInputHandler()
    {
        api_.clearScope(scope_);
    }

    bool DeviceInputHandler::isTrackListRequested() const
    {
        return trackInputHandler_ ? trackInputHandler_->isTrackListRequested() : false;
    }

    EncoderID DeviceInputHandler::getEncoderIdForParameter(uint8_t paramIndex)
    {
        static constexpr EncoderID PARAM_TO_ENCODER[Device::PARAMETER_COUNT] = {
            EncoderID::MACRO_1, EncoderID::MACRO_2, EncoderID::MACRO_3, EncoderID::MACRO_4,
            EncoderID::MACRO_5, EncoderID::MACRO_6, EncoderID::MACRO_7, EncoderID::MACRO_8};

        return (paramIndex < Device::PARAMETER_COUNT) ? PARAM_TO_ENCODER[paramIndex] : EncoderID(0);
    }

    void DeviceInputHandler::setPageSelectionState(uint8_t pageCount, uint8_t currentIndex)
    {
        pageSelection_.count = pageCount;
        pageSelection_.cursor = currentIndex;

        // Set encoder to Relative mode for page navigation (delta-based with wrap)
        api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);

        // Initialize selector index
        pageSelection_.currentSelectorIndex = currentIndex;
        view_controller_.handlePageSelectorSetIndex(currentIndex);
    }

    void DeviceInputHandler::setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex,
                                                bool isNested,
                                                const etl::array<uint8_t, 4> *childrenTypes,
                                                uint8_t childrenTypesCount)
    {
        deviceList_.count = deviceCount;
        deviceList_.currentIndex = currentDeviceIndex;
        deviceList_.isNested = isNested;
        navigation_.mode = SelectorMode::DEVICES;

        for (uint8_t i = 0; i < childrenTypesCount && i < Device::MAX_DEVICES; i++)
        {
            deviceList_.childrenTypes[i] = childrenTypes[i];
        }

        // Set encoder to Relative mode for device navigation (delta-based with wrap)
        api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);

        // Initialize selector index (adjust for nested mode where "Back" is at index 0)
        navigation_.currentSelectorIndex = deviceList_.isNested ? currentDeviceIndex + 1 : currentDeviceIndex;
        view_controller_.handleDeviceSelectorSetIndex(navigation_.currentSelectorIndex);
    }

    void DeviceInputHandler::setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType,
                                                    uint8_t childrenCount,
                                                    const std::vector<uint8_t> &itemTypes)
    {
        navigation_.deviceIndex = deviceIndex;
        navigation_.childType = childType;
        navigation_.childrenCount = childrenCount;
        navigation_.mode = SelectorMode::CHILDREN;

        // Store itemTypes for each child
        Serial.printf("[C++ INPUT] setDeviceChildrenState: deviceIndex=%d, childrenCount=%d, itemTypes.size()=%d\n",
                      deviceIndex, childrenCount, itemTypes.size());

        for (size_t i = 0; i < itemTypes.size() && i < navigation_.itemTypes.size(); i++)
        {
            navigation_.itemTypes[i] = itemTypes[i];
            const char* typeStr = (itemTypes[i] == 0) ? "SLOT" : (itemTypes[i] == 1) ? "LAYER" : (itemTypes[i] == 2) ? "DRUM" : "UNKNOWN";
            Serial.printf("  [%d] itemType=%d (%s)\n", i, itemTypes[i], typeStr);
        }

        // Set encoder to Relative mode for children navigation (delta-based with wrap)
        api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);

        // Initialize selector index (start at "Back" item which is at index 0)
        navigation_.currentSelectorIndex = 0;
        view_controller_.handleDeviceSelectorSetIndex(0);
    }

    void DeviceInputHandler::setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex,
                                               bool isNested)
    {
        if (trackInputHandler_)
        {
            trackInputHandler_->setTrackListState(trackCount, currentTrackIndex, isNested);
        }
    }

    int DeviceInputHandler::wrapIndex(int value, int modulo)
    {
        return ((value % modulo) + modulo) % modulo;
    }

    void DeviceInputHandler::setupBindings()
    {
        setupMacroBindings();
        setupPageSelectorBindings();
        setupDeviceSelectorBindings();
    }

    void DeviceInputHandler::setupMacroBindings()
    {
        lv_obj_t *scope = scope_;

        // Macro encoder bindings (8 identical bindings reduced to loop)
        const EncoderID macroEncoders[Device::PARAMETER_COUNT] = {
            EncoderID::MACRO_1, EncoderID::MACRO_2, EncoderID::MACRO_3, EncoderID::MACRO_4,
            EncoderID::MACRO_5, EncoderID::MACRO_6, EncoderID::MACRO_7, EncoderID::MACRO_8};

        const ButtonID macroButtons[Device::PARAMETER_COUNT] = {
            ButtonID::MACRO_1, ButtonID::MACRO_2, ButtonID::MACRO_3, ButtonID::MACRO_4,
            ButtonID::MACRO_5, ButtonID::MACRO_6, ButtonID::MACRO_7, ButtonID::MACRO_8};

        for (uint8_t i = 0; i < Device::PARAMETER_COUNT; i++)
        {
            // Encoder turn
            api_.onTurned(macroEncoders[i], [this, i](float value)
                          { handleParameterChange(i, value); }, scope);

            // Button touch (press/release)
            api_.onPressed(macroButtons[i], [this, i]()
                           { protocol_.send(Protocol::DeviceMacroTouchMessage{i, true}); }, scope);
            api_.onReleased(macroButtons[i], [this, i]()
                            { protocol_.send(Protocol::DeviceMacroTouchMessage{i, false}); }, scope);
        }
    }

    void DeviceInputHandler::setupPageSelectorBindings()
    {
        lv_obj_t *scope = scope_;

        api_.onPressed(
            ButtonID::LEFT_BOTTOM,
            [this]()
            {
                if (!pageSelection_.requested)
                {
                    protocol_.send(Protocol::RequestDevicePageNamesMessage{});
                    pageSelection_.requested = true;
                }
            },
            scope);

        api_.onTurnedWhilePressed(
            EncoderID::NAV,
            ButtonID::LEFT_BOTTOM,
            [this](float delta)
            {
                if (pageSelection_.count == 0)
                    return;

                // In Relative mode, delta is ±1 (or ±N for fast scrolling)
                // Apply delta and wrap around list boundaries
                pageSelection_.currentSelectorIndex += static_cast<int>(delta);
                pageSelection_.currentSelectorIndex = wrapIndex(pageSelection_.currentSelectorIndex, pageSelection_.count);

                view_controller_.handlePageSelectorSetIndex(pageSelection_.currentSelectorIndex);
            },
            scope);

        api_.onReleased(ButtonID::LEFT_BOTTOM, [this]()
                        { handlePageSelectorRelease(); }, scope);
    }

    void DeviceInputHandler::setupDeviceSelectorBindings()
    {
        lv_obj_t *scope = scope_;

        api_.onPressed(ButtonID::LEFT_CENTER, [this]()
                       { handleDeviceSelectorRequest(); }, scope);

        api_.onTurnedWhilePressed(
            EncoderID::NAV,
            ButtonID::LEFT_CENTER,
            [this](float position)
            { handleDeviceSelectorNavigation(position); },
            scope);

        api_.onCombo(ButtonID::NAV, ButtonID::LEFT_CENTER, [this]()
                     { handleDeviceSelectorEnter(); }, scope);

        api_.onReleased(ButtonID::LEFT_CENTER, [this]()
                        { handleDeviceSelectorRelease(); }, scope);

        // Device state toggle: scoped to DeviceSelector overlay (only active when selector visible)
        api_.onPressed(
            ButtonID::BOTTOM_CENTER,
            [this]()
            {
                // Toggle the selected device in the list (not the current device)
                const int selectorIndex = view_controller_.getDeviceSelectorSelectedIndex();
                const int deviceIndex = getAdjustedDeviceIndex(selectorIndex);

                // Only toggle if it's a valid device (not "Back to parent")
                if (deviceIndex >= 0 && deviceIndex < deviceList_.count)
                {
                    protocol_.send(Protocol::DeviceStateChangeMessage{static_cast<uint8_t>(deviceIndex), true});
                }
            },
            view_.getDeviceSelectorElement());

        api_.onPressed(
            ButtonID::BOTTOM_LEFT,
            [this]()
            {
                protocol_.send(Protocol::RequestTrackListMessage{});
                if (trackInputHandler_)
                {
                    trackInputHandler_->setTrackListRequested(true);
                }
                // Don't hide device selector here - let handleTrackList() do it just before showing track selector
            },
            view_.getDeviceSelectorElement());
    }

    void DeviceInputHandler::handleParameterChange(uint8_t index, float value)
    {
        // Apply optimistic update to UI (knobs get instant feedback)
        // Buttons/Lists will be updated when echo comes back with displayValue
        view_controller_.handleParameterValue(index, value);

        // Send to host
        protocol_.send(Protocol::DeviceMacroValueChangeMessage{
            index,
            value,
            "",   // displayValue (host will echo back the formatted value)
            false // isEcho=false (this is not an echo, it's a new change from controller)
        });
    }

    void DeviceInputHandler::handlePageSelectorRelease()
    {
        const int selectedIndex = view_controller_.getPageSelectorSelectedIndex();

        view_controller_.handlePageSelectorConfirm();

        if (selectedIndex >= 0)
        {
            protocol_.send(Protocol::DevicePageSelectByIndexMessage{uint8_t(selectedIndex)});
        }

        // Reset state (encoder stays in Relative mode, no need to reset position)
        pageSelection_.requested = false;
    }

    void DeviceInputHandler::handleDeviceSelectorRequest()
    {
        if (!deviceList_.requested)
        {
            protocol_.send(Protocol::RequestDeviceListMessage{});
            deviceList_.requested = true;
        }
    }

    void DeviceInputHandler::handleDeviceSelectorNavigation(float delta)
    {
        int itemCount = view_.getDeviceSelectorItemCount();
        if (itemCount == 0)
            return;

        // In Relative mode, delta is ±1 (or ±N for fast scrolling)
        // Apply delta and wrap around list boundaries
        navigation_.currentSelectorIndex += static_cast<int>(delta);
        navigation_.currentSelectorIndex = wrapIndex(navigation_.currentSelectorIndex, itemCount);

        view_controller_.handleDeviceSelectorSetIndex(navigation_.currentSelectorIndex);
    }

    void DeviceInputHandler::handleDeviceSelectorEnter()
    {
        int selectorIndex = view_controller_.getDeviceSelectorSelectedIndex();
        if (selectorIndex < 0)
            return;

        switch (navigation_.mode)
        {
        case SelectorMode::DEVICES:
            handleDevicesModeEnter(selectorIndex);
            break;
        case SelectorMode::CHILDREN:
            handleChildrenModeEnter(selectorIndex);
            break;
    }
}

void DeviceInputHandler::handleDevicesModeEnter(int selectorIndex) {
    if (deviceList_.isNested && selectorIndex == 0) {
        protocol_.send(Protocol::ExitToParentMessage{});
        return;
    }

    const int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
    if (deviceIndex < 0 || deviceIndex >= deviceList_.count) return;

    // If device has children, request flat list (slots+layers+drums)
    if (hasChildren(deviceIndex)) {
        navigation_.deviceIndex = deviceIndex;
        // childType=0: Java now returns ALL children (flat list)
        protocol_.send(Protocol::RequestDeviceChildrenMessage{static_cast<uint8_t>(deviceIndex), 0});
    }
}

void DeviceInputHandler::handleChildrenModeEnter(int selectorIndex) {
    if (selectorIndex == 0) {
        handleBackNavigation();
    } else {
        int childIndex = selectorIndex - 1;

        // Get itemType for this child (0=slot, 1=layer, 2=drum)
        uint8_t itemType = (childIndex < static_cast<int>(navigation_.itemTypes.size()))
                            ? navigation_.itemTypes[childIndex] : 0;

        const char* itemTypeStr = (itemType == 0) ? "SLOT" : (itemType == 1) ? "LAYER" : (itemType == 2) ? "DRUM" : "UNKNOWN";
        Serial.printf("[C++ INPUT] Sending EnterDeviceChild: deviceIndex=%d, itemType=%d (%s), childIndex=%d\n",
                      navigation_.deviceIndex, itemType, itemTypeStr, childIndex);

        protocol_.send(Protocol::EnterDeviceChildMessage{navigation_.deviceIndex,
                                                         itemType,
                                                         static_cast<uint8_t>(childIndex)});
    }
}

void DeviceInputHandler::handleDeviceSelectorRelease() {
    const int selectorIndex = view_controller_.getDeviceSelectorSelectedIndex();

    // If track selector is open on top, close it too (user released device selector button first)
    if (view_.isTrackSelectorVisible()) {
        view_.hideTrackSelector();
    }

    if (selectorIndex >= 0) {
        switch (navigation_.mode) {
            case SelectorMode::DEVICES:
                if (deviceList_.isNested && selectorIndex == 0) {
                    protocol_.send(Protocol::ExitToParentMessage{});
                } else {
                    int deviceIndex = getAdjustedDeviceIndex(selectorIndex);
                    if (deviceIndex >= 0 && deviceIndex < deviceList_.count) {
                        protocol_.send(Protocol::DeviceSelectByIndexMessage{static_cast<uint8_t>(deviceIndex)});
                    }
                }
                break;

            case SelectorMode::CHILDREN:
                if (selectorIndex == 0) {
                    handleBackNavigation();
                } else {
                    int childIndex = selectorIndex - 1;

                    // Get itemType for this child (0=slot, 1=layer, 2=drum)
                    uint8_t itemType = (childIndex < static_cast<int>(navigation_.itemTypes.size()))
                                        ? navigation_.itemTypes[childIndex] : 0;

                    const char* itemTypeStr = (itemType == 0) ? "SLOT" : (itemType == 1) ? "LAYER" : (itemType == 2) ? "DRUM" : "UNKNOWN";
                    Serial.printf("[C++ INPUT] (Release) Sending EnterDeviceChild: deviceIndex=%d, itemType=%d (%s), childIndex=%d\n",
                                  navigation_.deviceIndex, itemType, itemTypeStr, childIndex);

                    protocol_.send(Protocol::EnterDeviceChildMessage{navigation_.deviceIndex,
                                                                     itemType,
                                                                     static_cast<uint8_t>(childIndex)});
                }
                break;
        }
        view_controller_.handleDeviceSelectorConfirm();
    } else {
        view_controller_.handleDeviceSelectorConfirm();
    }

    // Reset state (encoder stays in Relative mode, no need to reset position)
    deviceList_.requested = false;
}

bool DeviceInputHandler::hasChildren(uint8_t deviceIndex) const {
    if (deviceIndex >= Device::MAX_DEVICES) return false;
    return deviceList_.childrenTypes[deviceIndex][0] != Device::None;
}

uint8_t DeviceInputHandler::getFirstChildType(uint8_t deviceIndex) const {
    if (deviceIndex >= Device::MAX_DEVICES) return Device::None;
    return deviceList_.childrenTypes[deviceIndex][0];
}

int DeviceInputHandler::getAdjustedDeviceIndex(int selectorIndex) const {
    if (deviceList_.isNested) {
        return selectorIndex - 1;
    }
    return selectorIndex;
}

void DeviceInputHandler::handleBackNavigation() {
    // Back from children → show device list
    protocol_.send(Protocol::RequestDeviceListMessage{});
}

} // namespace Bitwig
