#pragma once

/**
 * @file ViewManager.hpp
 * @brief Manages view switching with reactive signals
 *
 * ViewManager provides:
 * - Enum of available views (ViewType)
 * - Signal for view changes (reactive binding)
 * - Methods to switch, cycle through views
 *
 * Views register themselves and ViewManager handles activate/deactivate calls.
 */

#include <array>
#include <cstdint>

#include <oc/log/Log.hpp>
#include <oc/state/Signal.hpp>
#include <oc/ui/lvgl/IView.hpp>

namespace bitwig::state {

using oc::state::Signal;
using oc::ui::lvgl::IView;

/**
 * @brief Available view types in the application
 */
enum class ViewType : uint8_t {
    REMOTE_CONTROLS = 0,  // Device remote controls (8 parameters)
    MIX,                  // Mixer view (volume, pan, sends)
    CLIP,                 // Clip/Scene launcher
    COUNT                 // Sentinel for iteration
};

/**
 * @brief Get display name for a view type
 */
inline const char* viewTypeName(ViewType type) {
    switch (type) {
        case ViewType::REMOTE_CONTROLS: return "Remote Controls";
        case ViewType::MIX: return "Mix";
        case ViewType::CLIP: return "Clip";
        default: return "Unknown";
    }
}

/**
 * @brief Manages view lifecycle and transitions
 *
 * Usage:
 * 1. Register views with registerView()
 * 2. Subscribe to currentView signal for UI updates
 * 3. Call switchTo(), next(), previous() to change views
 */
class ViewManager {
public:
    static constexpr size_t VIEW_COUNT = static_cast<size_t>(ViewType::COUNT);

    ViewManager() = default;

    // Non-copyable, non-movable (holds view references)
    ViewManager(const ViewManager&) = delete;
    ViewManager& operator=(const ViewManager&) = delete;
    ViewManager(ViewManager&&) = delete;
    ViewManager& operator=(ViewManager&&) = delete;

    // =========================================================================
    // Registration
    // =========================================================================

    /**
     * @brief Register a view for a given type
     * @param type The view type
     * @param view Pointer to the view (must outlive ViewManager)
     */
    void registerView(ViewType type, IView* view) {
        auto idx = static_cast<size_t>(type);
        if (idx < VIEW_COUNT) {
            views_[idx] = view;
            OC_LOG_DEBUG("[ViewManager] Registered view '{}' for type {}",
                         view ? view->getViewId() : "null", static_cast<int>(type));
        }
    }

    // =========================================================================
    // Navigation
    // =========================================================================

    /**
     * @brief Switch to a specific view
     * @param type The view to switch to
     */
    void switchTo(ViewType type) {
        if (type == ViewType::COUNT) return;
        if (type == current_.get()) return;

        auto oldIdx = static_cast<size_t>(current_.get());
        auto newIdx = static_cast<size_t>(type);

        // Deactivate current view
        if (views_[oldIdx] != nullptr) {
            views_[oldIdx]->onDeactivate();
            OC_LOG_DEBUG("[ViewManager] Deactivated '{}'", views_[oldIdx]->getViewId());
        }

        // Activate new view
        if (views_[newIdx] != nullptr) {
            views_[newIdx]->onActivate();
            OC_LOG_DEBUG("[ViewManager] Activated '{}'", views_[newIdx]->getViewId());
        }

        current_.set(type);
        OC_LOG_INFO("[ViewManager] Switched to view: {}", viewTypeName(type));
    }

    /**
     * @brief Cycle to next view
     */
    void next() {
        auto idx = static_cast<size_t>(current_.get());
        auto nextIdx = (idx + 1) % VIEW_COUNT;
        switchTo(static_cast<ViewType>(nextIdx));
    }

    /**
     * @brief Cycle to previous view
     */
    void previous() {
        auto idx = static_cast<size_t>(current_.get());
        auto prevIdx = (idx + VIEW_COUNT - 1) % VIEW_COUNT;
        switchTo(static_cast<ViewType>(prevIdx));
    }

    // =========================================================================
    // Accessors
    // =========================================================================

    /**
     * @brief Get current view type (reactive signal)
     */
    Signal<ViewType>& currentView() { return current_; }
    const Signal<ViewType>& currentView() const { return current_; }

    /**
     * @brief Get current view type value
     */
    ViewType current() const { return current_.get(); }

    /**
     * @brief Get view pointer for a type
     */
    IView* getView(ViewType type) const {
        auto idx = static_cast<size_t>(type);
        return (idx < VIEW_COUNT) ? views_[idx] : nullptr;
    }

    /**
     * @brief Get current view pointer
     */
    IView* currentViewPtr() const {
        return getView(current_.get());
    }

    /**
     * @brief Initialize with first registered view active
     * Call after all views are registered
     */
    void initialize() {
        // Find first registered view and activate it
        for (size_t i = 0; i < VIEW_COUNT; ++i) {
            if (views_[i] != nullptr) {
                auto type = static_cast<ViewType>(i);
                views_[i]->onActivate();
                current_.set(type);
                OC_LOG_INFO("[ViewManager] Initialized with view: {}", viewTypeName(type));
                return;
            }
        }
        OC_LOG_WARN("[ViewManager] No views registered during initialization");
    }

    /**
     * @brief Reset all views (deactivate and clear)
     */
    void reset() {
        auto idx = static_cast<size_t>(current_.get());
        if (views_[idx] != nullptr) {
            views_[idx]->onDeactivate();
        }
        for (auto& view : views_) {
            view = nullptr;
        }
        current_.set(ViewType::REMOTE_CONTROLS);
    }

private:
    std::array<IView*, VIEW_COUNT> views_{};
    Signal<ViewType> current_{ViewType::REMOTE_CONTROLS};
};

}  // namespace bitwig::state
