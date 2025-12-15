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
 *     │   ├── HandlerHostMacro
 *     │   ├── HandlerHostLastClicked
 *     │   └── HandlerHostMidi
 *     ├── InputHandlers (input → state + protocol)
 *     │   ├── HandlerInputTransport
 *     │   ├── HandlerInputMacro
 *     │   ├── HandlerInputDevicePage
 *     │   ├── HandlerInputDeviceSelector
 *     │   ├── HandlerInputTrack
 *     │   └── HandlerInputLastClicked
 *     └── Views (subscribe to state)
 *         └── DeviceView, TransportBar, etc.
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

// Include all handlers (required for unique_ptr with make_unique in templates)
#include "handler/host/HandlerHostDevice.hpp"
#include "handler/host/HandlerHostLastClicked.hpp"
#include "handler/host/HandlerHostMacro.hpp"
#include "handler/host/HandlerHostMidi.hpp"
#include "handler/host/HandlerHostPage.hpp"
#include "handler/host/HandlerHostPlugin.hpp"
#include "handler/host/HandlerHostTrack.hpp"
#include "handler/host/HandlerHostTransport.hpp"
#include "handler/input/HandlerInputDevicePage.hpp"
#include "handler/input/HandlerInputDeviceSelector.hpp"
#include "handler/input/HandlerInputLastClicked.hpp"
#include "handler/input/HandlerInputMacro.hpp"
#include "handler/input/HandlerInputTrack.hpp"
#include "handler/input/HandlerInputTransport.hpp"
#include "ui/device/DeviceView.hpp"
#include "ui/transportbar/TransportBar.hpp"
#include "ui/ViewContainer.hpp"

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

private:
    void createProtocol();
    void createHostHandlers();
    void createInputHandlers();
    void createViews();

    // =========================================================================
    // Members
    // =========================================================================

    state::BitwigState state_;
    std::unique_ptr<BitwigProtocol> protocol_;

    // Host Handlers (protocol → state)
    std::unique_ptr<handler::HandlerHostPlugin> hostPlugin_;
    std::unique_ptr<handler::HandlerHostTransport> hostTransport_;
    std::unique_ptr<handler::HandlerHostDevice> hostDevice_;
    std::unique_ptr<handler::HandlerHostTrack> hostTrack_;
    std::unique_ptr<handler::HandlerHostPage> hostPage_;
    std::unique_ptr<handler::HandlerHostMacro> hostMacro_;
    std::unique_ptr<handler::HandlerHostLastClicked> hostLastClicked_;
    std::unique_ptr<handler::HandlerHostMidi> hostMidi_;

    // Input Handlers (input → state + protocol)
    std::unique_ptr<handler::HandlerInputTransport> inputTransport_;
    std::unique_ptr<handler::HandlerInputMacro> inputMacro_;
    std::unique_ptr<handler::HandlerInputDevicePage> inputDevicePage_;
    std::unique_ptr<handler::HandlerInputDeviceSelector> inputDeviceSelector_;
    std::unique_ptr<handler::HandlerInputTrack> inputTrack_;
    std::unique_ptr<handler::HandlerInputLastClicked> inputLastClicked_;

    // UI Container
    std::unique_ptr<ViewContainer> viewContainer_;

    // Views (subscribe to state in their constructors)
    std::unique_ptr<DeviceView> deviceView_;
    std::unique_ptr<TransportBar> transportBar_;
};

}  // namespace bitwig
