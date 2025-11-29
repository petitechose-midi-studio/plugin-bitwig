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
#include "handler/midi/MidiInputHandler.hpp"
#include "handler/plugin/PluginLifecycleHandler.hpp"

namespace Bitwig
{

    class PluginHostHandler;

    class Plugin : public IPlugin
    {
    public:
        // Called by PluginManager before construction
        static void loadResources();

        explicit Plugin(ControllerAPI &api);
        ~Plugin() override;

        bool initialize() override;
        void cleanup() override {}

        const char *getName() const override { return "Bitwig Studio"; }
        bool isEnabled() const override { return enabled_; }
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        void update() override {}

    private:
        // Module structs - group related components
        struct TransportModule {
            TransportBar bar;
            TransportBarController controller;
            TransportInputHandler inputHandler;
            TransportHostHandler hostHandler;
        };

        struct DeviceModule {
            DeviceView view;
            DeviceController controller;
            DeviceInputHandler inputHandler;
            DeviceHostHandler hostHandler;
        };

        struct SplashModule {
            SplashView view;
        };

        struct LastClickedModule {
            LastClickedHandler handler;
        };

        struct MidiModule {
            MidiInputHandler handler;
        };

        struct LifecycleModule {
            PluginLifecycleHandler handler;
        };

        // Members
        ControllerAPI &api_;
        bool enabled_;

        Protocol::Protocol protocol_;
        ViewContainer viewContainer_;

        TransportModule transport_;
        DeviceModule device_;
        SplashModule splash_;
        LastClickedModule lastClicked_;
        MidiModule midi_;

        ViewRegistry viewRegistry_;
        ViewManager viewManager_;

        LifecycleModule lifecycle_;
    };

} // namespace Bitwig
