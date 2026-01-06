#pragma once

/**
 * @file HandlerInputTrack.hpp
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

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>

#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"
#include <ui/OverlayController.hpp>

namespace bitwig::handler {

/**
 * @brief Track selector input handler (Input -> Protocol)
 *
 * All bindings are scoped to the overlay element (highest priority).
 * This matches the original behavior where the track selector overlay
 * captures all input when visible.
 */
class HandlerInputTrack {
public:
    HandlerInputTrack(state::BitwigState& state,
                      ::core::ui::OverlayController<bitwig::ui::OverlayType>& overlays,
                      BitwigProtocol& protocol,
                      oc::api::EncoderAPI& encoders,
                      oc::api::ButtonAPI& buttons,
                      lv_obj_t* overlayElement);

    ~HandlerInputTrack() = default;

    // Non-copyable
    HandlerInputTrack(const HandlerInputTrack&) = delete;
    HandlerInputTrack& operator=(const HandlerInputTrack&) = delete;

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
    ::core::ui::OverlayController<bitwig::ui::OverlayType>& overlays_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    oc::api::ButtonAPI& buttons_;
    lv_obj_t* overlayElement_;
};

}  // namespace bitwig::handler
