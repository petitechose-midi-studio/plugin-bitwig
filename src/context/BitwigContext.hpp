#pragma once

/**
 * @file BitwigContext.hpp
 * @brief Main context for Bitwig DAW integration
 *
 * BitwigContext manages:
 * - BitwigState: reactive state for all Bitwig-related data
 * - BitwigProtocol: SysEx communication with Bitwig
 * - Input bindings: encoder/button mappings to protocol actions
 * - Host connection: detect connect/disconnect from Bitwig
 *
 * ## Architecture
 *
 * ```
 * BitwigContext
 *     ├── BitwigState (reactive state)
 *     ├── BitwigProtocol (SysEx transport)
 *     ├── Views (subscribe to state)
 *     └── Input bindings (update state/send protocol)
 * ```
 */

#include <memory>

#include <lvgl.h>

#include <oc/context/IContext.hpp>
#include <oc/ui/lvgl/IView.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig {

/**
 * @brief Context for Bitwig DAW control
 *
 * Implements IContext lifecycle with DAW connection detection.
 * Owns BitwigState and BitwigProtocol instances.
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
    // State Access (for handlers and views)
    // =========================================================================

    state::BitwigState& state() { return state_; }
    const state::BitwigState& state() const { return state_; }

    BitwigProtocol& protocol() { return *protocol_; }
    const BitwigProtocol& protocol() const { return *protocol_; }

private:
    // =========================================================================
    // Setup Methods
    // =========================================================================

    void setupProtocol();
    void setupProtocolCallbacks();
    void setupInputBindings();
    void setupView();

    // =========================================================================
    // Protocol Callback Handlers
    // =========================================================================

    void handleHostInitialized();
    void handleHostDeactivated();
    void handleTransportTempo(float tempo);
    void handleDeviceChange(const std::string& name, uint8_t type, bool enabled);
    void handleParameterUpdate(uint8_t index, float value, const std::string& displayValue);
    void handleParameterName(uint8_t index, const std::string& name);

    // =========================================================================
    // Input Handlers
    // =========================================================================

    void onEncoderTurn(uint8_t index, float value);
    void onEncoderPress(uint8_t index);
    void onPlayPressed();
    void onRecordPressed();
    void onStopPressed();

    // =========================================================================
    // Members
    // =========================================================================

    state::BitwigState state_;
    std::unique_ptr<BitwigProtocol> protocol_;
    std::unique_ptr<oc::ui::lvgl::IView> view_;

    // Host connection timeout
    uint32_t lastHostPing_ = 0;
    static constexpr uint32_t HOST_TIMEOUT_MS = 5000;
};

}  // namespace bitwig
