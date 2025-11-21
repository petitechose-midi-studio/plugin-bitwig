#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "TrackConstants.hpp"

namespace Bitwig
{

    class DeviceView;
    class DeviceController;

    /**
     * @brief Hardware input handler for TrackListSelector (SCOPED)
     *
     * Handles track navigation and actions when TrackListSelector is visible.
     * DeviceInputHandler triggers the display, TrackInputHandler manages interaction.
     */
    class TrackInputHandler
    {
    public:
        TrackInputHandler(ControllerAPI &api, DeviceView &view, DeviceController &controller,
                          Protocol::Protocol &protocol, lv_obj_t *scope);
        ~TrackInputHandler();

        /**
         * @brief Update track list state from host
         * Called by DeviceHostHandler when TRACK_LIST is received
         */
        void setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested);

        /**
         * @brief Check if track list was explicitly requested by user
         * @return true if user requested the track list, false otherwise
         */
        bool isTrackListRequested() const { return trackList_.requested; }

        /**
         * @brief Mark track list as requested by user
         * @param requested True if user requested the track list
         */
        void setTrackListRequested(bool requested) { trackList_.requested = requested; }

    private:
        void setupBindings();

        void handleTrackSelectorNavigation(float position);
        void handleTrackSelectorRelease();
        void handleTrackSelectorEnter();
        void handleTrackMute();
        void handleTrackSolo();

        // Helper for circular index wrapping (handles negative values)
        static int wrapIndex(int value, int modulo);

        ControllerAPI &api_;
        DeviceView &view_;
        lv_obj_t *scope_; // Scope for input bindings (TrackListSelector overlay)
        DeviceController &view_controller_;
        Protocol::Protocol &protocol_;

        // Track list state
        struct
        {
            uint8_t count = 0;
            uint8_t currentIndex = 0;
            bool isNested = false;
            bool requested = false;
        } trackList_;
    };

} // namespace Bitwig
