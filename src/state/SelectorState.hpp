#pragma once

/**
 * @file SelectorState.hpp
 * @brief Signal-based state for selectors (pages, devices, tracks)
 *
 * Design rationale:
 * - SignalVector: for lists replaced in bulk (names, types, colors)
 * - std::array<Signal<T>>: for per-element reactivity (mute, solo, enabled states)
 */

#include <array>
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
    // Bulk-replaced lists (SignalVector)
    SignalVector<std::string, MAX_DEVICES> names;
    SignalVector<uint8_t, MAX_DEVICES> deviceTypes;
    SignalVector<bool, MAX_DEVICES> hasSlots;
    SignalVector<bool, MAX_DEVICES> hasLayers;
    SignalVector<bool, MAX_DEVICES> hasDrums;

    // Per-element reactive state (updated individually by host)
    std::array<Signal<bool>, MAX_DEVICES> deviceStates{};

    // Child devices (slots/layers/drums) - bulk replaced
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
        hasSlots.clear();
        hasLayers.clear();
        hasDrums.clear();
        for (auto& s : deviceStates) s.set(false);
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
    // Bulk-replaced lists (SignalVector)
    SignalVector<std::string, MAX_TRACKS> names;
    SignalVector<uint8_t, MAX_TRACKS> trackTypes;
    SignalVector<uint32_t, MAX_TRACKS> trackColors;

    // Per-element reactive state (updated individually by host)
    std::array<Signal<bool>, MAX_TRACKS> muteStates{};
    std::array<Signal<bool>, MAX_TRACKS> soloStates{};

    Signal<int8_t> currentIndex{0};
    Signal<int8_t> activeTrackIndex{0};
    Signal<bool> isNested{false};
    Signal<bool> visible{false};

    void reset() {
        names.clear();
        trackTypes.clear();
        trackColors.clear();
        for (auto& s : muteStates) s.set(false);
        for (auto& s : soloStates) s.set(false);
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


/**
 * @brief State for view selector overlay
 */
struct ViewSelectorState {
    Signal<int> selectedIndex{0};
    Signal<bool> visible{false};

    void reset() {
        selectedIndex.set(0);
        visible.set(false);
    }
};

}  // namespace bitwig::state
