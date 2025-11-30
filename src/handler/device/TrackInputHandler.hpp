#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"

namespace Bitwig
{

    class DeviceView;
    class DeviceController;

    /**
     * @brief Hardware input handler for TrackSelector overlay
     *
     * Handles track navigation when TrackSelector overlay is visible:
     * - Close on BOTTOM_LEFT press
     * - Navigate with encoder while holding
     * - Enter/exit track groups
     * - Mute/Solo toggles
     */
    class TrackInputHandler
    {
    public:
        TrackInputHandler(ControllerAPI &api, DeviceView &view, DeviceController &controller,
                          Protocol::Protocol &protocol);
        ~TrackInputHandler();

        void setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested);
        bool isTrackListRequested() const { return state_.requested; }
        void setTrackListRequested(bool requested) { state_.requested = requested; }

    private:
        void setupBindings();

        void navigate(float delta);
        void select();
        void selectAndDive();
        void close();
        void toggleMute();
        void toggleSolo();

        int getSelectedTrackIndex() const;
        int getAdjustedTrackIndex(int selectorIndex) const;

        ControllerAPI &api_;
        DeviceView &view_;
        DeviceController &controller_;
        Protocol::Protocol &protocol_;

        struct TrackListState
        {
            uint8_t count = 0;
            uint8_t currentIndex = 0;
            bool isNested = false;
            bool requested = false;
            int currentSelectorIndex = 0;
        };
        TrackListState state_;
    };

} // namespace Bitwig
