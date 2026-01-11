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
 *     │   ├── PluginHostHandler
 *     │   ├── TransportHostHandler
 *     │   ├── DeviceHostHandler
 *     │   ├── TrackHostHandler
 *     │   ├── PageHostHandler
 *     │   ├── RemoteControlHostHandler
 *     │   ├── LastClickedHostHandler
 *     │   └── MidiHostHandler
 *     ├── InputHandlers (input → state + protocol)
 *     │   ├── TransportInputHandler
 *     │   ├── ViewSwitcherInputHandler (LEFT_TOP → cycle views)
 *     │   ├── RemoteControlInputHandler
 *     │   ├── DevicePageInputHandler
 *     │   ├── DeviceSelectorInputHandler
 *     │   ├── TrackInputHandler
 *     │   └── LastClickedInputHandler
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
#include <oc/context/Requirements.hpp>
#include <oc/state/Signal.hpp>
#include <oc/ui/lvgl/IView.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"
#include <state/OverlayManager.hpp>

// Include all handlers (required for unique_ptr with make_unique in templates)
#include "handler/host/DeviceHostHandler.hpp"
#include "handler/host/LastClickedHostHandler.hpp"
#include "handler/host/RemoteControlHostHandler.hpp"
#include "handler/host/MidiHostHandler.hpp"
#include "handler/host/PageHostHandler.hpp"
#include "handler/host/PluginHostHandler.hpp"
#include "handler/host/TrackHostHandler.hpp"
#include "handler/host/TransportHostHandler.hpp"
#include "handler/input/DevicePageInputHandler.hpp"
#include "handler/input/DeviceSelectorInputHandler.hpp"
#include "handler/input/LastClickedInputHandler.hpp"
#include "handler/input/RemoteControlInputHandler.hpp"
#include "handler/input/TrackInputHandler.hpp"
#include "handler/input/TransportInputHandler.hpp"
#include "handler/input/ViewStateInputHandler.hpp"
#include "handler/input/ViewSwitcherInputHandler.hpp"
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
    static constexpr oc::context::Requirements REQUIRES{
        .button = true,
        .encoder = true,
        .midi = true
    };

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

    core::state::OverlayManager<bitwig::ui::OverlayType>& overlays() { return *overlay_controller_; }
    const core::state::OverlayManager<bitwig::ui::OverlayType>& overlays() const { return *overlay_controller_; }

private:
    void createProtocol();
    void createHostHandlers();
    void createInputHandlers();
    void createViews();
    void createOverlayManager();

    // =========================================================================
    // Members
    // =========================================================================

    state::BitwigState state_;
    std::unique_ptr<BitwigProtocol> protocol_;
    std::unique_ptr<core::state::OverlayManager<bitwig::ui::OverlayType>> overlay_controller_;

    // Host Handlers (protocol → state)
    std::unique_ptr<handler::PluginHostHandler> host_plugin_;
    std::unique_ptr<handler::TransportHostHandler> host_transport_;
    std::unique_ptr<handler::DeviceHostHandler> host_device_;
    std::unique_ptr<handler::TrackHostHandler> host_track_;
    std::unique_ptr<handler::PageHostHandler> host_page_;
    std::unique_ptr<handler::RemoteControlHostHandler> host_remote_control_;
    std::unique_ptr<handler::LastClickedHostHandler> host_last_clicked_;
    std::unique_ptr<handler::MidiHostHandler> host_midi_;

    // Input Handlers (input → state + protocol)
    std::unique_ptr<handler::TransportInputHandler> input_transport_;
    std::unique_ptr<handler::ViewSwitcherInputHandler> input_view_switcher_;
    std::unique_ptr<handler::RemoteControlInputHandler> input_remote_control_;
    std::unique_ptr<handler::DevicePageInputHandler> input_device_page_;
    std::unique_ptr<handler::DeviceSelectorInputHandler> input_device_selector_;
    std::unique_ptr<handler::TrackInputHandler> input_track_;
    std::unique_ptr<handler::LastClickedInputHandler> input_last_clicked_;
    std::unique_ptr<handler::ViewStateInputHandler> input_view_state_;

    // UI Container
    std::unique_ptr<core::ui::ViewContainer> view_container_;

    // Views (managed by ViewManager)
    std::unique_ptr<ui::RemoteControlsView> remote_controls_view_;
    std::unique_ptr<ui::MixView> mix_view_;
    std::unique_ptr<ui::ClipView> clip_view_;

    // Persistent UI (always visible)
    std::unique_ptr<ui::TransportBar> transport_bar_;

    // Global overlays
    std::unique_ptr<ui::ViewSelector> view_selector_;
    std::vector<oc::state::Subscription> view_selector_subs_;
};

}  // namespace bitwig
