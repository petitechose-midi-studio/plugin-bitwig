#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"

namespace Bitwig
{

    /**
     * @brief Hardware input handler for transport controls (GLOBAL)
     *
     * Sends transport commands to Bitwig:
     * - Play/Pause toggle
     * - Record toggle
     * - Stop
     * - Tempo adjustment
     *
     * State is managed by TransportHostHandler (receives confirmations from Bitwig).
     */
    class TransportInputHandler
    {
    public:
        TransportInputHandler(ControllerAPI &api, Protocol::Protocol &protocol);
        ~TransportInputHandler() = default;

    private:
        void setupInputBindings();
        void togglePlay();
        void toggleRecord();
        void stop();
        void adjustTempo(float delta);

        ControllerAPI &api_;
        Protocol::Protocol &protocol_;
    };

} // namespace Bitwig
