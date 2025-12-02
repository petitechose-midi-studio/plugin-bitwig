#include "TrackInputHandler.hpp"

#include "InputUtils.hpp"

#include "protocol/struct/EnterTrackGroupMessage.hpp"
#include "protocol/struct/ExitTrackGroupMessage.hpp"
#include "protocol/struct/RequestDeviceListMessage.hpp"
#include "protocol/struct/TrackMuteMessage.hpp"
#include "protocol/struct/TrackSelectByIndexMessage.hpp"
#include "protocol/struct/TrackSoloMessage.hpp"
#include "ui/device/DeviceView.hpp"

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

TrackInputHandler::TrackInputHandler(ControllerAPI &api, DeviceView &view,
                                     Protocol::Protocol &protocol)
    : api_(api), view_(view), protocol_(protocol) {
    setupInputBindings();
}

TrackInputHandler::~TrackInputHandler() = default;

// =============================================================================
// Public API
// =============================================================================

void TrackInputHandler::setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex,
                                          bool isNested) {
    // Only update cursor if content changed (entered/exited group or first open)
    bool contentChanged = (state_.count != trackCount) || (state_.isNested != isNested);

    state_.count = trackCount;
    state_.isNested = isNested;

    if (contentChanged) {
        int selectorIndex = isNested ? currentTrackIndex + 1 : currentTrackIndex;
        view_.state().trackSelector.currentIndex = selectorIndex;
        view_.state().dirty.trackSelector = true;
        view_.sync();
    }

    api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::RELATIVE);
}

// =============================================================================
// Bindings Setup
// =============================================================================

void TrackInputHandler::setupInputBindings() {
    lv_obj_t *overlay = view_.getTrackSelectorElement();

    // Confirm selection and close on press (scoped to overlay)
    api_.onReleased(
        ButtonID::BOTTOM_LEFT,
        [this]() {
            select();
            close();
        },
        overlay);

    // Close without confirming (scoped to overlay)
    api_.onReleased(ButtonID::LEFT_TOP, [this]() { close(); }, overlay);

    // Navigate tracks while holding
    api_.onTurned(EncoderID::NAV, [this](float delta) { navigate(delta); }, overlay);

    // Enter track group
    api_.onReleased(ButtonID::NAV, [this]() { selectAndDive(); }, overlay);

    // Mute/Solo
    api_.onReleased(ButtonID::BOTTOM_CENTER, [this]() { toggleMute(); }, overlay);
    api_.onReleased(ButtonID::BOTTOM_RIGHT, [this]() { toggleSolo(); }, overlay);
}

// =============================================================================
// Handlers
// =============================================================================

void TrackInputHandler::navigate(float delta) {
    int itemCount = static_cast<int>(view_.state().trackSelector.names.size());
    if (itemCount == 0) return;

    auto &state = view_.state().trackSelector;
    int newIndex = state.currentIndex + static_cast<int>(delta);
    newIndex = InputUtils::wrapIndex(newIndex, itemCount);
    state.currentIndex = newIndex;
    view_.state().dirty.trackSelector = true;
    view_.sync();
}

void TrackInputHandler::select() {
    int selectedIndex = view_.state().trackSelector.currentIndex;

    if (state_.isNested && selectedIndex == 0) {
        protocol_.send(Protocol::ExitTrackGroupMessage{});
    } else {
        int trackIndex = getAdjustedTrackIndex(selectedIndex);
        if (trackIndex >= 0 && trackIndex < state_.count) {
            protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
        }
    }
}

void TrackInputHandler::close() {
    if (!view_.state().trackSelector.visible) {
        state_.requested = false;
        return;
    }

    view_.state().trackSelector.visible = false;
    view_.state().dirty.trackSelector = true;
    view_.state().deviceSelector.visible = true;
    view_.state().dirty.deviceSelector = true;
    view_.sync();
    protocol_.send(Protocol::RequestDeviceListMessage{});
    // Don't reset requested here - DeviceSelectorInputHandler checks it to prevent reopen
}

void TrackInputHandler::selectAndDive() {
    int selectedIndex = view_.state().trackSelector.currentIndex;

    if (state_.isNested && selectedIndex == 0) {
        // Exit to parent group
        protocol_.send(Protocol::ExitTrackGroupMessage{});
    } else {
        int trackIndex = getAdjustedTrackIndex(selectedIndex);
        if (trackIndex >= 0 && trackIndex < state_.count) {
            // Select the track
            protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
            // If it's a group, also enter inside
            protocol_.send(Protocol::EnterTrackGroupMessage{static_cast<uint8_t>(trackIndex)});
        }
    }

    // Refresh track list (will show children if we entered a group)
    protocol_.send(Protocol::RequestTrackListMessage{});
}

void TrackInputHandler::toggleMute() {
    int trackIndex = getSelectedTrackIndex();
    if (trackIndex >= 0) {
        protocol_.send(Protocol::TrackMuteMessage{static_cast<uint8_t>(trackIndex), true});
    }
}

void TrackInputHandler::toggleSolo() {
    int trackIndex = getSelectedTrackIndex();
    if (trackIndex >= 0) {
        protocol_.send(Protocol::TrackSoloMessage{static_cast<uint8_t>(trackIndex), true});
    }
}

// =============================================================================
// Helpers
// =============================================================================

int TrackInputHandler::getSelectedTrackIndex() {
    int currentIndex = view_.state().trackSelector.currentIndex;
    if (state_.isNested && currentIndex == 0) return -1;

    int trackIndex = getAdjustedTrackIndex(currentIndex);
    return (trackIndex >= 0 && trackIndex < state_.count) ? trackIndex : -1;
}

int TrackInputHandler::getAdjustedTrackIndex(int selectorIndex) const {
    return state_.isNested ? selectorIndex - 1 : selectorIndex;
}

}  // namespace Bitwig
