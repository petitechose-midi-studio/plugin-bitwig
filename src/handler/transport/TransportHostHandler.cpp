#include "TransportHostHandler.hpp"
#include "../../ui/transportbar/TransportBarController.hpp"
#include "TransportInputHandler.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    TransportHostHandler::TransportHostHandler(Protocol::Protocol &protocol,
                                               TransportBarController &controller)
        : protocol_(protocol), view_controller_(controller)
    {
        setupProtocolCallbacks();
    }

    void TransportHostHandler::setupProtocolCallbacks()
    {
        protocol_.onTransportPlay = [this](const Protocol::TransportPlayMessage &msg)
        {
            view_controller_.setPlaying(msg.isPlaying);
        };

        protocol_.onTransportRecord = [this](const Protocol::TransportRecordMessage &msg)
        {
            view_controller_.setRecording(msg.isRecording);
        };
        protocol_.onTransportStop = [this](const Protocol::TransportStopMessage &msg)
        {
            view_controller_.setPlaying(false);
            view_controller_.setRecording(false);
        };
    }

} // namespace Bitwig
