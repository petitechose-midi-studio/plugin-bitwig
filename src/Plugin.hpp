#pragma once

#include "api/ControllerAPI.hpp"
#include "handler/device/DeviceHostHandler.hpp"
#include "handler/device/DeviceInputHandler.hpp"
#include "handler/lastclicked/LastClickedHostHandler.hpp"
#include "handler/lastclicked/LastClickedInputHandler.hpp"
#include "handler/lastclicked/LastClickedState.hpp"
#include "handler/midi/MidiActivityHandler.hpp"
#include "handler/plugin/PluginLifecycleHandler.hpp"
#include "handler/transport/TransportHostHandler.hpp"
#include "handler/transport/TransportInputHandler.hpp"
#include "protocol/Protocol.hpp"
#include "resource/common/interface/IPlugin.hpp"
#include "ui/ViewContainer.hpp"
#include "ui/ViewManager.hpp"
#include "ui/ViewRegistry.hpp"
#include "ui/device/DeviceView.hpp"
#include "ui/splash/SplashView.hpp"
#include "ui/transportbar/TransportBar.hpp"
#include "ui/transportbar/TransportBarController.hpp"

namespace Bitwig {

class Plugin : public IPlugin {
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
    // Handler groups (no circular dependencies)
    struct TransportHandlers {
        TransportInputHandler input;
        TransportHostHandler host;
    };

    struct DeviceHandlers {
        DeviceInputHandler input;
        DeviceHostHandler host;
    };

    struct LastClickedHandlers {
        LastClickedInputHandler input;
        LastClickedHostHandler host;
    };

    // Core dependencies (initialized first)
    ControllerAPI &api_;
    bool enabled_;
    Protocol::Protocol protocol_;
    ViewContainer view_container_;

    // Views and state objects (initialized second, no dependencies on handlers)
    TransportBar transport_bar_;
    TransportBarController transport_controller_;
    DeviceView device_view_;
    SplashView splash_view_;
    LastClickedState last_clicked_state_;

    // Handlers (initialized third, can safely reference views/state)
    TransportHandlers transport_handlers_;
    DeviceHandlers device_handlers_;
    LastClickedHandlers last_clicked_handlers_;
    MidiActivityHandler midi_handler_;

    // View management (initialized last)
    ViewRegistry view_registry_;
    ViewManager view_manager_;
    PluginLifecycleHandler lifecycle_handler_;
};

}  // namespace Bitwig
