#pragma once

/**
 * @file HandlerInputDevicePage.hpp
 * @brief Handles page selector input -> sends protocol messages
 *
 * InputHandler pattern:
 * - Receives APIs from context
 * - Defines its own bindings in constructor
 *
 * Controls page navigation when PageSelector overlay is visible:
 * - LEFT_BOTTOM press (latch): open page selector (toggle behavior)
 * - LEFT_BOTTOM release: close and confirm selection
 * - LEFT_TOP: close without confirming (cancel)
 * - NAV encoder: navigate pages (scoped to overlay)
 * - NAV button: confirm selection without closing
 */

#include <lvgl.h>

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Page selector input handler (Input -> Protocol)
 *
 * Uses two-level scoping like original:
 * - scopeElement: for LEFT_BOTTOM open/close/navigate (view level)
 * - overlayElement: for NAV button confirm (overlay level - higher priority)
 */
class HandlerInputDevicePage {
public:
    HandlerInputDevicePage(state::BitwigState& state,
                           BitwigProtocol& protocol,
                           oc::api::EncoderAPI& encoders,
                           oc::api::ButtonAPI& buttons,
                           lv_obj_t* scopeElement,
                           lv_obj_t* overlayElement);

    ~HandlerInputDevicePage() = default;

    // Non-copyable
    HandlerInputDevicePage(const HandlerInputDevicePage&) = delete;
    HandlerInputDevicePage& operator=(const HandlerInputDevicePage&) = delete;

private:
    void setupBindings();

    void requestPageList();
    void navigate(float delta);
    void confirmSelection();
    void closeSelector();
    void cancel();

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    oc::api::ButtonAPI& buttons_;
    lv_obj_t* scopeElement_;
    lv_obj_t* overlayElement_;

    bool requested_ = false;
};

}  // namespace bitwig::handler
