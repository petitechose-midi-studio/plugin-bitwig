#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "DeviceConstants.hpp"

namespace Bitwig {

class DeviceView;

/**
 * @brief Hardware input handler for macro encoders and buttons
 *
 * Handles the 8 macro parameter controls:
 * - Encoder turns for value changes (optimistic UI update)
 * - Button press/release for touch messages
 */
class MacroInputHandler {
public:
    MacroInputHandler(ControllerAPI& api, DeviceView& view,
                      Protocol::Protocol& protocol, lv_obj_t* scope);
    ~MacroInputHandler();

    static EncoderID getEncoderIdForParameter(uint8_t paramIndex);

private:
    void setupInputBindings();
    void handleValueChange(uint8_t index, float value);
    void sendTouch(uint8_t index, bool touched);

    ControllerAPI& api_;
    DeviceView& view_;
    Protocol::Protocol& protocol_;
    lv_obj_t* scope_;
};

} // namespace Bitwig
