#include "TransportInputHandler.hpp"
#include "../../ui/transportbar/TransportBarController.hpp"
#include "../../protocol/MessageStructure.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    TransportInputHandler::TransportInputHandler(ControllerAPI &api,
                                                 TransportBarController &controller,
                                                 Protocol::Protocol &protocol)
        : api_(api), view_controller_(controller), protocol_(protocol)
    {
        setupBindings();
    }

    void TransportInputHandler::setupBindings()
    {
        api_.onPressed(ButtonID::BOTTOM_LEFT, [this]()
                       { togglePlay(); });

        api_.onPressed(ButtonID::BOTTOM_RIGHT, [this]()
                       { toggleRecord(); });

        api_.onPressed(ButtonID::BOTTOM_CENTER, [this]()
                       { stop(); });
    }

    void TransportInputHandler::togglePlay()
    {
        protocol_.send(Protocol::TransportPlayMessage{!isPlaying_});
    }

    void TransportInputHandler::toggleRecord()
    {
        protocol_.send(Protocol::TransportRecordMessage{!isRecording_});
    }

    void TransportInputHandler::stop()
    {
        protocol_.send(Protocol::TransportStopMessage{});
    }

} // namespace Bitwig
