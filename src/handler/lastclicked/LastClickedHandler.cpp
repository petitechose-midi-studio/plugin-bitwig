#include "LastClickedHandler.hpp"

namespace Bitwig
{

    LastClickedHandler::LastClickedHandler(ControllerAPI &api, Protocol::Protocol &protocol, lv_obj_t *scope)
        : api_(api), protocol_(protocol), scope_(scope)
    {

        // Initialize current parameter state
        currentParameter_.name = "";
        currentParameter_.value = 0.0f;
        currentParameter_.displayValue = "";
        currentParameter_.parameterType = 0;  // Knob
        currentParameter_.discreteCount = -1; // Continuous
        currentParameter_.exists = false;

        setupProtocolCallbacks();
        setupInputBindings();
    }

    LastClickedHandler::~LastClickedHandler()
    {
        api_.clearScope(scope_);
    }

    void LastClickedHandler::setupProtocolCallbacks()
    {
        // Host → Controller: Parameter info when user clicks a new parameter
        protocol_.onLastClickedUpdate = [this](const Protocol::LastClickedUpdateMessage &msg)
        {
            if (!msg.fromHost)
                return;
            handleLastClickedUpdate(msg);
        };
    }

    void LastClickedHandler::setupInputBindings()
    {
        // OPT encoder turn
        api_.onTurned(EncoderID::OPT, [this](float value)
                      { handleEncoderTurn(value); }, scope_);
    }

    void LastClickedHandler::handleLastClickedUpdate(const Protocol::LastClickedUpdateMessage &msg)
    {
        // Store parameter info
        currentParameter_.name = std::string(msg.parameterName.data());
        currentParameter_.value = msg.parameterValue;
        currentParameter_.displayValue = std::string(msg.displayValue.data());
        currentParameter_.parameterType = msg.parameterType;
        currentParameter_.discreteCount = msg.discreteValueCount;
        currentParameter_.exists = msg.parameterExists;

        if (!currentParameter_.exists)
        {
            return;
        }

        // Configure OPT encoder based on parameter type
        if (currentParameter_.parameterType == 0)
        {
            api_.setEncoderContinuous(EncoderID::OPT);
        }
        else
        {
            api_.setEncoderDiscreteSteps(EncoderID::OPT, currentParameter_.discreteCount);
        }

        // Set encoder position
        api_.setEncoderPosition(EncoderID::OPT, currentParameter_.value);
    }

    void LastClickedHandler::handleLastClickedValueChange(const Protocol::LastClickedValueChangeMessage &msg)
    {
        // Host sent back the value - update our cache
        currentParameter_.value = msg.parameterValue;
        api_.setEncoderPosition(EncoderID::OPT, msg.parameterValue);
    }

    void LastClickedHandler::handleEncoderTurn(float value)
    {
        if (!currentParameter_.exists)
        {
            return;
        }

        currentParameter_.value = value;

        // Send encoder value to host
        protocol_.send(Protocol::LastClickedValueChangeMessage{
            value,
            "",
            false});
    }

} // namespace Bitwig
