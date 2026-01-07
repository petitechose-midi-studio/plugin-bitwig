#pragma once

/**
 * @file DeviceSelectorInputHandler.hpp
 * @brief Handles device selector input -> sends protocol messages
 *
 * InputHandler pattern for device navigation when DeviceSelector overlay is visible:
 * - LEFT_CENTER press (latch): open device selector
 * - LEFT_CENTER release: confirm and close
 * - LEFT_TOP release: close without confirming
 * - NAV encoder: navigate devices
 * - NAV button: enter device children
 * - BOTTOM_CENTER: toggle device state
 * - BOTTOM_LEFT: open track selector (stacked)
 */

#include <array>

#include <lvgl.h>

#include <api/InputAPI.hpp>
#include <oc/state/Signal.hpp>
#include <ui/OverlayBindingContext.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Device selector input handler (Input -> Protocol)
 *
 * Two-level scoping:
 * - scopeElement (view): For opening the overlay (LEFT_CENTER press)
 * - overlayElement: For all other bindings when overlay is visible
 *
 * With stacked overlays, DeviceSelector stays visible underneath TrackSelector.
 */
class DeviceSelectorInputHandler {
public:
    using OverlayCtx = core::ui::OverlayBindingContext<bitwig::ui::OverlayType>;

    DeviceSelectorInputHandler(state::BitwigState& state,
                               OverlayCtx overlayCtx,
                               BitwigProtocol& protocol,
                               core::api::InputAPI input);

    ~DeviceSelectorInputHandler() = default;

    // Non-copyable
    DeviceSelectorInputHandler(const DeviceSelectorInputHandler&) = delete;
    DeviceSelectorInputHandler& operator=(const DeviceSelectorInputHandler&) = delete;

private:
    void setupBindings();

    void requestDeviceList();
    void navigate(float delta);
    void selectAndDive();
    void select();
    void enterDeviceAtIndex(int selectorIndex);
    void enterChildAtIndex(int selectorIndex);
    void toggleState();
    void requestTrackList();
    void close();

    bool hasChildrenAtDisplayIndex(int displayIndex) const;
    int getAdjustedDeviceIndex(int selectorIndex) const;
    bool isShowingChildren() const;

    state::BitwigState& state_;
    OverlayCtx overlay_ctx_;
    BitwigProtocol& protocol_;
    core::api::InputAPI input_;

    // Minimal local state - everything else comes from BitwigState Signals
    uint8_t current_device_index_ = 0;  // Device being navigated (for children mode)
    bool requested_ = false;

    // Auto-reset local state when overlay hidden externally
    oc::state::Subscription visible_sub_;
};

}  // namespace bitwig::handler
