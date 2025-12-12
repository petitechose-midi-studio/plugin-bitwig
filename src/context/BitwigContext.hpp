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
 *     ├── Handlers (update state from protocol/input)
 *     │   ├── DeviceHostHandler (protocol → state)
 *     │   ├── TransportHostHandler (protocol → state)
 *     │   ├── MacroInputHandler (input → protocol)
 *     │   └── ...
 *     └── Views (subscribe to state)
 *         └── DeviceView, TransportBar, etc.
 * ```
 *
 * The context itself is thin - handlers and views do the work.
 */

#include <memory>

#include <lvgl.h>

#include <oc/context/IContext.hpp>
#include <oc/ui/lvgl/IView.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

// Forward declarations for handlers (will be adapted)
namespace bitwig::handler {
class HostHandler;
class InputHandler;
}

namespace bitwig {

/**
 * @brief Context for Bitwig DAW control
 *
 * Implements IContext lifecycle with DAW connection detection.
 * Owns BitwigState, BitwigProtocol, and coordinates handlers/views.
 */
class BitwigContext : public oc::context::IContext {
public:
    BitwigContext() = default;
    ~BitwigContext() override = default;

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

    bool isConnected() const override { return state_.host.connected.get(); }
    void onConnected() override;
    void onDisconnected() override;

    // =========================================================================
    // Accessors (for handlers)
    // =========================================================================

    state::BitwigState& state() { return state_; }
    const state::BitwigState& state() const { return state_; }

    BitwigProtocol& protocol() { return *protocol_; }
    const BitwigProtocol& protocol() const { return *protocol_; }

private:
    void createProtocol();
    void createHandlers();
    void createViews();

    // =========================================================================
    // Members
    // =========================================================================

    state::BitwigState state_;
    std::unique_ptr<BitwigProtocol> protocol_;

    // Handlers (will be populated when adapted)
    std::unique_ptr<handler::HostHandler> hostHandler_;
    std::unique_ptr<handler::InputHandler> inputHandler_;

    // Views
    std::unique_ptr<oc::ui::lvgl::IView> view_;
};

}  // namespace bitwig
