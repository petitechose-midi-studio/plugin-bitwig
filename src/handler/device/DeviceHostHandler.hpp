#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"
#include "DeviceConstants.hpp"
#include <array>
#include <string>

namespace Bitwig
{

    class DeviceView;
    class DeviceInputHandler;

    /**
     * @brief Receives device updates FROM Bitwig host and updates view state directly
     */
    class DeviceHostHandler
    {
    public:
        DeviceHostHandler(Protocol::Protocol &protocol, ControllerAPI &api, DeviceView &view,
                          DeviceInputHandler &inputHandler);
        ~DeviceHostHandler() = default;

    private:
        void setupProtocolCallbacks();

        template <typename MacroArray>
        void updateMacroEncoderPositions(const MacroArray &macros);

        int toDeviceDisplayIndex(int rawIndex) const;
        int toTrackDisplayIndex(int rawIndex) const;

        Protocol::Protocol &protocol_;
        ControllerAPI &api_;
        DeviceView &view_;
        DeviceInputHandler &input_handler_;

        uint8_t parameterTypes_[Device::PARAMETER_COUNT] = {};
    };

} // namespace Bitwig
