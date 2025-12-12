#include "PluginLifecycleHandler.hpp"

#include <oc/log/Log.hpp>

#include "protocol/struct/HostDeactivatedMessage.hpp"
#include "protocol/struct/HostInitializedMessage.hpp"
#include "protocol/struct/RequestHostStatusMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;

PluginLifecycleHandler::PluginLifecycleHandler(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();

    // Request host status on startup
    protocol_.requestHostStatus();
}

void PluginLifecycleHandler::setupProtocolCallbacks() {
    protocol_.onHostInitialized = [this](const HostInitializedMessage& msg) {
        if (state_.host.connected.get()) {
            return;  // Already connected, ignore duplicate
        }

        OC_LOG_INFO("Bitwig host initialized");
        state_.host.connected.set(msg.isHostActive);

        // Request initial data from host
        if (msg.isHostActive) {
            protocol_.requestDeviceList();
            protocol_.requestPageNames();
            protocol_.requestTrackList();
        }
    };

    protocol_.onHostDeactivated = [this](const HostDeactivatedMessage&) {
        OC_LOG_INFO("Bitwig host deactivated");
        state_.host.connected.set(false);

        // Reset all state on disconnect
        state_.resetAll();
    };
}

}  // namespace bitwig::handler
