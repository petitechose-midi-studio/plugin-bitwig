#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "DeviceConstants.hpp"
#include <memory>

namespace Bitwig
{

    class DeviceView;
    class DeviceController;
    class TrackInputHandler;

    /**
     * @brief Hardware input handler for DeviceView (SCOPED)
     *
     * OPTIMISTIC UPDATES for encoders (high-frequency, latency-critical):
     * - Update UI immediately (no lag)
     * - Send to Bitwig
     * - HostHandler syncs encoder position on confirmation
     *
     * Other actions (button press, etc.): Send only
     */
    class DeviceInputHandler
    {
    public:
        DeviceInputHandler(ControllerAPI &api, DeviceView &view, DeviceController &controller,
                           Protocol::Protocol &protocol, lv_obj_t *scope);
        ~DeviceInputHandler();

        /**
         * @brief Convert Bitwig parameter index to physical encoder ID
         * @param paramIndex Bitwig macro parameter index (0-7)
         * @return Physical encoder ID, or EncoderID(0) if invalid
         */
        static EncoderID getEncoderIdForParameter(uint8_t paramIndex);

        /**
         * @brief Update page selection state from host
         * Called by DeviceHostHandler when DEVICE_PAGE_NAMES is received
         */
        void setPageSelectionState(uint8_t pageCount, uint8_t currentIndex);

        /**
         * @brief Update device list state from host
         * Called by DeviceHostHandler when DEVICE_LIST is received
         */
        void setDeviceListState(uint8_t deviceCount, uint8_t currentDeviceIndex, bool isNested,
                                const etl::array<uint8_t, 4> *childrenTypes, uint8_t childrenTypesCount);

        /**
         * @brief Update device children state from host
         * Called by DeviceHostHandler when DEVICE_CHILDREN is received
         */
        void setDeviceChildrenState(uint8_t deviceIndex, uint8_t childType, uint8_t childrenCount,
                                    const std::vector<uint8_t> &itemTypes);

        /**
         * @brief Update track list state from host
         * Called by DeviceHostHandler when TRACK_LIST is received
         */
        void setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested);

        /**
         * @brief Check if device list was explicitly requested by user
         * @return true if user requested the device list, false otherwise
         */
        bool isDeviceListRequested() const { return deviceList_.requested; }

        /**
         * @brief Check if track list was explicitly requested by user
         * @return true if user requested the track list, false otherwise
         */
        bool isTrackListRequested() const;

    private:
        void setupBindings();
        void setupMacroBindings();
        void setupPageSelectorBindings();
        void setupDeviceSelectorBindings();

        void handleParameterChange(uint8_t index, float value);
        void handlePageSelectorRelease();

        void handleDeviceSelectorRequest();
        void handleDeviceSelectorNavigation(float position);
        void handleDeviceSelectorEnter();
        void handleDeviceSelectorRelease();

        void handleDevicesModeEnter(int selectorIndex);
        void handleChildrenModeEnter(int selectorIndex);

        bool hasChildren(uint8_t deviceIndex) const;
        uint8_t getFirstChildType(uint8_t deviceIndex) const;
        int getAdjustedDeviceIndex(int selectorIndex) const;
        void handleBackNavigation();

        // Helper for circular index wrapping (handles negative values)
        static int wrapIndex(int value, int modulo);

        ControllerAPI &api_;
        DeviceView &view_;
        lv_obj_t *scope_; // Scope for input bindings
        DeviceController &view_controller_;
        Protocol::Protocol &protocol_;

        // Track input handler (manages TrackListSelector interactions)
        std::unique_ptr<TrackInputHandler> trackInputHandler_;

        // Page selection state
        struct
        {
            uint8_t count = 0;
            uint8_t cursor = 0;
            bool requested = false;
        } pageSelection_;

        // Device list state
        struct
        {
            uint8_t count = 0;
            uint8_t currentIndex = 0;
            bool isNested = false;
            bool requested = false;
            etl::array<etl::array<uint8_t, Device::MAX_CHILD_TYPES>, Device::MAX_DEVICES> childrenTypes;
        } deviceList_;

        // Navigation state
        enum class SelectorMode
        {
            DEVICES,
            CHILDREN
        };
        struct
        {
            SelectorMode mode = SelectorMode::DEVICES;
            uint8_t deviceIndex = 0; // Device being navigated (for children request)
            uint8_t childType = 0;  // Deprecated - kept for compatibility
            uint8_t childrenCount = 0;
            etl::array<uint8_t, 16> itemTypes = {};  // Store itemType for each child
        } navigation_;
    };

} // namespace Bitwig
