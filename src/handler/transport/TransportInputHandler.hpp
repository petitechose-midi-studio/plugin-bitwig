#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"

namespace Bitwig
{

    class TransportBarController;

    /**
     * @brief Hardware input handler for transport controls (GLOBAL)
     *
     * OPTIMISTIC UPDATES for transport (instant feedback):
     * - Update UI immediately
     * - Send to Bitwig
     * - HostHandler syncs state on confirmation
     */
    class TransportInputHandler
    {
    public:
        TransportInputHandler(ControllerAPI &api, TransportBarController &controller,
                              Protocol::Protocol &protocol);
        ~TransportInputHandler() = default;

    private:
        void setupBindings();
        void togglePlay();
        void toggleRecord();
        void stop();
        void adjustTempo(float normalizedValue);

        ControllerAPI &api_;
        TransportBarController &view_controller_;
        Protocol::Protocol &protocol_;

        bool isPlaying_ = false;
        bool isRecording_ = false;
    };

} // namespace Bitwig
