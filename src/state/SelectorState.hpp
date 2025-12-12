#pragma once

/**
 * @file SelectorState.hpp
 * @brief Signal-based state for selectors (pages, devices, tracks)
 */

#include <string>

#include <oc/state/Signal.hpp>
#include <oc/state/SignalString.hpp>
#include <oc/state/SignalVector.hpp>

#include "Constants.hpp"

namespace bitwig::state {

using oc::state::Signal;
using oc::state::SignalLabel;
using oc::state::SignalVector;

// =============================================================================
// Page Selector
// =============================================================================

/**
 * @brief State for page (parameter bank) selection
 */
struct PageSelectorState {
    SignalVector<std::string, MAX_PAGES> names;
    Signal<int8_t> selectedIndex{0};
    Signal<bool> visible{false};

    void reset() {
        names.clear();
        selectedIndex.set(0);
        visible.set(false);
    }
};

// =============================================================================
// Device Selector
// =============================================================================

/**
 * @brief State for device chain selection
 */
struct DeviceSelectorState {
    SignalVector<std::string, MAX_DEVICES> names;
    SignalVector<uint8_t, MAX_DEVICES> deviceTypes;
    SignalVector<bool, MAX_DEVICES> deviceStates;
    SignalVector<bool, MAX_DEVICES> hasSlots;
    SignalVector<bool, MAX_DEVICES> hasLayers;
    SignalVector<bool, MAX_DEVICES> hasDrums;

    // Child devices (slots/layers/drums)
    SignalVector<std::string, MAX_CHILDREN> childrenNames;
    SignalVector<uint8_t, MAX_CHILDREN> childrenTypes;

    Signal<int8_t> currentIndex{0};
    Signal<int8_t> activeDeviceIndex{0};
    Signal<bool> isNested{false};
    Signal<bool> showingChildren{false};
    Signal<bool> showFooter{false};
    Signal<bool> visible{false};

    void reset() {
        names.clear();
        deviceTypes.clear();
        deviceStates.clear();
        hasSlots.clear();
        hasLayers.clear();
        hasDrums.clear();
        childrenNames.clear();
        childrenTypes.clear();
        currentIndex.set(0);
        activeDeviceIndex.set(0);
        isNested.set(false);
        showingChildren.set(false);
        showFooter.set(false);
        visible.set(false);
    }
};

// =============================================================================
// Track Selector
// =============================================================================

/**
 * @brief State for track selection
 */
struct TrackSelectorState {
    SignalVector<std::string, MAX_TRACKS> names;
    SignalVector<bool, MAX_TRACKS> muteStates;
    SignalVector<bool, MAX_TRACKS> soloStates;
    SignalVector<uint8_t, MAX_TRACKS> trackTypes;
    SignalVector<uint32_t, MAX_TRACKS> trackColors;

    Signal<int8_t> currentIndex{0};
    Signal<int8_t> activeTrackIndex{0};
    Signal<bool> isNested{false};
    Signal<bool> visible{false};

    void reset() {
        names.clear();
        muteStates.clear();
        soloStates.clear();
        trackTypes.clear();
        trackColors.clear();
        currentIndex.set(0);
        activeTrackIndex.set(0);
        isNested.set(false);
        visible.set(false);
    }
};

// =============================================================================
// Current Track Info
// =============================================================================

/**
 * @brief State for the currently active track (whose devices are displayed)
 */
struct CurrentTrackState {
    SignalLabel name;
    Signal<uint32_t> color{0xFFFFFF};
    Signal<TrackType> trackType{TrackType::UNKNOWN};

    void reset() {
        name.set("");
        color.set(0xFFFFFF);
        trackType.set(TrackType::UNKNOWN);
    }
};

}  // namespace bitwig::state
