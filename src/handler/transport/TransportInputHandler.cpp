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
        // Configure tempo encoder for Relative mode (no bounds to avoid initialization event)
        api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);

        api_.onPressed(ButtonID::BOTTOM_LEFT, [this]()
                       { togglePlay(); });

        api_.onPressed(ButtonID::BOTTOM_RIGHT, [this]()
                       { toggleRecord(); });

        api_.onPressed(ButtonID::BOTTOM_CENTER, [this]()
                       { stop(); });

        api_.onTurned(EncoderID::NAV, [this](float delta)
                      { adjustTempo(delta); });
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

    void TransportInputHandler::adjustTempo(float delta)
    {
        // In Relative mode, encoder sends delta directly (±1)
        protocol_.send(Protocol::TransportTempoMessage{delta});
    }

} // namespace Bitwig
