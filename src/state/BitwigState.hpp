#pragma once

/**
 * @file BitwigState.hpp
 * @brief Aggregated reactive state for the Bitwig context
 *
 * Single source of truth for all Bitwig-related UI state.
 * Handlers update signals, views subscribe to them for automatic updates.
 *
 * ## Architecture
 *
 * ```
 *                    BitwigState
 *                        │
 *         ┌──────────────┼──────────────┐
 *         │              │              │
 *     Handlers      ─────┼─────     Views
 *   (write signals)      │       (subscribe)
 *         │              │              │
 *         └──────────────┴──────────────┘
 * ```
 *
 * ## Usage
 *
 * ```cpp
 * // Handler updates state
 * void onDeviceNameChanged(const char* name) {
 *     state_.device.name.set(name);
 * }
 *
 * // View subscribes to state
 * sub_ = state_.device.name.subscribe([this](const char* name) {
 *     deviceLabel_.setText(name);
 * });
 * ```
 */

#include "Constants.hpp"
#include "DeviceInfoState.hpp"
#include "HostState.hpp"
#include "LastClickedState.hpp"
#include "OverlayManager.hpp"
#include "ParameterState.hpp"
#include "SelectorState.hpp"
#include "TransportState.hpp"

namespace bitwig::state {

/**
 * @brief Complete reactive state for Bitwig context
 *
 * Non-copyable, non-movable (signals hold subscriber pointers).
 */
struct BitwigState {
    // =========================================================================
    // Host Connection
    // =========================================================================
    HostState host;

    // =========================================================================
    // Transport
    // =========================================================================
    TransportState transport;

    // =========================================================================
    // Current Device
    // =========================================================================
    DeviceInfoState device;

    // =========================================================================
    // Parameters (8 macro slots)
    // =========================================================================
    ParameterState parameters;

    // =========================================================================
    // Last Clicked Parameter
    // =========================================================================
    LastClickedState lastClicked;

    // =========================================================================
    // Selectors
    // =========================================================================
    PageSelectorState pageSelector;
    DeviceSelectorState deviceSelector;
    TrackSelectorState trackSelector;
    CurrentTrackState currentTrack;

    // =========================================================================
    // Overlay Manager (centralized visibility control)
    // =========================================================================
    OverlayManager overlays;

    // =========================================================================
    // Lifecycle
    // =========================================================================

    BitwigState() {
        // Register overlays for centralized management
        overlays.registerOverlay(OverlayType::PAGE_SELECTOR, pageSelector.visible);
        overlays.registerOverlay(OverlayType::DEVICE_SELECTOR, deviceSelector.visible);
        overlays.registerOverlay(OverlayType::TRACK_SELECTOR, trackSelector.visible);
    }

    // Non-copyable, non-movable
    BitwigState(const BitwigState&) = delete;
    BitwigState& operator=(const BitwigState&) = delete;
    BitwigState(BitwigState&&) = delete;
    BitwigState& operator=(BitwigState&&) = delete;

    /**
     * @brief Reset all state to defaults
     *
     * Called when host disconnects or on context cleanup.
     */
    void resetAll() {
        host.reset();
        transport.reset();
        device.reset();
        parameters.resetAll();
        lastClicked.reset();
        pageSelector.reset();
        deviceSelector.reset();
        trackSelector.reset();
        currentTrack.reset();
        overlays.hideAll();
    }
};

}  // namespace bitwig::state
