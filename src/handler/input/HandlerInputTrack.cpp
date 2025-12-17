#include "HandlerInputTrack.hpp"

#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "handler/InputUtils.hpp"
#include "handler/NestedIndexUtils.hpp"
#include "protocol/struct/EnterTrackGroupMessage.hpp"
#include "state/Constants.hpp"
#include "protocol/struct/ExitTrackGroupMessage.hpp"
#include "protocol/struct/RequestDeviceListWindowMessage.hpp"
#include "protocol/struct/RequestTrackListWindowMessage.hpp"
#include "protocol/struct/TrackMuteMessage.hpp"
#include "protocol/struct/TrackSelectByIndexMessage.hpp"
#include "protocol/struct/TrackSoloMessage.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;

HandlerInputTrack::HandlerInputTrack(state::BitwigState& state,
                                     BitwigProtocol& protocol,
                                     oc::api::EncoderAPI& encoders,
                                     oc::api::ButtonAPI& buttons,
                                     lv_obj_t* overlayElement)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , buttons_(buttons)
    , overlayElement_(overlayElement) {
    setupBindings();
}

void HandlerInputTrack::setupBindings() {
    // === ALL OVERLAY-LEVEL BINDINGS ===
    // All bindings are scoped to the track selector overlay.
    // When the overlay is visible, these bindings capture input with highest priority.
    // This matches the original behavior.

    // Confirm selection and close on BOTTOM_LEFT release
    buttons_.button(ButtonID::BOTTOM_LEFT)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() {
            select();
            close();
        });

    // Close without confirming on LEFT_TOP release
    buttons_.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { close(); });

    // Navigate tracks
    encoders_.encoder(EncoderID::NAV)
        .turn()
        .scope(scope(overlayElement_))
        .then([this](float delta) { navigate(delta); });

    // Enter track group
    buttons_.button(ButtonID::NAV)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { selectAndDive(); });

    // Mute
    buttons_.button(ButtonID::BOTTOM_CENTER)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { toggleMute(); });

    // Solo
    buttons_.button(ButtonID::BOTTOM_RIGHT)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { toggleSolo(); });
}

void HandlerInputTrack::navigate(float delta) {
    auto& ts = state_.trackSelector;

    // Use totalCount for navigation (windowed loading)
    uint8_t totalCount = ts.totalCount.get();
    // Account for back button in nested mode
    int displayCount = ts.isNested.get() ? totalCount + 1 : totalCount;
    if (displayCount == 0) {
        // Fallback to names.size() for legacy/compatibility
        displayCount = static_cast<int>(ts.names.size());
    }
    if (displayCount == 0) return;

    int currentIndex = ts.currentIndex.get();
    int newIndex = currentIndex + static_cast<int>(delta);
    newIndex = wrapIndex(newIndex, displayCount);
    ts.currentIndex.set(newIndex);

    // Prefetch next window if approaching end of loaded data
    uint8_t loadedUpTo = ts.loadedUpTo.get();
    // Convert display index to track index (for nested, display 0 is back button)
    int trackIndex = ts.isNested.get() ? newIndex - 1 : newIndex;

    // Only prefetch if we have a valid track index approaching the loaded boundary
    if (trackIndex >= 0 &&
        loadedUpTo > state::PREFETCH_THRESHOLD &&
        static_cast<uint8_t>(trackIndex) >= loadedUpTo - state::PREFETCH_THRESHOLD &&
        loadedUpTo < totalCount) {
        protocol_.send(Protocol::RequestTrackListWindowMessage{loadedUpTo});
    }
}

void HandlerInputTrack::select() {
    auto& ts = state_.trackSelector;
    int selectedIndex = ts.currentIndex.get();

    if (ts.isNested.get() && selectedIndex == 0) {
        protocol_.send(Protocol::ExitTrackGroupMessage{});
    } else {
        int trackIndex = getAdjustedTrackIndex(selectedIndex);
        size_t count = ts.names.size();
        if (trackIndex >= 0 && static_cast<size_t>(trackIndex) < count) {
            protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
        }
    }
}

void HandlerInputTrack::close() {
    auto& ts = state_.trackSelector;

    if (!ts.visible.get()) return;

    // Hide track selector via OverlayManager (restores device selector from stack)
    state_.overlays.hide();

    // Reset device cache and request fresh windowed device list for restored overlay
    auto& ds = state_.deviceSelector;
    ds.names.clear();
    ds.totalCount.set(0);
    ds.loadedUpTo.set(0);
    protocol_.send(Protocol::RequestDeviceListWindowMessage{0});
}

void HandlerInputTrack::selectAndDive() {
    auto& ts = state_.trackSelector;
    int selectedIndex = ts.currentIndex.get();

    if (ts.isNested.get() && selectedIndex == 0) {
        // Exit to parent group
        protocol_.send(Protocol::ExitTrackGroupMessage{});
    } else {
        int trackIndex = getAdjustedTrackIndex(selectedIndex);
        size_t count = ts.names.size();
        if (trackIndex >= 0 && static_cast<size_t>(trackIndex) < count) {
            // Select the track
            protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
            // If it's a group, also enter inside
            protocol_.send(Protocol::EnterTrackGroupMessage{static_cast<uint8_t>(trackIndex)});
        }
    }

    // Reset cache and refresh track list (will show children if we entered a group)
    ts.names.clear();
    ts.totalCount.set(0);
    ts.loadedUpTo.set(0);
    protocol_.send(Protocol::RequestTrackListWindowMessage{0});
}

void HandlerInputTrack::toggleMute() {
    int trackIndex = getSelectedTrackIndex();
    if (trackIndex >= 0) {
        protocol_.send(Protocol::TrackMuteMessage{static_cast<uint8_t>(trackIndex), true});
    }
}

void HandlerInputTrack::toggleSolo() {
    int trackIndex = getSelectedTrackIndex();
    if (trackIndex >= 0) {
        protocol_.send(Protocol::TrackSoloMessage{static_cast<uint8_t>(trackIndex), true});
    }
}

int HandlerInputTrack::getSelectedTrackIndex() {
    auto& ts = state_.trackSelector;
    int currentIndex = ts.currentIndex.get();

    if (ts.isNested.get() && currentIndex == 0) return -1;

    int trackIndex = getAdjustedTrackIndex(currentIndex);
    size_t count = ts.names.size();
    return (trackIndex >= 0 && static_cast<size_t>(trackIndex) < count) ? trackIndex : -1;
}

int HandlerInputTrack::getAdjustedTrackIndex(int selectorIndex) const {
    return utils::toRawIndex(selectorIndex, state_.trackSelector.isNested.get());
}

}  // namespace bitwig::handler
