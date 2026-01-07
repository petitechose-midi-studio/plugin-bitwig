#pragma once

/**
 * @file TrackInputHandler.hpp
 * @brief Handles track selector input -> sends protocol messages
 *
 * InputHandler pattern for track navigation when TrackSelector overlay is visible:
 * - BOTTOM_LEFT release: confirm and close
 * - LEFT_TOP release: close without confirming
 * - NAV encoder: navigate tracks
 * - NAV button: enter track group
 * - BOTTOM_CENTER: toggle mute
 * - BOTTOM_RIGHT: toggle solo
 */

#include <lvgl.h>

#include <api/InputAPI.hpp>
#include <ui/OverlayBindingContext.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Track selector input handler (Input -> Protocol)
 *
 * All bindings are scoped to the overlay element (highest priority).
 * This matches the original behavior where the track selector overlay
 * captures all input when visible.
 */
class TrackInputHandler {
public:
    using OverlayCtx = core::ui::OverlayBindingContext<bitwig::ui::OverlayType>;

    TrackInputHandler(state::BitwigState& state,
                      OverlayCtx overlayCtx,
                      BitwigProtocol& protocol,
                      core::api::InputAPI input);

    ~TrackInputHandler() = default;

    // Non-copyable
    TrackInputHandler(const TrackInputHandler&) = delete;
    TrackInputHandler& operator=(const TrackInputHandler&) = delete;

private:
    void setupBindings();

    void navigate(float delta);
    void select();
    void selectAndDive();
    void close();
    void toggleMute();
    void toggleSolo();

    int getSelectedTrackIndex();
    int getAdjustedTrackIndex(int selectorIndex) const;

    state::BitwigState& state_;
    OverlayCtx overlay_ctx_;
    BitwigProtocol& protocol_;
    core::api::InputAPI input_;
};

}  // namespace bitwig::handler
