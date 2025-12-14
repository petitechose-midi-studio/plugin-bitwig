#include "HandlerHostPlugin.hpp"

#include <oc/teensy/LogOutput.hpp>
#include <oc/log/Log.hpp>

#include "protocol/struct/HostDeactivatedMessage.hpp"
#include "protocol/struct/HostInitializedMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;

HandlerHostPlugin::HandlerHostPlugin(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    OC_LOG_INFO("[HostPlugin] Setting up protocol callbacks...");
    setupProtocolCallbacks();
    OC_LOG_INFO("[HostPlugin] Callbacks registered");

    // Request host status on startup
    OC_LOG_INFO("[HostPlugin] Requesting host status...");
    protocol_.requestHostStatus();
}

void HandlerHostPlugin::setupProtocolCallbacks() {
    protocol_.onHostInitialized = [this](const HostInitializedMessage& msg) {
        OC_LOG_INFO("[HostPlugin] >> onHostInitialized: isActive={}", msg.isHostActive);

        if (state_.host.connected.get()) {
            OC_LOG_WARN("[HostPlugin] Already connected, ignoring duplicate");
            return;  // Already connected, ignore duplicate
        }

        OC_LOG_INFO("[HostPlugin] Setting host.connected = {}", msg.isHostActive);
        state_.host.connected.set(msg.isHostActive);

        // Request initial data from host
        if (msg.isHostActive) {
            OC_LOG_INFO("[HostPlugin] Requesting device list...");
            protocol_.requestDeviceList();
            OC_LOG_INFO("[HostPlugin] Requesting page names...");
            protocol_.requestPageNames();
            OC_LOG_INFO("[HostPlugin] Requesting track list...");
            protocol_.requestTrackList();
        }
        OC_LOG_INFO("[HostPlugin] << onHostInitialized done");
    };

    protocol_.onHostDeactivated = [this](const HostDeactivatedMessage&) {
        OC_LOG_INFO("[HostPlugin] >> onHostDeactivated");
        OC_LOG_INFO("[HostPlugin] Setting host.connected = false");
        state_.host.connected.set(false);

        // Reset all state on disconnect
        OC_LOG_INFO("[HostPlugin] Resetting all state...");
        state_.resetAll();
        OC_LOG_INFO("[HostPlugin] << onHostDeactivated done");
    };
}

}  // namespace bitwig::handler
