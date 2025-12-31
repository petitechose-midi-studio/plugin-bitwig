package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.SendDestinationsListMessage;
import protocol.struct.TrackListWindowMessage;
import config.BitwigConfig;
import util.ColorUtils;
import protocol.TrackType;
import java.util.List;
import java.util.ArrayList;

/**
 * TrackHost - Observes Bitwig Tracks and sends updates TO controller
 *
 * RESPONSIBILITIES:
 * 1. Observe track changes (name, color, mute, solo) → send protocol messages
 * 2. Execute mute/solo toggles with observer-based confirmation
 * 3. Send track lists on request
 * 4. MixView support: Volume/Pan/Sends for 4 tracks with send filtering
 *
 * DELEGATES TO:
 * - TrackNavigator: Group navigation (enter/exit)
 */
public class TrackHost {
    // MixView constants
    private static final int MAX_MIX_TRACKS = 4;
    private static final int MAX_SENDS = 8;

    private final ControllerHost host;
    private final Protocol protocol;
    private final CursorTrack cursorTrack;
    private final TrackBank mainTrackBank;
    private final TrackBank effectTrackBank;  // For send destination names
    private final TrackBank siblingTrackBank;  // Tracks at same level as cursor track
    private final Track parentTrack;  // Parent track for navigation
    private final TrackNavigator navigator;

    // MixView: selected send index for filtering (-1 = none, 0-7 = send index)
    private int selectedMixSendIndex = -1;

    // Cache for change detection
    private String lastTrackName = "";
    private long lastTrackColor = 0;

    // Pending toggle tracking for reliable mute/solo confirmation
    // Includes timestamp to auto-expire if observer never fires
    private int pendingMuteTrackIndex = -1;
    private long pendingMuteTimestamp = 0;
    private int pendingSoloTrackIndex = -1;
    private long pendingSoloTimestamp = 0;

    public TrackHost(
        ControllerHost host,
        Protocol protocol,
        CursorTrack cursorTrack,
        TrackBank mainTrackBank,
        TrackBank effectTrackBank
    ) {
        this.host = host;
        this.protocol = protocol;
        this.cursorTrack = cursorTrack;
        this.mainTrackBank = mainTrackBank;
        this.effectTrackBank = effectTrackBank;

        // Create a TrackBank for sibling tracks (same level as cursor track)
        // Note: 8 sends to match mainTrackBank configuration for send bank access
        this.siblingTrackBank = cursorTrack.createSiblingsTrackBank(BitwigConfig.MAX_BANK_SIZE, MAX_SENDS, 0, true, false);

        // Create parent track object to detect if cursor track has a parent
        this.parentTrack = cursorTrack.createParentTrack(0, 0);

        // Create navigator (handles group navigation)
        this.navigator = new TrackNavigator(host, cursorTrack, mainTrackBank, siblingTrackBank, parentTrack);
    }

