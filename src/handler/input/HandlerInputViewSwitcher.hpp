#pragma once

/**
 * @file HandlerInputViewSwitcher.hpp
 * @brief Handles view switching via LEFT_TOP button with overlay selector
 *
 * Controls view switching with selector overlay:
 * - LEFT_TOP press (latch): open view selector (toggle behavior)
 * - LEFT_TOP release: close and confirm selection
 * - NAV encoder: navigate views (scoped to overlay)
 * - NAV button: confirm selection without closing
 *
 * Scoping ensures LEFT_TOP cancel in other overlays takes precedence.
 */

#include <lvgl.h>

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>
#include <oc/state/Signal.hpp>

#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief View selector input handler
 *
 * Uses two-level scoping:
 * - scopeElement: mainZone (parent of all views) - for LEFT_TOP open/close
 * - overlayElement: ViewSelector overlay - for NAV navigation
 */
class HandlerInputViewSwitcher {
public:
    HandlerInputViewSwitcher(state::BitwigState& state,
                             oc::api::EncoderAPI& encoders,
                             oc::api::ButtonAPI& buttons,
                             lv_obj_t* scopeElement,
                             lv_obj_t* overlayElement);

    ~HandlerInputViewSwitcher() = default;

    // Non-copyable
    HandlerInputViewSwitcher(const HandlerInputViewSwitcher&) = delete;
    HandlerInputViewSwitcher& operator=(const HandlerInputViewSwitcher&) = delete;

private:
    void setupBindings();

    void openSelector();
    void navigate(float delta);
    void confirmSelection();
    void closeSelector();
    void cancel();

    state::BitwigState& state_;
    oc::api::EncoderAPI& encoders_;
    oc::api::ButtonAPI& buttons_;
    lv_obj_t* scopeElement_;
    lv_obj_t* overlayElement_;

    // Static view names
    static constexpr const char* VIEW_NAMES[] = {"Remote Controls", "Mix", "Clip"};
    static constexpr size_t VIEW_COUNT = 3;

    // Auto-reset latch when overlay hidden externally (by OverlayManager)
    oc::state::Subscription visibleSub_;
};

}  // namespace bitwig::handler
