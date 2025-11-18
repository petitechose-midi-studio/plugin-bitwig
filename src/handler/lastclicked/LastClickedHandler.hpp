#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "../../protocol/struct/LastClickedUpdateMessage.hpp"
#include "../../protocol/struct/LastClickedValueChangeMessage.hpp"
#include "../../protocol/struct/LastClickedTouchMessage.hpp"
#include <string>

namespace Bitwig
{

    /**
     * @brief Last Clicked Parameter Handler
     *
     * Handles control of Bitwig's last clicked parameter via OPT encoder.
     * Completely separate from device macros - works with any parameter in Bitwig.
     *
     * Responsibilities:
     * - Receive LAST_CLICKED_UPDATE from host (when user clicks a parameter)
     * - Configure OPT encoder (continuous/discrete, position)
     * - Handle OPT encoder turns and button press
     * - Send value changes back to host
     *
     * Flow:
     * 1. User clicks parameter in Bitwig → LAST_CLICKED_UPDATE received
     * 2. Configure OPT encoder based on parameter type
     * 3. User turns OPT → send LAST_CLICKED_VALUE_CHANGE
     * 4. Host updates parameter → receive echo with displayValue
     */
    class LastClickedHandler
    {
    public:
        LastClickedHandler(ControllerAPI &api, Protocol::Protocol &protocol, lv_obj_t *scope);
        ~LastClickedHandler();

    private:
        void setupProtocolCallbacks();
        void setupInputBindings();

        void handleLastClickedUpdate(const Protocol::LastClickedUpdateMessage &msg);
        void handleLastClickedValueChange(const Protocol::LastClickedValueChangeMessage &msg);

        void handleEncoderTurn(float value);

        ControllerAPI &api_;
        Protocol::Protocol &protocol_;
        lv_obj_t *scope_;

        // Current parameter state
        struct
        {
            std::string name;
            float value;
            std::string displayValue;
            uint8_t parameterType; // 0=Knob, 1=Button, 2=List
            int16_t discreteCount; // -1=continuous, N=steps
            bool exists;
        } currentParameter_;
    };

} // namespace Bitwig