    /**
     * Setup track state observers
     */
    public void setupObservers() {
        // Mark cursor track observables as interested
        cursorTrack.exists().markInterested();
        cursorTrack.name().markInterested();
        cursorTrack.color().markInterested();
        cursorTrack.position().markInterested();
        cursorTrack.trackType().markInterested();
        cursorTrack.isActivated().markInterested();
        cursorTrack.mute().markInterested();
        cursorTrack.solo().markInterested();
        cursorTrack.isMutedBySolo().markInterested();
        cursorTrack.arm().markInterested();
        // Cursor track channel parameters
        cursorTrack.volume().value().markInterested();
        cursorTrack.volume().displayedValue().markInterested();
        cursorTrack.pan().value().markInterested();
        cursorTrack.pan().displayedValue().markInterested();

        // Mark main track bank observables as interested
        mainTrackBank.itemCount().markInterested();
        mainTrackBank.scrollPosition().markInterested();
        mainTrackBank.canScrollBackwards().markInterested();
        mainTrackBank.canScrollForwards().markInterested();

        // Observer for track count changes (add/remove track)
        mainTrackBank.itemCount().addValueObserver(count -> {
            sendTrackListWindow(0);  // Use windowed loading
        });

        // Mark all tracks in bank as interested + add observers
        for (int i = 0; i < BitwigConfig.MAX_BANK_SIZE; i++) {
            final int trackIndex = i;
            Track track = mainTrackBank.getItemAt(i);
            markTrackInterested(track);
            addTrackObservers(track, trackIndex);
        }

        // Mark parent track observables as interested
        parentTrack.exists().markInterested();
        parentTrack.name().markInterested();

        // Mark sibling track bank observables as interested + add observers
        siblingTrackBank.itemCount().markInterested();

        // Observer for sibling track count changes (add/remove track in group)
        siblingTrackBank.itemCount().addValueObserver(count -> {
            sendTrackListWindow(0);  // Use windowed loading
        });
        for (int i = 0; i < BitwigConfig.MAX_BANK_SIZE; i++) {
            final int trackIndex = i;
            Track track = siblingTrackBank.getItemAt(i);
            markTrackInterested(track);
            addTrackObservers(track, trackIndex);
        }

        // Observer on cursor track change (name or color)
        cursorTrack.name().addValueObserver(trackName -> {
            if (cursorTrack.exists().get() && !trackName.equals(lastTrackName)) {
                lastTrackName = trackName;
                sendTrackChange();
            }
        });

        cursorTrack.color().addValueObserver((r, g, b) -> {
            long trackColor = ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
            if (trackColor != lastTrackColor) {
                lastTrackColor = trackColor;
                sendTrackChange();
            }
        });

        // Observer for track selection changes (focus moved to different track)
        // Updates track list with new activeTrackIndex for TrackSelector sync
        cursorTrack.position().addValueObserver(position -> {
            if (cursorTrack.exists().get()) {
                sendTrackListWindow(0);
            }
        });

        // Observer for track existence changes (first track added to empty project)
        // Ensures sync when transitioning from 0 to 1 track
        cursorTrack.exists().addValueObserver(exists -> {
            if (exists) {
                lastTrackName = cursorTrack.name().get();
                sendTrackChange();
                sendTrackListWindow(0);
            }
        });

        // Effect track bank for send destination names
        for (int i = 0; i < MAX_SENDS; i++) {
            Track effectTrack = effectTrackBank.getItemAt(i);
            effectTrack.exists().markInterested();
            effectTrack.name().markInterested();
        }

        // MixView observers (first MAX_MIX_TRACKS tracks)
        setupMixViewObservers();
    }

    /**
     * Setup observers for MixView: Volume/Pan/Arm with automation + Sends with filtering
     */
    private void setupMixViewObservers() {
        for (int t = 0; t < MAX_MIX_TRACKS; t++) {
            final int trackIndex = t;
            Track track = mainTrackBank.getItemAt(t);

            // Volume observers
            track.volume().displayedValue().addValueObserver(display -> {
                if (!track.exists().get()) return;
                float value = (float) track.volume().value().get();
                protocol.trackVolumeState(trackIndex, value, display);
            });

            track.volume().hasAutomation().addValueObserver(hasAuto -> {
                if (!track.exists().get()) return;
                protocol.trackVolumeHasAutomationState(trackIndex, hasAuto);
            });

            track.volume().modulatedValue().addValueObserver(modVal -> {
                if (!track.exists().get()) return;
                protocol.trackVolumeModulatedValueState(trackIndex, (float) modVal);
            });

            // Pan observers
            track.pan().displayedValue().addValueObserver(display -> {
                if (!track.exists().get()) return;
                float value = (float) track.pan().value().get();
                protocol.trackPanState(trackIndex, value, display);
            });

            track.pan().hasAutomation().addValueObserver(hasAuto -> {
                if (!track.exists().get()) return;
                protocol.trackPanHasAutomationState(trackIndex, hasAuto);
            });

            track.pan().modulatedValue().addValueObserver(modVal -> {
                if (!track.exists().get()) return;
                protocol.trackPanModulatedValueState(trackIndex, (float) modVal);
            });

            // Arm observer
            track.arm().addValueObserver(isArm -> {
                if (!track.exists().get()) return;
                protocol.trackArmState(trackIndex, isArm);
            });

            // Send observers with filtering
            setupSendObservers(track, trackIndex);
        }
    }

