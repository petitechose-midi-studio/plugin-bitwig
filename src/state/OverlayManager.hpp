#pragma once

/**
 * @file OverlayManager.hpp
 * @brief Centralized overlay management with exclusive visibility and stack restoration
 *
 * Design rationale:
 * - Only one overlay visible at a time (exclusive)
 * - Supports stacking: opening TrackSelector on DeviceSelector remembers to restore
 * - Clean API: show(type), hide(), hideAll()
 * - Works with existing Signal<bool> visible fields
 */

#include <oc/log/Log.hpp>
#include <oc/state/Signal.hpp>

#include <array>
#include <cstdint>

namespace bitwig::state {

using oc::state::Signal;

/**
 * @brief Overlay types managed by OverlayManager
 */
enum class OverlayType : uint8_t {
    NONE = 0,
    PAGE_SELECTOR,
    DEVICE_SELECTOR,
    TRACK_SELECTOR,
    COUNT  // Must be last
};

/**
 * @brief Centralized manager for overlay visibility
 *
 * Ensures only one overlay is visible at a time and supports
 * restoration when closing stacked overlays.
 *
 * Usage:
 * @code
 * // In BitwigState or handler initialization
 * overlays.registerOverlay(OverlayType::PAGE_SELECTOR, pageSelector.visible);
 * overlays.registerOverlay(OverlayType::DEVICE_SELECTOR, deviceSelector.visible);
 * overlays.registerOverlay(OverlayType::TRACK_SELECTOR, trackSelector.visible);
 *
 * // In handlers
 * overlays.show(OverlayType::DEVICE_SELECTOR);  // Hides all others, shows device
 * overlays.show(OverlayType::TRACK_SELECTOR);   // Remembers device was open, shows track
 * overlays.hide();                               // Restores device selector
 * overlays.hide();                               // Hides all
 * @endcode
 */
class OverlayManager {
public:
    static constexpr size_t MAX_STACK_DEPTH = 4;

    OverlayManager() = default;

    // Non-copyable, non-movable (holds references)
    OverlayManager(const OverlayManager&) = delete;
    OverlayManager& operator=(const OverlayManager&) = delete;
    OverlayManager(OverlayManager&&) = delete;
    OverlayManager& operator=(OverlayManager&&) = delete;

    /**
     * @brief Register an overlay's visible signal
     * @param type The overlay type
     * @param visible Reference to the Signal<bool> controlling visibility
     */
    void registerOverlay(OverlayType type, Signal<bool>& visible) {
        auto idx = static_cast<size_t>(type);
        if (idx < static_cast<size_t>(OverlayType::COUNT)) {
            overlays_[idx] = &visible;
        }
    }

    /**
     * @brief Show an overlay (hides all others, supports stack restoration)
     * @param type The overlay to show
     * @param pushToStack If true, current overlay is pushed for later restoration
     */
    void show(OverlayType type, bool pushToStack = true) {
        if (type == OverlayType::NONE || type == OverlayType::COUNT) {
            hideAll();
            return;
        }

        // Push current to stack if requested and something is visible
        if (pushToStack && current_ != OverlayType::NONE && current_ != type) {
            pushStack(current_);
        }

        // Hide all overlays
        hideAllInternal();

        // Show requested overlay
        auto idx = static_cast<size_t>(type);
        if (overlays_[idx] != nullptr) {
            overlays_[idx]->set(true);
            current_ = type;
            OC_LOG_DEBUG("[OverlayManager] show: {}", static_cast<int>(type));
        }
    }

    /**
     * @brief Hide current overlay and restore previous from stack
     */
    void hide() {
        if (current_ == OverlayType::NONE) return;

        // Hide current
        auto idx = static_cast<size_t>(current_);
        if (overlays_[idx] != nullptr) {
            overlays_[idx]->set(false);
        }

        OC_LOG_DEBUG("[OverlayManager] hide: {}", static_cast<int>(current_));

        // Restore from stack or set to none
        OverlayType restored = popStack();
        if (restored != OverlayType::NONE) {
            auto restoreIdx = static_cast<size_t>(restored);
            if (overlays_[restoreIdx] != nullptr) {
                overlays_[restoreIdx]->set(true);
                OC_LOG_DEBUG("[OverlayManager] restored: {}", static_cast<int>(restored));
            }
        }
        current_ = restored;
    }

    /**
     * @brief Hide all overlays and clear stack
     */
    void hideAll() {
        hideAllInternal();
        clearStack();
        current_ = OverlayType::NONE;
        OC_LOG_DEBUG("[OverlayManager] hideAll");
    }

    /**
     * @brief Get currently visible overlay
     */
    OverlayType current() const { return current_; }

    /**
     * @brief Check if a specific overlay is visible
     */
    bool isVisible(OverlayType type) const { return current_ == type; }

    /**
     * @brief Check if any overlay is visible
     */
    bool hasVisibleOverlay() const { return current_ != OverlayType::NONE; }

private:
    void hideAllInternal() {
        for (size_t i = 0; i < static_cast<size_t>(OverlayType::COUNT); ++i) {
            if (overlays_[i] != nullptr) {
                overlays_[i]->set(false);
            }
        }
    }

    void pushStack(OverlayType type) {
        if (stackTop_ < MAX_STACK_DEPTH) {
            stack_[stackTop_++] = type;
        }
    }

    OverlayType popStack() {
        if (stackTop_ > 0) {
            return stack_[--stackTop_];
        }
        return OverlayType::NONE;
    }

    void clearStack() {
        stackTop_ = 0;
    }

    std::array<Signal<bool>*, static_cast<size_t>(OverlayType::COUNT)> overlays_{};
    std::array<OverlayType, MAX_STACK_DEPTH> stack_{};
    size_t stackTop_ = 0;
    OverlayType current_ = OverlayType::NONE;
};

}  // namespace bitwig::state
