#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "DeviceConstants.hpp"

namespace Bitwig {

class DeviceController;

/**
 * @brief Hardware input handler for macro encoders and buttons
 *
 * Handles the 8 macro parameter controls:
 * - Encoder turns for value changes
 * - Button press/release for touch messages
 */
class MacroInputHandler {
public:
    MacroInputHandler(ControllerAPI& api, DeviceController& controller,
                      Protocol::Protocol& protocol, lv_obj_t* scope);
    ~MacroInputHandler();

    static EncoderID getEncoderIdForParameter(uint8_t paramIndex);

private:
    void setupBindings();
    void handleValueChange(uint8_t index, float value);
    void sendTouch(uint8_t index, bool touched);

    ControllerAPI& api_;
    DeviceController& controller_;
    Protocol::Protocol& protocol_;
    lv_obj_t* scope_;
};

} // namespace Bitwig
