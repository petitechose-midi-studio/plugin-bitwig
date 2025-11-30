package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;
import util.ColorUtils;
import util.TrackTypeUtils;
import java.util.List;
import java.util.ArrayList;
import java.util.stream.IntStream;

/**
 * TrackHost - Observes Bitwig Tracks and sends updates TO controller
 *
 * RESPONSIBILITIES:
 * 1. Observe track changes (name, color, mute, solo) → send protocol messages
 * 2. Execute mute/solo toggles with observer-based confirmation
 * 3. Send track lists on request
 *
 * DELEGATES TO:
 * - TrackNavigator: Group navigation (enter/exit)
 */
public class TrackHost {
    private final ControllerHost host;
    private final Protocol protocol;
    private final CursorTrack cursorTrack;
    private final TrackBank mainTrackBank;
    private final TrackBank siblingTrackBank;  // Tracks at same level as cursor track
    private final Track parentTrack;  // Parent track for navigation
    private final TrackNavigator navigator;

    // Cache for change detection
    private String lastTrackName = "";
    private long lastTrackColor = 0;

    // Pending toggle tracking for reliable mute/solo confirmation
    // Includes timestamp to auto-expire if observer never fires
    private int pendingMuteTrackIndex = -1;
    private long pendingMuteTimestamp = 0;
    private int pendingSoloTrackIndex = -1;
    private long pendingSoloTimestamp = 0;
    
    private boolean trackListPending = false;  // Debounce for sendTrackList

    public TrackHost(
        ControllerHost host,
        Protocol protocol,
        CursorTrack cursorTrack,
        TrackBank mainTrackBank
    ) {
        this.host = host;
        this.protocol = protocol;
        this.cursorTrack = cursorTrack;
        this.mainTrackBank = mainTrackBank;

        // Create a TrackBank for sibling tracks (same level as cursor track)
        this.siblingTrackBank = cursorTrack.createSiblingsTrackBank(BitwigConfig.MAX_BANK_SIZE, 0, 0, true, false);

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

        // Mark main track bank observables as interested
        mainTrackBank.itemCount().markInterested();
        mainTrackBank.scrollPosition().markInterested();
        mainTrackBank.canScrollBackwards().markInterested();
        mainTrackBank.canScrollForwards().markInterested();

        // Observer for track count changes (add/remove track)
        mainTrackBank.itemCount().addValueObserver(count -> {
            host.println("[TRACK HOST] Main bank track count changed: " + count);
            sendTrackList();
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
            host.println("[TRACK HOST] Sibling bank track count changed: " + count);
            sendTrackList();
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
    }

    /**
     * Send current track state (called at startup)
     */
    public void sendInitialState() {
        sendTrackChange();
        sendTrackList();
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
        track.isGroup().markInterested();
        track.trackType().markInterested();
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
                String trackName = track.name().get();
                host.println("\n[TRACK HOST] MUTE → \"" + trackName + "\" [" + trackIndex + "] = " + (isMuted ? "MUTED" : "UNMUTED") + "\n");
                protocol.send(new TrackMuteMessage(trackIndex, isMuted));
            }
        });

        // Solo observer - sends confirmation when solo state changes
        track.solo().addValueObserver(isSoloed -> {
            long elapsed = System.currentTimeMillis() - pendingSoloTimestamp;
            if (pendingSoloTrackIndex == trackIndex && track.exists().get() && elapsed < BitwigConfig.TOGGLE_CONFIRM_TIMEOUT_MS) {
                pendingSoloTrackIndex = -1;
                String trackName = track.name().get();
                host.println("\n[TRACK HOST] SOLO → \"" + trackName + "\" [" + trackIndex + "] = " + (isSoloed ? "SOLOED" : "UNSOLOED") + "\n");
                protocol.send(new TrackSoloMessage(trackIndex, isSoloed));
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
     * Send track list to controller
     */
    public void sendTrackList() {
        // Debounce: skip if already pending
        if (trackListPending) return;
        trackListPending = true;

        // Delay to let Bitwig API update values (cursorTrack.position(), etc.)
        host.scheduleTask(() -> {
            trackListPending = false;
            // Capture API data AFTER delay
            final TrackBank bank = getCurrentBank();
            final boolean hasParent = hasParentGroup();
            final String parentName = hasParent ? parentTrack.name().get() : "";
            final List<TrackListMessage.Tracks> tracks = buildTrackList(bank);
            final int selectedIndex = findSelectedTrackIndex(tracks);

            // Log and send
            String context = hasParent ? " ↳ " + parentName : " ⌂ root";
            host.println("[TRACK HOST] Tracks: " + tracks.size() + " | Cursor: " + selectedIndex + context);
            protocol.send(new TrackListMessage(tracks.size(), selectedIndex, hasParent, parentName, tracks));
        }, BitwigConfig.TRACK_SELECT_DELAY_MS);
    }

    private List<TrackListMessage.Tracks> buildTrackList(TrackBank bank) {
        final List<TrackListMessage.Tracks> tracks = new ArrayList<>();

        for (int i = 0; i < bank.getSizeOfBank(); i++) {
            Track track = bank.getItemAt(i);
            if (track.exists().get()) {
                tracks.add(new TrackListMessage.Tracks(
                    track.position().get(),
                    track.name().get(),
                    ColorUtils.toUint32Hex(track.color().get()),
                    track.isActivated().get(),
                    track.mute().get(),
                    track.solo().get(),
                    track.isGroup().get(),
                    TrackTypeUtils.toInt(track.trackType().get())
                ));
            }
        }

        return tracks;
    }

    private int findSelectedTrackIndex(List<TrackListMessage.Tracks> tracks) {
        final int cursorPosition = cursorTrack.position().get();
        return IntStream.range(0, tracks.size())
            .filter(i -> tracks.get(i).getTrackIndex() == cursorPosition)
            .findFirst()
            .orElse(0);
    }

    /**
     * Send current track info (name, color, position, type)
     * Called when cursor track changes
     */
    private void sendTrackChange() {
        final String trackName = cursorTrack.name().get();
        final long trackColor = ColorUtils.toUint32Hex(cursorTrack.color().get());
        final int trackPosition = cursorTrack.position().get();
        final int trackType = TrackTypeUtils.toInt(cursorTrack.trackType().get());

        protocol.send(new TrackChangeMessage(trackName, trackColor, trackPosition, trackType));
    }

    /**
     * Execute track navigation: enter group
     * Delegates to TrackNavigator
     */
    public void enterTrackGroup(int trackIndex) {
        navigator.enterTrackGroup(trackIndex, this::sendTrackList);
    }

    /**
     * Execute track navigation: exit group
     * Delegates to TrackNavigator
     */
    public void exitTrackGroup() {
        navigator.exitTrackGroup(this::sendTrackList);
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
            // Observer (addTrackObservers) will send TrackMuteMessage confirmation
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
            // Observer (addTrackObservers) will send TrackSoloMessage confirmation
        }
    }
}