    /**
     * Setup send observers for a track with selectedMixSendIndex filtering
     */
    private void setupSendObservers(Track track, int trackIndex) {
        SendBank sendBank = track.sendBank();

        for (int s = 0; s < MAX_SENDS; s++) {
            final int sendIndex = s;
            Send send = sendBank.getItemAt(s);

            // Value - FILTERED by selectedMixSendIndex
            send.displayedValue().addValueObserver(display -> {
                if (sendIndex != selectedMixSendIndex) return;
                if (!track.exists().get() || !send.exists().get()) return;
                float value = (float) send.value().get();
                protocol.trackSendValueState(trackIndex, sendIndex, value, display);
            });

            // Has automation - FILTERED
            send.hasAutomation().addValueObserver(hasAuto -> {
                if (sendIndex != selectedMixSendIndex) return;
                if (!track.exists().get() || !send.exists().get()) return;
                protocol.trackSendHasAutomationState(trackIndex, sendIndex, hasAuto);
            });

            // Modulated value - FILTERED
            send.modulatedValue().addValueObserver(modVal -> {
                if (sendIndex != selectedMixSendIndex) return;
                if (!track.exists().get() || !send.exists().get()) return;
                protocol.trackSendModulatedValueState(trackIndex, sendIndex, (float) modVal);
            });

            // Enabled - FILTERED
            send.isEnabled().addValueObserver(isEnabled -> {
                if (sendIndex != selectedMixSendIndex) return;
                if (!track.exists().get() || !send.exists().get()) return;
                protocol.trackSendEnabledState(trackIndex, sendIndex, isEnabled);
            });

            // Pre-fader - FILTERED
            send.isPreFader().addValueObserver(isPreFader -> {
                if (sendIndex != selectedMixSendIndex) return;
                if (!track.exists().get() || !send.exists().get()) return;
                protocol.trackSendPreFaderState(trackIndex, sendIndex, isPreFader);
            });
        }
    }

    /**
     * Send current track state (called at startup)
     */
    public void sendInitialState() {
        sendTrackChange();
        sendTrackListWindow(0);  // Use windowed loading
    }

    /**
     * Mark all relevant observables of a track as interested
     * @param track Track to mark observables for
     */
    private void markTrackInterested(Track track) {
        track.exists().markInterested();
        track.name().markInterested();
        track.color().markInterested();
        track.position().markInterested();
        track.isActivated().markInterested();
        track.mute().markInterested();
        track.solo().markInterested();
        track.isMutedBySolo().markInterested();
        track.arm().markInterested();
        track.isGroup().markInterested();
        track.trackType().markInterested();
        // Channel parameters (volume, pan)
        track.volume().value().markInterested();
        track.volume().displayedValue().markInterested();
        track.pan().value().markInterested();
        track.pan().displayedValue().markInterested();
        // Automation for volume/pan
        track.volume().modulatedValue().markInterested();
        track.volume().hasAutomation().markInterested();
        track.pan().modulatedValue().markInterested();
        track.pan().hasAutomation().markInterested();
        // Sends
        SendBank sendBank = track.sendBank();
        for (int s = 0; s < MAX_SENDS; s++) {
            Send send = sendBank.getItemAt(s);
            send.exists().markInterested();
            send.name().markInterested();
            send.value().markInterested();
            send.displayedValue().markInterested();
            send.isEnabled().markInterested();
            send.sendMode().markInterested();
            send.isPreFader().markInterested();
            send.modulatedValue().markInterested();
            send.hasAutomation().markInterested();
        }
    }

    /**
     * Add mute/solo observers to track for reliable toggle confirmation
     * @param track Track to add observers to
     * @param trackIndex Index of track in current bank
     */
    private void addTrackObservers(Track track, int trackIndex) {
        // Mute observer - sends confirmation when mute state changes
        track.mute().addValueObserver(isMuted -> {
            long elapsed = System.currentTimeMillis() - pendingMuteTimestamp;
            if (pendingMuteTrackIndex == trackIndex && track.exists().get() && elapsed < BitwigConfig.TOGGLE_CONFIRM_TIMEOUT_MS) {
                pendingMuteTrackIndex = -1;
                protocol.trackMuteState(trackIndex, isMuted);
            }
        });

        // Solo observer - sends confirmation when solo state changes
        track.solo().addValueObserver(isSoloed -> {
            long elapsed = System.currentTimeMillis() - pendingSoloTimestamp;
            if (pendingSoloTrackIndex == trackIndex && track.exists().get() && elapsed < BitwigConfig.TOGGLE_CONFIRM_TIMEOUT_MS) {
                pendingSoloTrackIndex = -1;
                protocol.trackSoloState(trackIndex, isSoloed);
            }
        });
    }

