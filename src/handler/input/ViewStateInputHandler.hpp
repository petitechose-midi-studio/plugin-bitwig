#pragma once

/**
 * @file ViewStateInputHandler.hpp
 * @brief Sends VIEW_STATE_CHANGE to host when view or overlay state changes
 *
 * InputHandler pattern:
 * - Watches ViewManager.currentView() signal
 * - Watches overlay visibility (ExclusiveVisibilityStack)
 * - Sends ViewStateChangeMessage when either changes
 *
 * The host uses this to control modulated values batch:
 * - Only sends batch when RemoteControls view is active AND no selector open
 */

#include <oc/state/Signal.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "protocol/ViewType.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief View state change notifier (Controller -> Host)
 *
 * Informs host about active view and selector state for bandwidth management.
 */
class ViewStateInputHandler {
public:
    ViewStateInputHandler(state::BitwigState& state, BitwigProtocol& protocol);
    ~ViewStateInputHandler() = default;

    // Non-copyable
    ViewStateInputHandler(const ViewStateInputHandler&) = delete;
    ViewStateInputHandler& operator=(const ViewStateInputHandler&) = delete;

private:
    void setupSubscriptions();
    void sendViewState();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;

    // Track current state to avoid duplicate sends
    ViewType last_view_type_ = ViewType::REMOTE_CONTROLS;
    bool last_selector_active_ = false;

    // Subscriptions
    oc::state::Subscription view_sub_;
    oc::state::Subscription page_selector_sub_;
    oc::state::Subscription device_selector_sub_;
    oc::state::Subscription track_selector_sub_;
    oc::state::Subscription view_selector_sub_;
};

}  // namespace bitwig::handler
