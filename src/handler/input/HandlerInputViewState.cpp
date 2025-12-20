#include "HandlerInputViewState.hpp"

#include <oc/log/Log.hpp>

#include "protocol/struct/ViewStateChangeMessage.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;

HandlerInputViewState::HandlerInputViewState(state::BitwigState& state,
                                             BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupSubscriptions();

    // Send initial state
    sendViewState();
}

void HandlerInputViewState::setupSubscriptions() {
    // Watch view changes
    viewSub_ = state_.views.currentView().subscribe([this](ViewType) {
        sendViewState();
    });

    // Watch all selector visibility signals
    // Any change triggers a state update
    pageSelectorSub_ = state_.pageSelector.visible.subscribe([this](bool) {
        sendViewState();
    });

    deviceSelectorSub_ = state_.deviceSelector.visible.subscribe([this](bool) {
        sendViewState();
    });

    trackSelectorSub_ = state_.trackSelector.visible.subscribe([this](bool) {
        sendViewState();
    });

    viewSelectorSub_ = state_.viewSelector.visible.subscribe([this](bool) {
        sendViewState();
    });
}

void HandlerInputViewState::sendViewState() {
    ViewType currentView = state_.views.current();
    bool selectorActive = state_.overlays.hasVisibleOverlay();

    // Avoid duplicate sends
    if (currentView == lastViewType_ && selectorActive == lastSelectorActive_) {
        return;
    }

    lastViewType_ = currentView;
    lastSelectorActive_ = selectorActive;

    OC_LOG_DEBUG("[ViewState] Sending: view={}, selectorActive={}",
                 static_cast<int>(currentView), selectorActive);

    protocol_.send(ViewStateChangeMessage{
        static_cast<uint8_t>(currentView),
        selectorActive
    });
}

}  // namespace bitwig::handler