    /**
     * Get current track bank based on navigation depth
     * Delegates to TrackNavigator
     */
    private TrackBank getCurrentBank() {
        return navigator.getCurrentBank();
    }

    /**
     * Check if inside a track group
     * Delegates to TrackNavigator
     */
    private boolean hasParentGroup() {
        return navigator.hasParentGroup();
    }

    /**
     * Send a windowed portion of the track list (16 items max per window).
     * Used for lazy-loading large track lists.
     *
     * @param requestedStartIndex The starting index requested by the controller
     */
    public void sendTrackListWindow(int requestedStartIndex) {
        host.scheduleTask(() -> {
            final TrackBank bank = getCurrentBank();
            final int totalTrackCount = bank.itemCount().get();
            final int cursorPosition = cursorTrack.position().get();
            final boolean hasParent = hasParentGroup();
            final String parentName = hasParent ? parentTrack.name().get() : "";

            // Clamp startIndex if out of range
            int startIndex = requestedStartIndex;
            if (startIndex >= totalTrackCount) {
                startIndex = Math.max(0, totalTrackCount - BitwigConfig.LIST_WINDOW_SIZE);
            }
            if (startIndex < 0) {
                startIndex = 0;
            }

            // Build windowed track list
            final TrackListWindowMessage.Tracks[] windowTracks = buildTrackListWindow(bank, startIndex);

            protocol.trackListWindow(
                totalTrackCount,
                startIndex,
                cursorPosition,
                hasParent,
                parentName,
                windowTracks
            );
        }, BitwigConfig.TRACK_SELECT_DELAY_MS);
    }

    private TrackListWindowMessage.Tracks[] buildTrackListWindow(TrackBank bank, int startIndex) {
        final List<TrackListWindowMessage.Tracks> tracks = new ArrayList<>();
        int endIndex = Math.min(startIndex + BitwigConfig.LIST_WINDOW_SIZE, bank.getSizeOfBank());

        for (int i = startIndex; i < endIndex; i++) {
            Track track = bank.getItemAt(i);
            if (track.exists().get()) {
                tracks.add(new TrackListWindowMessage.Tracks(
                    track.position().get(),
                    track.name().get(),
                    ColorUtils.toUint32Hex(track.color().get()),
                    track.isActivated().get(),
                    track.mute().get(),
                    track.solo().get(),
                    track.isMutedBySolo().get(),
                    track.arm().get(),
                    track.isGroup().get(),
                    TrackType.fromString(track.trackType().get()),
                    (float) track.volume().value().get(),
                    (float) track.pan().value().get()
                ));
            }
        }

        return tracks.toArray(new TrackListWindowMessage.Tracks[0]);
    }

    /**
     * Send current track info (name, color, position, type, channel params)
     * Called when cursor track changes
     */
    private void sendTrackChange() {
        final String trackName = cursorTrack.name().get();
        final long trackColor = ColorUtils.toUint32Hex(cursorTrack.color().get());
        final int trackPosition = cursorTrack.position().get();
        final TrackType trackType = TrackType.fromString(cursorTrack.trackType().get());
        final boolean isActivated = cursorTrack.isActivated().get();
        final boolean isMute = cursorTrack.mute().get();
        final boolean isSolo = cursorTrack.solo().get();
        final boolean isMutedBySolo = cursorTrack.isMutedBySolo().get();
        final boolean isArm = cursorTrack.arm().get();
        final float volume = (float) cursorTrack.volume().value().get();
        final String volumeDisplay = cursorTrack.volume().displayedValue().get();
        final float pan = (float) cursorTrack.pan().value().get();
        final String panDisplay = cursorTrack.pan().displayedValue().get();

        protocol.trackChange(
            trackName, trackColor, trackPosition, trackType,
            isActivated, isMute, isSolo, isMutedBySolo, isArm,
            volume, volumeDisplay, pan, panDisplay
        );
    }

    /**
     * Execute track navigation: enter group
     * Delegates to TrackNavigator
     */
    public void enterTrackGroup(int trackIndex) {
        navigator.enterTrackGroup(trackIndex, () -> sendTrackListWindow(0));
    }

