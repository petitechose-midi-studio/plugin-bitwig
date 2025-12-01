#pragma once

#include "../../protocol/Protocol.hpp"
#include "../../protocol/struct/HostInitializedMessage.hpp"
#include "../../protocol/struct/HostDeactivatedMessage.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    class ViewManager;

    class PluginLifecycleHandler
    {
    public:
        explicit PluginLifecycleHandler(ViewManager &viewManager, Protocol::Protocol &protocol);
        ~PluginLifecycleHandler() = default;

        bool isHostActive() const { return is_host_active_; }

    private:
        void setupProtocolCallbacks();

        void handleHostInitialized(const Protocol::HostInitializedMessage &msg);
        void handleHostDeactivated(const Protocol::HostDeactivatedMessage &msg);

        ViewManager &view_manager_;
        Protocol::Protocol &protocol_;
        bool is_host_active_ = false;
    };

} // namespace Bitwig
