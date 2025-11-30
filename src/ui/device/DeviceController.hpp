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
struct DeviceListMessage;
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
         * @param deviceIndex Device index in the device bank
         * @param enabled True if device is active, false if bypassed
         */
        void handleDeviceState(uint8_t deviceIndex, bool enabled);

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
         * @brief Update current device index and hasChildren state for top bar
         * @param deviceIndex Current device index
         * @param hasChildren Whether device has slots/layers/drums
         *
         * Called from DeviceHostHandler when device list is received.
         */
        void handleCurrentDeviceInfo(uint8_t deviceIndex, bool hasChildren);

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
         * @brief Handle page selector confirmation (button release)
         * Hides selector (InputHandler will send protocol message)
         */
        void handlePageSelectorConfirm();

        /**
         * @brief Get selected page index from selector
         * @return Selected index, or -1 if not visible
         */
        int getPageSelectorSelectedIndex() const;

        void handleDeviceList(const Protocol::DeviceListMessage &msg);

        void handleShowDeviceSelectorWithIndicators(
            const std::vector<std::string> &names,
            int currentIndex,
            const std::vector<bool> &deviceStates,
            const std::vector<bool> &hasSlots,
            const std::vector<bool> &hasLayers,
            const std::vector<bool> &hasDrums);

        /**
         * @brief Handle device children display request
         * @param items Vector of formatted children names
         * @param itemTypes Vector of item types (0=LIST/slot, 1=COPY/layer, 2=KEYBOARD/drum)
         */
        void handleShowDeviceChildren(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes);

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
        int toDisplayIndex(int rawIndex) const
        {
            return is_device_nested_ ? rawIndex + 1 : rawIndex;
        }

        int toTrackDisplayIndex(int rawIndex) const
        {
            return is_track_nested_ ? rawIndex + 1 : rawIndex;
        }

        DeviceView &view_;
        bool is_device_nested_ = false;
        bool is_track_nested_ = false;
        int current_device_index_ = -1;  // Currently selected device index
        bool current_device_has_children_ = false;  // Current device has slots/layers/drums
    };

} // namespace Bitwig
