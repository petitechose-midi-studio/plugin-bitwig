#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "DeviceConstants.hpp"
#include <etl/string.h>
#include <etl/array.h>

namespace Bitwig
{

    class DeviceView;
    class DeviceController;
    class DeviceInputHandler;

    /**
     * @brief Listen to device updates FROM Bitwig host
     *
     * PARAMETER VALUES (encoders):
     * - Sync encoder position ONLY (UI already updated optimistically by InputHandler)
     *
     * OTHER UPDATES (device name, param names, etc.):
     * - Update UI (server authority)
     *
     * IMPORTANT: Do NOT update UI for parameter values (already done optimistically).
     * This prevents redundant updates and maintains instant UI feedback.
     */
    class DeviceHostHandler
    {
    public:
        DeviceHostHandler(Protocol::Protocol &protocol, ControllerAPI &api, DeviceView &view,
                          DeviceController &controller, DeviceInputHandler &inputHandler);
        ~DeviceHostHandler() = default;

    private:
        void setupProtocolCallbacks();

        template <typename MacroArray>
        void updateMacroEncoderPositions(const MacroArray &macros);

        Protocol::Protocol &protocol_;
        ControllerAPI &api_;
        DeviceView &view_;
        DeviceController &view_controller_;
        DeviceInputHandler &input_handler_;

        // Parameter type cache (Knob/Button/List) from last device/page change
        uint8_t parameterTypes_[Device::PARAMETER_COUNT] = {};
    };

} // namespace Bitwig
