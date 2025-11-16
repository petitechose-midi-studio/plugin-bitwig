#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Protocol {
struct DeviceChangeMessage;
struct DeviceChangeHeaderMessage;
struct DeviceMacroUpdateMessage;
struct DeviceMacroDiscreteValuesMessage;
struct DevicePageChangeMessage;
struct TrackChangeMessage;
struct TrackListMessage;
}  // namespace Protocol

namespace Bitwig
{

    class DeviceView;

    class DeviceController
    {
    public:
        explicit DeviceController(DeviceView &view);

        /**
         * @brief Handle single parameter value change (optimistic update)
         * @param paramIndex Parameter index (0-7)
         * @param normalizedValue Normalized value (0.0-1.0)
         */
        void handleParameterValue(uint8_t paramIndex, float normalizedValue);

        /**
         * @brief Handle single parameter value change with formatted display (optimistic update)
         * @param paramIndex Parameter index (0-7)
         * @param normalizedValue Normalized value (0.0-1.0)
         * @param displayValue Formatted display value from Bitwig
         */
        void handleParameterValueWithDisplay(uint8_t paramIndex, float normalizedValue, const char *displayValue);

        /**
         * @brief Handle single parameter name change
         * @param paramIndex Parameter index (0-7)
         * @param name Parameter name
         */
        void handleParameterName(uint8_t paramIndex, const char *name);

        /**
         * @brief Handle device enabled/bypassed state change
         * @param enabled True if device is active, false if bypassed
         */
        void handleDeviceState(bool enabled);

        /**
         * @brief Handle device state change for specific device in list
         * @param deviceIndex Device index in the device bank
         * @param enabled True if device is active, false if bypassed
         */
        void handleDeviceStateAtIndex(uint8_t deviceIndex, bool enabled);

        /**
         * @brief Handle track context change (NEW - granular track updates)
         * Updates track name, color only
         * @param msg TrackChangeMessage from protocol
         */
        void handleTrackChange(const Protocol::TrackChangeMessage &msg);

        /**
         * @brief Handle device change header (NEW - optimized)
         * Updates device name, state, page info only (lightweight)
         * @param msg DeviceChangeHeaderMessage from protocol
         */
        void handleDeviceChangeHeader(const Protocol::DeviceChangeHeaderMessage &msg);

        /**
         * @brief Handle individual macro update (NEW - optimized)
         * Updates single parameter with type, name, value
         * @param msg DeviceMacroUpdateMessage from protocol
         */
        void handleMacroUpdate(const Protocol::DeviceMacroUpdateMessage &msg);

        /**
         * @brief Handle discrete values for List parameter (NEW - optimized)
         * Updates discrete value names for List widgets
         * @param msg DeviceMacroDiscreteValuesMessage from protocol
         */
        void handleMacroDiscreteValues(const Protocol::DeviceMacroDiscreteValuesMessage &msg);

        /**
         * @brief Handle device page change (user navigates pages)
         * Updates page info and all 8 parameters for new page
         * @param msg DevicePageChangeMessage from protocol
         */
        void handlePageChange(const Protocol::DevicePageChangeMessage &msg);

        /**
         * @brief Handle page selector display request
         * @param pageNames Vector of available page names
         * @param currentIndex Currently active page index
         */
        void handleShowPageSelector(const std::vector<std::string> &pageNames, int currentIndex);

        /**
         * @brief Handle page selector navigation (encoder position)
         * @param index Absolute page index (already wrapped)
         */
        void handlePageSelectorSetIndex(int index);

        /**
         * @brief Handle page selector confirmation (button release)
         * Hides selector (InputHandler will send protocol message)
         */
        void handlePageSelectorConfirm();

        /**
         * @brief Get selected page index from selector
         * @return Selected index, or -1 if not visible
         */
        int getPageSelectorSelectedIndex() const;

        /**
         * @brief Handle device selector display request
         * @param items Vector of formatted device names (already formatted with [S]/[L]/etc.)
         * @param currentIndex Currently active device index
         */
        void handleShowDeviceSelector(const std::vector<std::string> &items, int currentIndex);

        /**
         * @brief Show device selector with colored indicators
         * @param names Device names (without text indicators)
         * @param currentIndex Current device index
         * @param deviceStates Device enabled states (per device)
         * @param hasSlots Device has slots (per device)
         * @param hasLayers Device has layers (per device)
         * @param hasDrums Device has drum pads (per device)
         */
        void handleShowDeviceSelectorWithIndicators(
            const std::vector<std::string> &names,
            int currentIndex,
            const std::vector<bool> &deviceStates,
            const std::vector<bool> &hasSlots,
            const std::vector<bool> &hasLayers,
            const std::vector<bool> &hasDrums);

        /**
         * @brief Handle device children display request
         * @param items Vector of formatted children names (folders or actual children)
         */
        void handleShowDeviceChildren(const std::vector<std::string> &items);

        /**
         * @brief Handle device selector navigation (encoder position)
         * @param index Absolute item index (already wrapped)
         */
        void handleDeviceSelectorSetIndex(int index);

        /**
         * @brief Handle device selector confirmation (hide overlay)
         * Just hides selector (InputHandler handles protocol messages)
         */
        void handleDeviceSelectorConfirm();

        /**
         * @brief Get selected device index from selector
         * @return Selected index, or -1 if not visible
         */
        int getDeviceSelectorSelectedIndex() const;

        /**
         * @brief Handle track list display request
         * @param msg Track list message from protocol
         */
        void handleTrackList(const Protocol::TrackListMessage &msg);

        /**
         * @brief Handle track mute state change
         * @param trackIndex Track index
         * @param isMuted New mute state
         */
        void handleTrackMuteState(uint8_t trackIndex, bool isMuted);

        /**
         * @brief Handle track solo state change
         * @param trackIndex Track index
         * @param isSoloed New solo state
         */
        void handleTrackSoloState(uint8_t trackIndex, bool isSoloed);

    private:
        DeviceView &view_;
    };

} // namespace Bitwig
