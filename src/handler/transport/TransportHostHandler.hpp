#pragma once

#include "../../protocol/Protocol.hpp"

class ControllerAPI;

namespace Bitwig
{

    class TransportBarController;

    class TransportHostHandler
    {
    public:
        TransportHostHandler(ControllerAPI &api, Protocol::Protocol &protocol, TransportBarController &controller);
        ~TransportHostHandler() = default;

    private:
        void setupProtocolCallbacks();

        ControllerAPI &api_;
        Protocol::Protocol &protocol_;
        TransportBarController &view_controller_;
    };

} // namespace Bitwig
