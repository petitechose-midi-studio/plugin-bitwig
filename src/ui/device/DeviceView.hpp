#pragma once

/**
 * @file DeviceView.hpp
 * @brief Main view for Bitwig device control with reactive state bindings
 *
 * DeviceView subscribes to BitwigState signals and automatically updates
 * UI components when state changes. This is the reactive pattern:
 * - Handlers update BitwigState
 * - DeviceView subscribes to relevant signals
 * - UI updates automatically via subscription callbacks
 */

#include <array>
#include <memory>
#include <vector>

#include <lvgl.h>

#include <oc/state/Signal.hpp>
#include <oc/ui/lvgl/IView.hpp>

#include "component/DeviceSelector.hpp"
#include "component/DeviceStateBar.hpp"
#include "component/PageSelector.hpp"
#include "component/TrackSelector.hpp"
#include "state/BitwigState.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/widget/IParameterWidget.hpp"

namespace Bitwig {

using oc::ui::lvgl::IView;

class DeviceView : public IView {
public:
    /**
     * @brief Construct DeviceView with reactive state bindings
     *
     * @param zone Parent LVGL object (non-owned)
     * @param state Reference to BitwigState (must outlive this view)
     */
    explicit DeviceView(lv_obj_t* zone, bitwig::state::BitwigState& state);
    ~DeviceView();

    // Non-copyable, non-movable (owns subscriptions)
    DeviceView(const DeviceView&) = delete;
    DeviceView& operator=(const DeviceView&) = delete;
    DeviceView(DeviceView&&) = delete;
    DeviceView& operator=(DeviceView&&) = delete;

    // =========================================================================
    // IView interface
    // =========================================================================
    void onActivate() override;
    void onDeactivate() override;
    const char* getViewId() const override { return "bitwig.device"; }
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
    std::vector<oc::state::Subscription> subs_;

    void setupBindings();

    // =========================================================================
    // UI Components
    // =========================================================================
    bool initialized_{false};
    lv_obj_t* zone_;  // Parent zone (non-owned)
    lv_obj_t* top_bar_container_{nullptr};
    lv_obj_t* body_container_{nullptr};

    std::array<std::unique_ptr<IParameterWidget>, 8> widgets_;
    std::unique_ptr<DeviceStateBar> top_bar_component_;
    std::unique_ptr<PageSelector> page_selector_;
    std::unique_ptr<DeviceSelector> device_selector_;
    std::unique_ptr<TrackSelector> track_selector_;

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

}  // namespace Bitwig