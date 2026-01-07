#pragma once

/**
 * @file ViewSwitcherInputHandler.hpp
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

#include <api/InputAPI.hpp>
#include <oc/state/Signal.hpp>
#include <ui/OverlayBindingContext.hpp>

#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief View selector input handler
 *
 * Uses two-level scoping:
 * - scopeElement: mainZone (parent of all views) - for LEFT_TOP open/close
 * - overlayElement: ViewSelector overlay - for NAV navigation
 */
class ViewSwitcherInputHandler {
public:
    using OverlayCtx = core::ui::OverlayBindingContext<bitwig::ui::OverlayType>;

    ViewSwitcherInputHandler(state::BitwigState& state,
                             OverlayCtx overlayCtx,
                             core::api::InputAPI input);

    ~ViewSwitcherInputHandler() = default;

    // Non-copyable
    ViewSwitcherInputHandler(const ViewSwitcherInputHandler&) = delete;
    ViewSwitcherInputHandler& operator=(const ViewSwitcherInputHandler&) = delete;

private:
    void setupBindings();

    void openSelector();
    void navigate(float delta);
    void confirmSelection();
    void closeSelector();
    void cancel();

    state::BitwigState& state_;
    OverlayCtx overlay_ctx_;
    core::api::InputAPI input_;

    // Static view names
    static constexpr const char* VIEW_NAMES[] = {"Remote Controls", "Mix", "Clip"};
    static constexpr size_t VIEW_COUNT = 3;
};

}  // namespace bitwig::handler
