#include "HandlerInputTrack.hpp"

#include <oc/debug/InvariantAssert.hpp>
#include <oc/log/Log.hpp>
#include <oc/ui/lvgl/Scope.hpp>

#include "handler/InputUtils.hpp"
#include "handler/NestedIndexUtils.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using state::OverlayType;

HandlerInputTrack::HandlerInputTrack(state::BitwigState& state,
                                     state::OverlayController& overlays,
                                     BitwigProtocol& protocol,
                                     oc::api::EncoderAPI& encoders,
                                     oc::api::ButtonAPI& buttons,
                                     lv_obj_t* overlayElement)
    : state_(state)
    , overlays_(overlays)
    , protocol_(protocol)
    , encoders_(encoders)
    , buttons_(buttons)
    , overlayElement_(overlayElement) {
    setupBindings();
}

void HandlerInputTrack::setupBindings() {
    // Close without confirming
    buttons_.button(ButtonID::LEFT_TOP)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() { close(); });

    // Confirm selection and close
    buttons_.button(ButtonID::LEFT_CENTER)
        .press()
        .scope(scope(overlayElement_))
        .then([this]() {
            select();
            close();
        });

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

    // Confirm and close (same button that opened TrackSelector from DeviceSelector)
    buttons_.button(ButtonID::BOTTOM_LEFT)
        .release()
        .scope(scope(overlayElement_))
        .then([this]() {
            select();
            close();
        });
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
        shouldPrefetch<state::PREFETCH_THRESHOLD>(trackIndex, loadedUpTo, totalCount)) {
        protocol_.requestTrackListWindow(loadedUpTo);
    }
}

void HandlerInputTrack::select() {
    auto& ts = state_.trackSelector;
    int selectedIndex = ts.currentIndex.get();

    if (ts.isNested.get() && selectedIndex == 0) {
        protocol_.exitTrackGroup();
    } else {
        int trackIndex = getAdjustedTrackIndex(selectedIndex);
        size_t count = ts.names.size();
        if (trackIndex >= 0 && static_cast<size_t>(trackIndex) < count) {
            protocol_.trackSelect(static_cast<uint8_t>(trackIndex));
        }
    }
}

void HandlerInputTrack::close() {
    auto& ts = state_.trackSelector;

    if (!ts.visible.get()) return;

    // OverlayController handles latch cleanup and restores device selector from stack
    overlays_.hide();

    // Request fresh device list - don't clear cache to avoid flash
    // Old data remains visible until new data arrives
    protocol_.requestDeviceListWindow(0);
}

void HandlerInputTrack::selectAndDive() {
    auto& ts = state_.trackSelector;
    int selectedIndex = ts.currentIndex.get();

    if (ts.isNested.get() && selectedIndex == 0) {
        // Exit to parent group
        protocol_.exitTrackGroup();
    } else {
        int trackIndex = getAdjustedTrackIndex(selectedIndex);
        size_t count = ts.names.size();
        if (trackIndex >= 0 && static_cast<size_t>(trackIndex) < count) {
            // Select the track
            protocol_.trackSelect(static_cast<uint8_t>(trackIndex));
            // If it's a group, also enter inside
            protocol_.enterTrackGroup(static_cast<uint8_t>(trackIndex));
        }
    }

    // Reset cache and refresh track list (will show children if we entered a group)
    ts.names.clear();
    ts.totalCount.set(0);
    ts.loadedUpTo.set(0);
    protocol_.requestTrackListWindow(0);
}

void HandlerInputTrack::toggleMute() {
    int trackIndex = getSelectedTrackIndex();
    if (trackIndex >= 0) {
        protocol_.trackMute(static_cast<uint8_t>(trackIndex), true);
    }
}

void HandlerInputTrack::toggleSolo() {
    int trackIndex = getSelectedTrackIndex();
    if (trackIndex >= 0) {
        protocol_.trackSolo(static_cast<uint8_t>(trackIndex), true);
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
