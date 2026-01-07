#include "ViewStateInputHandler.hpp"

#include <oc/log/Log.hpp>

namespace bitwig::handler {

using namespace bitwig::state;

ViewStateInputHandler::ViewStateInputHandler(state::BitwigState& state,
                                             BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupSubscriptions();

    // Send initial state
    sendViewState();
}

void ViewStateInputHandler::setupSubscriptions() {
    // Watch view changes
    view_sub_ = state_.views.currentView().subscribe([this](ViewType) {
        sendViewState();
    });

    // Watch all selector visibility signals
    // Any change triggers a state update
    page_selector_sub_ = state_.pageSelector.visible.subscribe([this](bool) {
        sendViewState();
    });

    device_selector_sub_ = state_.deviceSelector.visible.subscribe([this](bool) {
        sendViewState();
    });

    track_selector_sub_ = state_.trackSelector.visible.subscribe([this](bool) {
        sendViewState();
    });

    view_selector_sub_ = state_.viewSelector.visible.subscribe([this](bool) {
        sendViewState();
    });
}

void ViewStateInputHandler::sendViewState() {
    ViewType currentView = state_.views.current();
    bool selectorActive = state_.overlays.hasVisible();

    // Avoid duplicate sends
    if (currentView == last_view_type_ && selectorActive == last_selector_active_) {
        return;
    }

    last_view_type_ = currentView;
    last_selector_active_ = selectorActive;

    OC_LOG_DEBUG("[ViewState] Sending: view={}, selectorActive={}",
                 static_cast<int>(currentView), selectorActive);

    protocol_.viewState(currentView, selectorActive);
}

}  // namespace bitwig::handler
