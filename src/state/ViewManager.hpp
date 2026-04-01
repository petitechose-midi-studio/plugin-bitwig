#pragma once

/**
 * @file ViewManager.hpp
 * @brief Template view manager with reactive signals for Bitwig firmware views
 *
 * Manages mutually exclusive top-level Bitwig views.
 *
 * @tparam EnumT View enum type (must have COUNT member)
 * @tparam DefaultValue Default enum value for initialization
 */

#include <array>

#include <oc/log/Log.hpp>
#include <oc/state/Signal.hpp>
#include <oc/ui/lvgl/IView.hpp>

namespace bitwig::state {

using oc::state::Signal;
using oc::ui::lvgl::IView;

template <typename EnumT, EnumT DefaultValue = static_cast<EnumT>(0)>
class ViewManager {
    static constexpr size_t COUNT = static_cast<size_t>(EnumT::COUNT);

public:
    ViewManager() = default;

    ViewManager(const ViewManager&) = delete;
    ViewManager& operator=(const ViewManager&) = delete;
    ViewManager(ViewManager&&) = delete;
    ViewManager& operator=(ViewManager&&) = delete;

    void registerView(EnumT type, IView* view) {
        const auto idx = static_cast<size_t>(type);
        if (idx >= COUNT) return;

        views_[idx] = view;
        OC_LOG_DEBUG("[ViewManager] Registered view '{}' for type {}",
                     view ? view->getViewId() : "null",
                     static_cast<int>(type));
    }

    void switchTo(EnumT type) {
        if (static_cast<size_t>(type) >= COUNT) return;
        if (type == current_.get()) return;

        const auto oldIdx = static_cast<size_t>(current_.get());
        const auto newIdx = static_cast<size_t>(type);

        if (views_[oldIdx] != nullptr) {
            views_[oldIdx]->onDeactivate();
            OC_LOG_DEBUG("[ViewManager] Deactivated '{}'", views_[oldIdx]->getViewId());
        }

        if (views_[newIdx] != nullptr) {
            views_[newIdx]->onActivate();
            OC_LOG_DEBUG("[ViewManager] Activated '{}'", views_[newIdx]->getViewId());
        }

        current_.set(type);
        OC_LOG_INFO("[ViewManager] Switched to view type={}", static_cast<int>(type));
    }

    void next() {
        const auto idx = static_cast<size_t>(current_.get());
        const auto nextIdx = (idx + 1) % COUNT;
        switchTo(static_cast<EnumT>(nextIdx));
    }

    void previous() {
        const auto idx = static_cast<size_t>(current_.get());
        const auto prevIdx = (idx + COUNT - 1) % COUNT;
        switchTo(static_cast<EnumT>(prevIdx));
    }

    Signal<EnumT>& currentView() { return current_; }
    const Signal<EnumT>& currentView() const { return current_; }

    EnumT current() const { return current_.get(); }

    IView* getView(EnumT type) const {
        const auto idx = static_cast<size_t>(type);
        return (idx < COUNT) ? views_[idx] : nullptr;
    }

    IView* currentViewPtr() const {
        return getView(current_.get());
    }

    void initialize() {
        for (size_t i = 0; i < COUNT; ++i) {
            if (views_[i] == nullptr) continue;

            const auto type = static_cast<EnumT>(i);
            views_[i]->onActivate();
            current_.set(type);
            OC_LOG_INFO("[ViewManager] Initialized with view type={}", static_cast<int>(type));
            return;
        }

        OC_LOG_WARN("[ViewManager] No views registered during initialization");
    }

    void reset() {
        const auto idx = static_cast<size_t>(current_.get());
        if (views_[idx] != nullptr) {
            views_[idx]->onDeactivate();
        }

        for (auto& view : views_) {
            view = nullptr;
        }

        current_.set(DefaultValue);
    }

private:
    std::array<IView*, COUNT> views_{};
    Signal<EnumT> current_{DefaultValue};
};

}  // namespace bitwig::state
