#include "PluginLifecycleHandler.hpp"
#include "../../ui/ViewManager.hpp"
#include "../../protocol/struct/RequestHostStatusMessage.hpp"

namespace Bitwig
{

    PluginLifecycleHandler::PluginLifecycleHandler(ViewManager &viewManager, Protocol::Protocol &protocol)
        : view_manager_(viewManager), protocol_(protocol)
    {
        setupProtocolCallbacks();
        protocol_.send(Protocol::RequestHostStatusMessage());
    }

    void PluginLifecycleHandler::setupProtocolCallbacks()
    {
        protocol_.onHostInitialized = [this](const Protocol::HostInitializedMessage &msg)
        {
            handleHostInitialized(msg);
        };

        protocol_.onHostDeactivated = [this](const Protocol::HostDeactivatedMessage &msg)
        {
            handleHostDeactivated(msg);
        };
    }

    void PluginLifecycleHandler::handleHostInitialized(const Protocol::HostInitializedMessage &msg)
    {
        if (is_host_active_) return;  // Already initialized, ignore duplicate
        LOGLN("[PluginLifecycle] Host initialized - showing splash");
        is_host_active_ = msg.isHostActive;
        view_manager_.showSplash(1500, "Bitwig is starting");
    }

    void PluginLifecycleHandler::handleHostDeactivated(const Protocol::HostDeactivatedMessage &)
    {
        is_host_active_ = false;
        view_manager_.showCore();
    }
}