#pragma once

/**
 * @file OverlayController.hpp
 * @brief Unified overlay management with authority resolution and cleanup
 *
 * Wraps OverlayManager and adds:
 * - Automatic scope cleanup on hide
 * - Automatic latch release on hide
 * - Authority resolution for input priority
 *
 * @see INVARIANTS.md - Invariant 2: Input Authority
 * @see INVARIANTS.md - Invariant 4: Overlay Lifecycle
 */

#include <array>
#include <cstdint>
#include <functional>

#include <oc/core/input/AuthorityResolver.hpp>
#include <oc/log/Log.hpp>

#include "OverlayManager.hpp"

// Forward declaration to avoid circular dependency
namespace oc::api {
class ButtonAPI;
}

namespace bitwig::state {

using oc::core::ScopeID;
using oc::core::input::AuthorityResolver;
using oc::hal::ButtonID;

/**
 * @brief Extended registration info for an overlay
 */
struct OverlayCleanupInfo {
    ScopeID scopeId = 0;          ///< Scope for input bindings (0 = no scope cleanup)
    ButtonID latchButton = 0;     ///< Button that latches this overlay (0 = no latch)
};

/**
 * @brief Unified controller for overlay visibility, authority, and cleanup
 *
 * This class wraps OverlayManager and adds automatic cleanup on hide.
 * It enforces the overlay lifecycle invariants:
 * - Only one overlay has input authority at a time
 * - Closing an overlay clears its scope and releases latches
 * - Authority resolver is automatically updated
 *
 * Usage:
 * @code
 * // In BitwigContext::initialize()
 * overlayController_ = std::make_unique<OverlayController>(state_.overlays, buttons());
 *
 * // Register cleanup info for each overlay
 * overlayController_->registerCleanup(OverlayType::DEVICE_SELECTOR,
 *                                      deviceSelectorScope,
 *                                      ButtonID::LEFT_CENTER);
 *
 * // Use OverlayController instead of state_.overlays directly
 * overlayController_->show(OverlayType::DEVICE_SELECTOR);
 * overlayController_->hide();  // Automatic cleanup!
 * @endcode
 */
class OverlayController {
public:
    OverlayController(OverlayManager& manager, oc::api::ButtonAPI& buttons)
        : manager_(manager)
        , buttons_(&buttons) {
        // Configure authority resolver to query current overlay
        authority_.setOverlayProvider([this]() {
            return currentScope();
        });
    }

    ~OverlayController() = default;

    // Non-copyable, non-movable
    OverlayController(const OverlayController&) = delete;
    OverlayController& operator=(const OverlayController&) = delete;
    OverlayController(OverlayController&&) = delete;
    OverlayController& operator=(OverlayController&&) = delete;

    // =========================================================================
    // Registration
    // =========================================================================

    /**
     * @brief Register cleanup info for an overlay
     *
     * @param type The overlay type
     * @param scopeId The ScopeID used for input bindings (buttons_.clearScope on hide)
     * @param latchButton The button that latches this overlay (buttons_.clearLatch on hide)
     */
    void registerCleanup(OverlayType type, ScopeID scopeId, ButtonID latchButton = 0) {
        auto idx = static_cast<size_t>(type);
        if (idx < static_cast<size_t>(OverlayType::COUNT)) {
            cleanup_[idx] = {scopeId, latchButton};
            OC_LOG_DEBUG("[OverlayController] Registered cleanup for overlay {} (scope={}, latch={})",
                         idx, scopeId, static_cast<int>(latchButton));
        }
    }

    // =========================================================================
    // Show / Hide (delegates to OverlayManager + cleanup)
    // =========================================================================

    /**
     * @brief Show an overlay
     *
     * @param type The overlay to show
     * @param stack If true, current overlay stays visible underneath
     */
    void show(OverlayType type, bool stack = false) {
        manager_.show(type, stack);
    }

    /**
     * @brief Hide current overlay with automatic cleanup
     *
     * Performs the complete overlay lifecycle:
     * 1. Clears the overlay's latch (via ButtonAPI)
     * 2. Clears the overlay's scope (via ButtonAPI)
     * 3. Hides the overlay (via OverlayManager)
     */
    void hide();

    /**
     * @brief Hide all overlays with cleanup
     */
    void hideAll();

    // =========================================================================
    // State Query (delegates to OverlayManager)
    // =========================================================================

    /// Get currently active overlay (top of stack)
    OverlayType current() const { return manager_.current(); }

    /// Get the scope of the current overlay (0 if none)
    ScopeID currentScope() const {
        auto type = manager_.current();
        if (type == OverlayType::NONE) return 0;
        return cleanup_[static_cast<size_t>(type)].scopeId;
    }

    /// Check if any overlay is visible
    bool hasVisibleOverlay() const { return manager_.hasVisibleOverlay(); }

    /// Check if a specific overlay is the current one
    bool isCurrent(OverlayType type) const { return manager_.current() == type; }

    // =========================================================================
    // Authority Resolver Access
    // =========================================================================

    /**
     * @brief Get the authority resolver
     *
     * Use this to configure additional authority providers (active view, custom layers).
     */
    AuthorityResolver& authority() { return authority_; }
    const AuthorityResolver& authority() const { return authority_; }

    /**
     * @brief Check if a scope currently has input authority
     */
    bool hasAuthority(ScopeID scope) const {
        return authority_.hasAuthority(scope);
    }

    // =========================================================================
    // Direct Scope Access
    // =========================================================================

    /**
     * @brief Get the scope for an overlay type
     */
    ScopeID getScopeFor(OverlayType type) const {
        auto idx = static_cast<size_t>(type);
        if (idx < static_cast<size_t>(OverlayType::COUNT)) {
            return cleanup_[idx].scopeId;
        }
        return 0;
    }

    /**
     * @brief Manually trigger cleanup for an overlay (without hiding)
     */
    void cleanupFor(OverlayType type);

private:
    void doCleanup(OverlayType type);

    OverlayManager& manager_;
    oc::api::ButtonAPI* buttons_;
    AuthorityResolver authority_;

    std::array<OverlayCleanupInfo, static_cast<size_t>(OverlayType::COUNT)> cleanup_{};
};

}  // namespace bitwig::state
