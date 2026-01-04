#pragma once

/**
 * @file BitwigContext.hpp
 * @brief Main context for Bitwig DAW integration
 *
 * BitwigContext manages the lifecycle and wires together:
 * - BitwigState: reactive state for all Bitwig-related data
 * - BitwigProtocol: SysEx communication with Bitwig
 * - Handlers: protocol callbacks and input bindings
 * - Views: UI components that subscribe to state
 *
 * ## Architecture
 *
 * ```
 * BitwigContext
 *     ├── BitwigState (reactive state - single source of truth)
 *     ├── BitwigProtocol (SysEx transport)
 *     ├── HostHandlers (protocol → state)
 *     │   ├── HandlerHostPlugin
 *     │   ├── HandlerHostTransport
 *     │   ├── HandlerHostDevice
 *     │   ├── HandlerHostTrack
 *     │   ├── HandlerHostPage
 *     │   ├── HandlerHostRemoteControl
 *     │   ├── HandlerHostLastClicked
 *     │   └── HandlerHostMidi
 *     ├── InputHandlers (input → state + protocol)
 *     │   ├── HandlerInputTransport
 *     │   ├── HandlerInputViewSwitcher (LEFT_TOP → cycle views)
 *     │   ├── HandlerInputRemoteControl
 *     │   ├── HandlerInputDevicePage
 *     │   ├── HandlerInputDeviceSelector
 *     │   ├── HandlerInputTrack
 *     │   └── HandlerInputLastClicked
 *     └── Views (managed by ViewManager)
 *         ├── RemoteControlsView (device parameters)
 *         ├── MixView (volume, pan, sends) - placeholder
 *         ├── ClipView (clip launcher) - placeholder
 *         └── TransportBar (persistent)
 * ```
 *
 * The context itself is thin - handlers and views do the work.
 */

#include <memory>
#include <vector>

#include <lvgl.h>

#include <oc/context/IContext.hpp>
#include <oc/state/Signal.hpp>
#include <oc/ui/lvgl/IView.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"
#include <state/OverlayController.hpp>

// Include all handlers (required for unique_ptr with make_unique in templates)
#include "handler/host/HandlerHostDevice.hpp"
#include "handler/host/HandlerHostLastClicked.hpp"
#include "handler/host/HandlerHostRemoteControl.hpp"
#include "handler/host/HandlerHostMidi.hpp"
#include "handler/host/HandlerHostPage.hpp"
#include "handler/host/HandlerHostPlugin.hpp"
#include "handler/host/HandlerHostTrack.hpp"
#include "handler/host/HandlerHostTransport.hpp"
#include "handler/input/HandlerInputDevicePage.hpp"
#include "handler/input/HandlerInputDeviceSelector.hpp"
#include "handler/input/HandlerInputLastClicked.hpp"
#include "handler/input/HandlerInputRemoteControl.hpp"
#include "handler/input/HandlerInputTrack.hpp"
#include "handler/input/HandlerInputTransport.hpp"
#include "handler/input/HandlerInputViewState.hpp"
#include "handler/input/HandlerInputViewSwitcher.hpp"
#include "ui/clip/ClipView.hpp"
#include "ui/mix/MixView.hpp"
#include "ui/remotecontrols/RemoteControlsView.hpp"
#include "ui/transportbar/TransportBar.hpp"
#include "ui/view/ViewSelector.hpp"
#include <ui/ViewContainer.hpp>

namespace bitwig {

/**
 * @brief Context for Bitwig DAW control
 *
 * Implements IContext lifecycle with DAW connection detection.
 * Owns BitwigState, BitwigProtocol, and coordinates handlers/views.
 */
class BitwigContext : public oc::context::IContext {
public:
    // Static resource loading (called by ContextManager during registration)
    static void loadResources();

    BitwigContext() = default;
    ~BitwigContext() override;

    // Non-copyable, non-movable
    BitwigContext(const BitwigContext&) = delete;
    BitwigContext& operator=(const BitwigContext&) = delete;
    BitwigContext(BitwigContext&&) = delete;
    BitwigContext& operator=(BitwigContext&&) = delete;

    // =========================================================================
    // IContext Lifecycle
    // =========================================================================

    bool initialize() override;
    void update() override;
    void cleanup() override;
    const char* getName() const override { return "Bitwig"; }

    // =========================================================================
    // DAW Connection
    // =========================================================================

    // Always return true - BitwigContext is the default context.
    // The framework's disconnect logic is for switching away from DAW contexts,
    // but since this is the only context, we never want to "disconnect".
    // Host connection state is tracked separately in state_.host.connected.
    bool isConnected() const override { return true; }
    void onConnected() override;
    void onDisconnected() override;

    // =========================================================================
    // Accessors (for testing/debugging)
    // =========================================================================

    state::BitwigState& state() { return state_; }
    const state::BitwigState& state() const { return state_; }

    BitwigProtocol& protocol() { return *protocol_; }
    const BitwigProtocol& protocol() const { return *protocol_; }

    ::state::OverlayController<bitwig::state::OverlayType>& overlays() { return *overlayController_; }
    const ::state::OverlayController<bitwig::state::OverlayType>& overlays() const { return *overlayController_; }

private:
    void createProtocol();
    void createHostHandlers();
    void createInputHandlers();
    void createViews();
    void createOverlayController();

    // =========================================================================
    // Members
    // =========================================================================

    state::BitwigState state_;
    std::unique_ptr<BitwigProtocol> protocol_;
    std::unique_ptr<::state::OverlayController<bitwig::state::OverlayType>> overlayController_;

    // Host Handlers (protocol → state)
    std::unique_ptr<handler::HandlerHostPlugin> hostPlugin_;
    std::unique_ptr<handler::HandlerHostTransport> hostTransport_;
    std::unique_ptr<handler::HandlerHostDevice> hostDevice_;
    std::unique_ptr<handler::HandlerHostTrack> hostTrack_;
    std::unique_ptr<handler::HandlerHostPage> hostPage_;
    std::unique_ptr<handler::HandlerHostRemoteControl> hostRemoteControl_;
    std::unique_ptr<handler::HandlerHostLastClicked> hostLastClicked_;
    std::unique_ptr<handler::HandlerHostMidi> hostMidi_;

    // Input Handlers (input → state + protocol)
    std::unique_ptr<handler::HandlerInputTransport> inputTransport_;
    std::unique_ptr<handler::HandlerInputViewSwitcher> inputViewSwitcher_;
    std::unique_ptr<handler::HandlerInputRemoteControl> inputRemoteControl_;
    std::unique_ptr<handler::HandlerInputDevicePage> inputDevicePage_;
    std::unique_ptr<handler::HandlerInputDeviceSelector> inputDeviceSelector_;
    std::unique_ptr<handler::HandlerInputTrack> inputTrack_;
    std::unique_ptr<handler::HandlerInputLastClicked> inputLastClicked_;
    std::unique_ptr<handler::HandlerInputViewState> inputViewState_;

    // UI Container
    std::unique_ptr<ui::ViewContainer> viewContainer_;

    // Views (managed by ViewManager)
    std::unique_ptr<RemoteControlsView> remoteControlsView_;
    std::unique_ptr<MixView> mixView_;
    std::unique_ptr<ClipView> clipView_;

    // Persistent UI (always visible)
    std::unique_ptr<TransportBar> transportBar_;

    // Global overlays
    std::unique_ptr<ViewSelector> viewSelector_;
    std::vector<oc::state::Subscription> viewSelectorSubs_;
};

}  // namespace bitwig
