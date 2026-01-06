#pragma once

/**
 * @file RemoteControlsView.hpp
 * @brief Main view for Bitwig remote controls with reactive state bindings
 *
 * RemoteControlsView subscribes to BitwigState signals and automatically updates
 * UI components when state changes. This is the reactive pattern:
 * - Handlers update BitwigState
 * - RemoteControlsView subscribes to relevant signals
 * - UI updates automatically via subscription callbacks
 */

#include <array>
#include <memory>
#include <vector>

#include <lvgl.h>

#include <oc/state/Signal.hpp>
#include <oc/state/SignalWatcher.hpp>
#include <oc/ui/lvgl/IView.hpp>

#include "protocol/ParameterType.hpp"
#include "RemoteControlsPageSelector.hpp"
#include "state/BitwigState.hpp"
#include "ui/device/DeviceSelector.hpp"
#include "ui/device/DeviceStateBar.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/track/TrackSelector.hpp"
#include "ui/widget/IParameterWidget.hpp"

namespace bitwig::ui {

using oc::ui::lvgl::IView;

class RemoteControlsView : public IView {
public:
    /**
     * @brief Construct RemoteControlsView with reactive state bindings
     *
     * @param zone Parent LVGL object (non-owned)
     * @param state Reference to BitwigState (must outlive this view)
     */
    explicit RemoteControlsView(lv_obj_t* zone, bitwig::state::BitwigState& state);
    ~RemoteControlsView();

    // Non-copyable, non-movable (owns subscriptions)
    RemoteControlsView(const RemoteControlsView&) = delete;
    RemoteControlsView& operator=(const RemoteControlsView&) = delete;
    RemoteControlsView(RemoteControlsView&&) = delete;
    RemoteControlsView& operator=(RemoteControlsView&&) = delete;

    // =========================================================================
    // IView interface
    // =========================================================================
    void onActivate() override;
    void onDeactivate() override;
    const char* getViewId() const override { return "bitwig.remotecontrols"; }
    lv_obj_t* getElement() const override { return zone_; }

    // =========================================================================
    // Overlay element accessors (for input handler scoping)
    // =========================================================================
    lv_obj_t* getPageSelectorElement() const;
    lv_obj_t* getDeviceSelectorElement() const;
    lv_obj_t* getTrackSelectorElement() const;

private:
    // =========================================================================
    // State & Subscriptions
    // =========================================================================
    bitwig::state::BitwigState& state_;
    std::vector<oc::state::Subscription> subs_;  // Individual subscriptions
    oc::state::SignalWatcher watcher_;           // Coalesced signal groups

    void setupBindings();

    // =========================================================================
    // UI Components
    // =========================================================================
    bool initialized_{false};
    lv_obj_t* zone_;  // Parent zone (non-owned)
    lv_obj_t* top_bar_container_{nullptr};
    lv_obj_t* body_container_{nullptr};

    std::array<std::unique_ptr<IParameterWidget>, bitwig::state::PARAMETER_COUNT> widgets_;
    std::array<ParameterType, bitwig::state::PARAMETER_COUNT> widgetTypes_{};
    std::unique_ptr<DeviceStateBar> top_bar_component_;
    std::unique_ptr<RemoteControlsPageSelector> page_selector_;
    std::unique_ptr<DeviceSelector> device_selector_;
    std::unique_ptr<TrackSelector> track_selector_;

    // =========================================================================
    // Dirty Flag System (debounces UI updates)
    // =========================================================================
    std::array<bool, bitwig::state::PARAMETER_COUNT> paramDirty_{};
    lv_timer_t* updateTimer_{nullptr};  // Timer to process dirty flags

    void markParameterDirty(uint8_t index);
    void processDirtyParameters();
    static void onUpdateTimer(lv_timer_t* timer);

    // =========================================================================
    // UI Creation
    // =========================================================================
    void createUI();
    void setupLayout();
    void createDeviceStateBar();

    // =========================================================================
    // Update Helpers (called by subscription callbacks)
    // =========================================================================
    void updateDeviceInfo();
    void updateParameter(uint8_t index);
    void updatePageSelector();
    void updateDeviceSelector();
    void updateTrackSelector();

    /**
     * @brief Create or recreate parameter widget based on type
     * @param index Parameter slot index (0-7)
     */
    void ensureWidgetForType(uint8_t index);
};

}  // namespace bitwig::ui