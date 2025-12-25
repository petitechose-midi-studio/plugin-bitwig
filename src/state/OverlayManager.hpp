#pragma once

/**
 * @file OverlayManager.hpp
 * @brief Centralized overlay management with single-level stacking
 *
 * Manages overlay visibility with support for one stacked overlay.
 * When stacking, the previous overlay remains visible underneath.
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
    VIEW_SELECTOR,
    COUNT  // Must be last
};

/**
 * @brief Centralized manager for overlay visibility
 *
 * Supports one level of stacking (e.g., TrackSelector on top of DeviceSelector).
 * When stacking, both overlays are visible; the top one has input priority.
 *
 * Usage:
 * @code
 * overlays.show(OverlayType::DEVICE_SELECTOR);           // Shows device
 * overlays.show(OverlayType::TRACK_SELECTOR, true);      // Track on top, device stays visible
 * overlays.hide();                                        // Hides track, device is current
 * overlays.hide();                                        // Hides device
 * @endcode
 */
class OverlayManager {
public:
    OverlayManager() = default;

    // Non-copyable, non-movable (holds references)
    OverlayManager(const OverlayManager&) = delete;
    OverlayManager& operator=(const OverlayManager&) = delete;
    OverlayManager(OverlayManager&&) = delete;
    OverlayManager& operator=(OverlayManager&&) = delete;

    /**
     * @brief Register an overlay's visible signal
     */
    void registerOverlay(OverlayType type, Signal<bool>& visible) {
        auto idx = static_cast<size_t>(type);
        if (idx < static_cast<size_t>(OverlayType::COUNT)) {
            overlays_[idx] = &visible;
        }
    }

    /**
     * @brief Show an overlay
     * @param type The overlay to show
     * @param stack If true, current overlay stays visible underneath
     */
    void show(OverlayType type, bool stack = false) {
        if (type == OverlayType::NONE || type == OverlayType::COUNT) {
            hideAll();
            return;
        }

        if (stack && current_ != OverlayType::NONE && current_ != type) {
            // Stacking: current stays visible, becomes previous
            previous_ = current_;
        } else if (current_ != OverlayType::NONE) {
            // Replacing: hide current
            setVisible(current_, false);
            previous_ = OverlayType::NONE;
        }

        setVisible(type, true);
        current_ = type;
        OC_LOG_DEBUG("[OverlayManager] show: {} (stack: {})", static_cast<int>(type), stack);
    }

    /**
     * @brief Hide current overlay, restore previous if stacked
     */
    void hide() {
        if (current_ == OverlayType::NONE) return;

        setVisible(current_, false);
        OC_LOG_DEBUG("[OverlayManager] hide: {}", static_cast<int>(current_));

        // Restore previous (already visible if was stacked)
        current_ = previous_;
        previous_ = OverlayType::NONE;

        if (current_ != OverlayType::NONE) {
            OC_LOG_DEBUG("[OverlayManager] current now: {}", static_cast<int>(current_));
        }
    }

    /**
     * @brief Hide all overlays
     */
    void hideAll() {
        for (size_t i = 1; i < static_cast<size_t>(OverlayType::COUNT); ++i) {
            if (overlays_[i] != nullptr) {
                overlays_[i]->set(false);
            }
        }
        current_ = OverlayType::NONE;
        previous_ = OverlayType::NONE;
        OC_LOG_DEBUG("[OverlayManager] hideAll");
    }

    /// Get currently active overlay (top of stack)
    OverlayType current() const { return current_; }

    /// Check if any overlay is visible
    bool hasVisibleOverlay() const { return current_ != OverlayType::NONE; }

private:
    void setVisible(OverlayType type, bool visible) {
        auto idx = static_cast<size_t>(type);
        if (overlays_[idx] != nullptr) {
            overlays_[idx]->set(visible);
        }
    }

    std::array<Signal<bool>*, static_cast<size_t>(OverlayType::COUNT)> overlays_{};
    OverlayType current_ = OverlayType::NONE;
    OverlayType previous_ = OverlayType::NONE;  // For single-level stacking
};

}  // namespace bitwig::state