    /**
     * Execute track navigation: exit group
     * Delegates to TrackNavigator
     */
    public void exitTrackGroup() {
        navigator.exitTrackGroup(() -> sendTrackListWindow(0));
    }

    /**
     * Get track at index from current context (siblings or main bank)
     * Called by TrackController to get correct track based on current navigation level
     *
     * @param trackIndex Index in current bank
     * @return Track at index, or null if doesn't exist
     */
    public Track getTrackAtIndex(int trackIndex) {
        return getCurrentBank().getItemAt(trackIndex);
    }

    /**
     * Execute mute toggle with observer-based confirmation
     * Called FROM TrackController when controller sends mute toggle command
     *
     * Uses observer pattern for confirmation:
     * 1. Set pending flag
     * 2. Toggle Bitwig state
     * 3. Observer fires when Bitwig updates → sends confirmation
     */
    public void toggleMute(int trackIndex) {
        Track track = getTrackAtIndex(trackIndex);
        if (track != null && track.exists().get()) {
            pendingMuteTrackIndex = trackIndex;
            pendingMuteTimestamp = System.currentTimeMillis();
            boolean currentState = track.mute().get();
            track.mute().set(!currentState);
            // Observer (addTrackObservers) will send trackMuteState confirmation
        }
    }

    /**
     * Execute solo toggle with observer-based confirmation
     * Called FROM TrackController when controller sends solo toggle command
     */
    public void toggleSolo(int trackIndex) {
        Track track = getTrackAtIndex(trackIndex);
        if (track != null && track.exists().get()) {
            pendingSoloTrackIndex = trackIndex;
            pendingSoloTimestamp = System.currentTimeMillis();
            boolean currentState = track.solo().get();
            track.solo().set(!currentState);
            // Observer (addTrackObservers) will send trackSoloState confirmation
        }
    }

    // =========================================================================
    // MixView: Send Selection & Destinations
    // =========================================================================

    /**
     * Set selected send index for MixView filtering
     * Called when controller sends SELECT_MIX_SEND
     */
    public void setSelectedMixSend(int sendIndex) {
        this.selectedMixSendIndex = sendIndex;
        if (sendIndex >= 0 && sendIndex < MAX_SENDS) {
            sendCurrentMixSendValues();
        }
    }

    /**
     * Send current values for the selected send across all MixView tracks
     * Called after changing selectedMixSendIndex
     */
    private void sendCurrentMixSendValues() {
        if (selectedMixSendIndex < 0) return;

        for (int t = 0; t < MAX_MIX_TRACKS; t++) {
            Track track = mainTrackBank.getItemAt(t);
            if (!track.exists().get()) continue;

            SendBank sendBank = track.sendBank();
            Send send = sendBank.getItemAt(selectedMixSendIndex);

            if (send.exists().get()) {
                float value = (float) send.value().get();
                String display = send.displayedValue().get();
                protocol.trackSendValueState(t, selectedMixSendIndex, value, display);
                protocol.trackSendHasAutomationState(t, selectedMixSendIndex, send.hasAutomation().get());
                protocol.trackSendModulatedValueState(t, selectedMixSendIndex, (float) send.modulatedValue().get());
                protocol.trackSendEnabledState(t, selectedMixSendIndex, send.isEnabled().get());
                protocol.trackSendPreFaderState(t, selectedMixSendIndex, send.isPreFader().get());
            }
        }
    }

    /**
     * Send list of send destination names (effect track names)
     * Called when controller sends REQUEST_SEND_DESTINATIONS
     */
    public void sendSendDestinations() {
        List<SendDestinationsListMessage.SendDestinations> destinations = new ArrayList<>();

        for (int i = 0; i < MAX_SENDS; i++) {
            Track effectTrack = effectTrackBank.getItemAt(i);
            if (effectTrack.exists().get()) {
                destinations.add(new SendDestinationsListMessage.SendDestinations(
                    i,
                    effectTrack.name().get()
                ));
            }
        }

        SendDestinationsListMessage.SendDestinations[] destArray =
            destinations.toArray(new SendDestinationsListMessage.SendDestinations[0]);
        protocol.sendDestinationsList(destArray.length, destArray);
    }
}
