#include "HandlerHostPlugin.hpp"

#include <oc/log/Log.hpp>

#include "protocol/struct/HostDeactivatedMessage.hpp"
#include "protocol/struct/HostInitializedMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;

HandlerHostPlugin::HandlerHostPlugin(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();
    protocol_.requestHostStatus();
}

void HandlerHostPlugin::setupProtocolCallbacks() {
    protocol_.onHostInitialized = [this](const HostInitializedMessage& msg) {
        if (state_.host.connected.get()) {
            OC_LOG_WARN("[HostPlugin] Already connected, ignoring duplicate");
            return;
        }

        OC_LOG_INFO("[HostPlugin] Host connected={}", msg.isHostActive);
        state_.host.connected.set(msg.isHostActive);

        if (msg.isHostActive) {
            // Reset all selector caches for fresh windowed loading
            state_.deviceSelector.names.clear();
            state_.deviceSelector.totalCount.set(0);
            state_.deviceSelector.loadedUpTo.set(0);

            state_.pageSelector.names.clear();
            state_.pageSelector.totalCount.set(0);
            state_.pageSelector.loadedUpTo.set(0);

            state_.trackSelector.names.clear();
            state_.trackSelector.totalCount.set(0);
            state_.trackSelector.loadedUpTo.set(0);

            // Request initial windows (uses windowed loading internally)
            protocol_.requestDeviceList();
            protocol_.requestPageNames();
            protocol_.requestTrackList();
        }
    };

    protocol_.onHostDeactivated = [this](const HostDeactivatedMessage&) {
        OC_LOG_INFO("[HostPlugin] Host disconnected");
        state_.host.connected.set(false);
        state_.resetAll();
    };
}

}  // namespace bitwig::handler
