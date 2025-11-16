#include "PluginLifecycleHandler.hpp"
#include "../../ui/ViewManager.hpp"
#include "../../protocol/struct/RequestHostStatusMessage.hpp"

namespace Plugin::Bitwig {

PluginLifecycleHandler::PluginLifecycleHandler(ViewManager& viewManager, Protocol::Protocol& protocol)
    : viewManager_(viewManager), protocol_(protocol) {
    setupProtocolCallbacks();
    // Demander le statut de l'host
    protocol_.send(Protocol::RequestHostStatusMessage());
}

void PluginLifecycleHandler::setupProtocolCallbacks() {
    protocol_.onHostInitialized = [this](const Protocol::HostInitializedMessage& msg) {
        handleHostInitialized(msg);
    };

    protocol_.onHostDeactivated = [this](const Protocol::HostDeactivatedMessage& msg) {
        handleHostDeactivated(msg);
    };
}

void PluginLifecycleHandler::handleHostInitialized(const Protocol::HostInitializedMessage& msg) {
    LOGLN("[PluginLifecycle] Host initialized - showing splash");
    viewManager_.showSplash(1500, "Bitwig is starting");
    isHostActive_ = msg.isHostActive;
}

void PluginLifecycleHandler::handleHostDeactivated(const Protocol::HostDeactivatedMessage&) {
    isHostActive_ = false;
    viewManager_.showCore();
}
}