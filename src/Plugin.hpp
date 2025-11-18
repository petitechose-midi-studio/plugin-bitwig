#pragma once

#include "resource/common/interface/IPlugin.hpp"
#include "api/ControllerAPI.hpp"
#include "protocol/Protocol.hpp"
#include "ui/ViewRegistry.hpp"
#include "ui/ViewContainer.hpp"
#include "ui/ViewManager.hpp"
#include "ui/splash/SplashView.hpp"
#include "ui/device/DeviceView.hpp"
#include "ui/device/DeviceController.hpp"
#include "ui/transportbar/TransportBar.hpp"
#include "ui/transportbar/TransportBarController.hpp"
#include "handler/device/DeviceInputHandler.hpp"
#include "handler/device/DeviceHostHandler.hpp"
#include "handler/transport/TransportInputHandler.hpp"
#include "handler/transport/TransportHostHandler.hpp"
#include "handler/lastclicked/LastClickedHandler.hpp"
#include "handler/plugin/PluginLifecycleHandler.hpp"

namespace Bitwig
{

    class PluginHostHandler;

    class Plugin : public IPlugin
    {
    public:
        explicit Plugin(ControllerAPI &api);
        ~Plugin() override;

        bool initialize() override;
        void cleanup() override {}

        const char *getName() const override { return "Bitwig Studio"; }
        bool isEnabled() const override { return enabled_; }
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        void update() override {}

    private:
        ControllerAPI &api_;
        bool enabled_;

        Protocol::Protocol protocol_;
        ViewContainer viewContainer_;

        struct
        {
            TransportBar bar;
            TransportBarController controller;
            TransportInputHandler inputHandler;
            TransportHostHandler hostHandler;
        } transport_;

        struct
        {
            DeviceView view;
            DeviceController controller;
            DeviceInputHandler inputHandler;
            DeviceHostHandler hostHandler;
        } device_;

        struct
        {
            SplashView view;
        } splash_;

        struct
        {
            LastClickedHandler handler;
        } lastClicked_;

        ViewRegistry viewRegistry_;
        ViewManager viewManager_;

        struct
        {
            PluginLifecycleHandler handler;
        } lifecycle_;
    };

} // namespace Bitwig
