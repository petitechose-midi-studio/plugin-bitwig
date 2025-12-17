#pragma once

/**
 * @file HandlerInputDeviceSelector.hpp
 * @brief Handles device selector input -> sends protocol messages
 *
 * InputHandler pattern for device navigation when DeviceSelector overlay is visible:
 * - LEFT_CENTER press (latch): open device selector
 * - LEFT_CENTER release: confirm and close
 * - LEFT_TOP release: close without confirming
 * - NAV encoder: navigate devices
 * - NAV button: enter device children
 * - BOTTOM_CENTER: toggle device state
 * - BOTTOM_LEFT: request track list
 */

#include <array>

#include <lvgl.h>

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>
#include <oc/state/Signal.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Device selector input handler (Input -> Protocol)
 *
 * Uses two-level scoping like original:
 * - scopeElement: for LEFT_CENTER open/close (view level)
 * - overlayElement: for NAV, BOTTOM_CENTER, BOTTOM_LEFT (overlay level - higher priority)
 */
class HandlerInputDeviceSelector {
public:
    HandlerInputDeviceSelector(state::BitwigState& state,
                               BitwigProtocol& protocol,
                               oc::api::EncoderAPI& encoders,
                               oc::api::ButtonAPI& buttons,
                               lv_obj_t* scopeElement,
                               lv_obj_t* overlayElement);

    ~HandlerInputDeviceSelector() = default;

    // Non-copyable
    HandlerInputDeviceSelector(const HandlerInputDeviceSelector&) = delete;
    HandlerInputDeviceSelector& operator=(const HandlerInputDeviceSelector&) = delete;

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
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    oc::api::ButtonAPI& buttons_;
    lv_obj_t* scopeElement_;
    lv_obj_t* overlayElement_;

    // Minimal local state - everything else comes from BitwigState Signals
    uint8_t currentDeviceIndex_ = 0;  // Device being navigated (for children mode)
    bool requested_ = false;

    // Auto-reset latch when overlay hidden externally (by OverlayManager)
    oc::state::Subscription visibleSub_;
};

}  // namespace bitwig::handler
