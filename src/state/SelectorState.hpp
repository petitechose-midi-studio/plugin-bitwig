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
#include "protocol/DeviceType.hpp"
#include "protocol/TrackType.hpp"

namespace bitwig::state {

using oc::state::Signal;
using oc::state::SignalLabel;
using oc::state::SignalVector;

// =============================================================================
// Page Selector
// =============================================================================

/**
 * @brief State for page (parameter bank) selection
 *
 * Supports windowed loading for large page lists (>16 pages):
 * - names: accumulated cache of page names (up to MAX_PAGES)
 * - totalCount: absolute total number of pages from host
 * - loadedUpTo: highest index loaded so far (for prefetch trigger)
 */
struct PageSelectorState {
    SignalVector<std::string, MAX_PAGES> names;  // Accumulated cache
    Signal<int> selectedIndex{0};
    Signal<bool> visible{false};

    // Windowed loading state
    Signal<uint8_t> totalCount{0};   // Total pages (absolute, from host)
    Signal<uint8_t> loadedUpTo{0};   // Highest loaded index (for prefetch)

    void reset() {
        names.clear();
        selectedIndex.set(0);
        visible.set(false);
        totalCount.set(0);
        loadedUpTo.set(0);
    }
};

// =============================================================================
// Device Selector
// =============================================================================

/**
 * @brief State for device chain selection
 *
 * Supports windowed loading for large device lists (>16 devices):
 * - names: accumulated cache of device names (up to MAX_DEVICES)
 * - totalCount: absolute total number of devices from host
 * - loadedUpTo: highest index loaded so far (for prefetch trigger)
 */
struct DeviceSelectorState {
    // Bulk-replaced lists (SignalVector) - accumulated cache for windowed loading
    SignalVector<std::string, MAX_DEVICES> names;
    SignalVector<DeviceType, MAX_DEVICES> deviceTypes;
    SignalVector<bool, MAX_DEVICES> hasSlots;
    SignalVector<bool, MAX_DEVICES> hasLayers;
    SignalVector<bool, MAX_DEVICES> hasDrums;

    // Per-element reactive state (updated individually by host)
    std::array<Signal<bool>, MAX_DEVICES> deviceStates{};

    // Child devices (slots/layers/drums) - bulk replaced
    SignalVector<std::string, MAX_CHILDREN> childrenNames;
    SignalVector<uint8_t, MAX_CHILDREN> childrenTypes;

    Signal<int> currentIndex{0};
    Signal<int> activeDeviceIndex{0};
    Signal<bool> isNested{false};
    Signal<bool> showingChildren{false};
    Signal<bool> showFooter{false};
    Signal<bool> visible{false};

    // Windowed loading state
    Signal<uint8_t> totalCount{0};   // Total devices (absolute, from host)
    Signal<uint8_t> loadedUpTo{0};   // Highest loaded index (for prefetch)
    Signal<bool> loading{false};     // True while waiting for host response

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
        totalCount.set(0);
        loadedUpTo.set(0);
        loading.set(false);
    }
};

// =============================================================================
// Track Selector
// =============================================================================

/**
 * @brief State for track selection
 *
 * Supports windowed loading for large track lists (>16 tracks):
 * - names: accumulated cache of track names (up to MAX_TRACKS)
 * - totalCount: absolute total number of tracks from host
 * - loadedUpTo: highest index loaded so far (for prefetch trigger)
 */
struct TrackSelectorState {
    // Bulk-replaced lists (SignalVector) - accumulated cache for windowed loading
    SignalVector<std::string, MAX_TRACKS> names;
    SignalVector<TrackType, MAX_TRACKS> trackTypes;
    SignalVector<uint32_t, MAX_TRACKS> trackColors;

    // Per-element reactive state (updated individually by host)
    std::array<Signal<bool>, MAX_TRACKS> muteStates{};
    std::array<Signal<bool>, MAX_TRACKS> soloStates{};

    Signal<int> currentIndex{0};
    Signal<int> activeTrackIndex{0};
    Signal<bool> isNested{false};
    Signal<bool> visible{false};

    // Windowed loading state
    Signal<uint8_t> totalCount{0};   // Total tracks (absolute, from host)
    Signal<uint8_t> loadedUpTo{0};   // Highest loaded index (for prefetch)

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
        totalCount.set(0);
        loadedUpTo.set(0);
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
    Signal<TrackType> trackType{TrackType::AUDIO};

    void reset() {
        name.set("");
        color.set(0xFFFFFF);
        trackType.set(TrackType::AUDIO);
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
