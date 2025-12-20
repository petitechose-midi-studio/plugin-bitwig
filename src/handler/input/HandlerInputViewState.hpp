#pragma once

/**
 * @file HandlerInputViewState.hpp
 * @brief Sends VIEW_STATE_CHANGE to host when view or overlay state changes
 *
 * InputHandler pattern:
 * - Watches ViewManager.currentView() signal
 * - Watches overlay visibility (OverlayManager)
 * - Sends ViewStateChangeMessage when either changes
 *
 * The host uses this to control modulated values batch:
 * - Only sends batch when RemoteControls view is active AND no selector open
 */

#include <oc/state/Signal.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief View state change notifier (Controller -> Host)
 *
 * Informs host about active view and selector state for bandwidth management.
 */
class HandlerInputViewState {
public:
    HandlerInputViewState(state::BitwigState& state, BitwigProtocol& protocol);
    ~HandlerInputViewState() = default;

    // Non-copyable
    HandlerInputViewState(const HandlerInputViewState&) = delete;
    HandlerInputViewState& operator=(const HandlerInputViewState&) = delete;

private:
    void setupSubscriptions();
    void sendViewState();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;

    // Track current state to avoid duplicate sends
    state::ViewType lastViewType_ = state::ViewType::REMOTE_CONTROLS;
    bool lastSelectorActive_ = false;

    // Subscriptions
    oc::state::Subscription viewSub_;
    oc::state::Subscription pageSelectorSub_;
    oc::state::Subscription deviceSelectorSub_;
    oc::state::Subscription trackSelectorSub_;
    oc::state::Subscription viewSelectorSub_;
};

}  // namespace bitwig::handler
