#pragma once

#include "../../protocol/Protocol.hpp"
#include "../../protocol/struct/HostInitializedMessage.hpp"
#include "../../protocol/struct/HostDeactivatedMessage.hpp"
#include "log/Macros.hpp"

namespace Plugin::Bitwig {

class ViewManager;

class PluginLifecycleHandler {
public:
    explicit PluginLifecycleHandler(ViewManager& viewManager, Protocol::Protocol& protocol);
    ~PluginLifecycleHandler() = default;

    bool isHostActive() const { return isHostActive_; }

private:
    void setupProtocolCallbacks();

    void handleHostInitialized(const Protocol::HostInitializedMessage& msg);
    void handleHostDeactivated(const Protocol::HostDeactivatedMessage& msg);

    ViewManager& viewManager_;
    Protocol::Protocol& protocol_;
    bool isHostActive_ = false;
};

} // namespace Plugin::Bitwig
