package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;
import util.ColorUtils;
import java.util.List;
import java.util.ArrayList;
import java.util.stream.IntStream;

/**
 * TrackHost - Observes Bitwig Tracks and sends updates TO controller
 *
 * RESPONSIBILITIES:
 * 1. Observe track changes (name, mute, solo, activated) → send protocol messages
 * 2. Execute track navigation (enter/exit groups, requires siblingTrackBank)
 * 3. Execute mute/solo toggles with observer-based confirmation
 * 4. Send track lists on request
 *
 * NOTE: Navigation logic is here because it needs access to siblingTrackBank
 * which can't be shared with TrackController (Bitwig API limitation)
 */
public class TrackHost {
    private final ControllerHost host;
    private final Protocol protocol;
    private final CursorTrack cursorTrack;
    private final TrackBank mainTrackBank;
    private final TrackBank siblingTrackBank;  // Tracks at same level as cursor track
    private final Track parentTrack;  // Parent track for navigation

    // Navigation depth tracking
    private int navigationDepth = 0;  // 0 = root level, >0 = inside group(s)

    // Cache for change detection
    private String lastTrackName = "";

    // Pending toggle tracking for reliable mute/solo confirmation
    private int pendingMuteTrackIndex = -1;
    private int pendingSoloTrackIndex = -1;

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
        this.siblingTrackBank = cursorTrack.createSiblingsTrackBank(32, 0, 0, true, false);

        // Create parent track object to detect if cursor track has a parent
        this.parentTrack = cursorTrack.createParentTrack(0, 0);
    }

    /**
     * Setup track state observers
     */
    public void setup() {
        // Mark cursor track observables as interested
        cursorTrack.exists().markInterested();
        cursorTrack.name().markInterested();
        cursorTrack.position().markInterested();
        cursorTrack.isActivated().markInterested();
        cursorTrack.mute().markInterested();
        cursorTrack.solo().markInterested();

        // Mark main track bank observables as interested
        mainTrackBank.itemCount().markInterested();
        mainTrackBank.scrollPosition().markInterested();
        mainTrackBank.canScrollBackwards().markInterested();
        mainTrackBank.canScrollForwards().markInterested();

        // Mark all tracks in bank as interested (32 slots) + add observers
        for (int i = 0; i < 32; i++) {
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
        for (int i = 0; i < 32; i++) {
            final int trackIndex = i;
            Track track = siblingTrackBank.getItemAt(i);
            markTrackInterested(track);
            addTrackObservers(track, trackIndex);
        }

        // Observer on cursor track name change
        cursorTrack.name().addValueObserver(trackName -> {
            if (cursorTrack.exists().get() && !trackName.equals(lastTrackName)) {
                lastTrackName = trackName;
                // Optional: send track change notification if needed
            }
        });
    }

    /**
     * Send current track state (called at startup)
     *
     * Delay send to ensure Bitwig API values are stabilized
     */
    public void sendInitialState() {
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
     * Convert Bitwig track type string to uint8
     * @param trackType Track type string from Bitwig API
     * @return 0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master
     */
    private int trackTypeToInt(String trackType) {
        switch (trackType) {
            case "Audio": return 0;
            case "Instrument": return 1;
            case "Hybrid": return 2;
            case "Group": return 3;
            case "Effect": return 4;
            case "Master": return 5;
            default: return 0;
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
            if (pendingMuteTrackIndex == trackIndex && track.exists().get()) {
                pendingMuteTrackIndex = -1;
                String trackName = track.name().get();
                host.println("\n[TRACK HOST] MUTE → \"" + trackName + "\" [" + trackIndex + "] = " + (isMuted ? "MUTED" : "UNMUTED") + "\n");
                protocol.send(new protocol.struct.TrackMuteMessage(trackIndex, isMuted));
            }
        });

        // Solo observer - sends confirmation when solo state changes
        track.solo().addValueObserver(isSoloed -> {
            if (pendingSoloTrackIndex == trackIndex && track.exists().get()) {
                pendingSoloTrackIndex = -1;
                String trackName = track.name().get();
                host.println("\n[TRACK HOST] SOLO → \"" + trackName + "\" [" + trackIndex + "] = " + (isSoloed ? "SOLOED" : "UNSOLOED") + "\n");
                protocol.send(new protocol.struct.TrackSoloMessage(trackIndex, isSoloed));
            }
        });
    }

    /**
     * Get current track bank based on navigation depth
     * @return siblingTrackBank if nested, mainTrackBank otherwise
     */
    private TrackBank getCurrentBank() {
        return navigationDepth > 0 ? siblingTrackBank : mainTrackBank;
    }

    /**
     * Check if cursor track has a parent group (not just master/root)
     * Uses navigation depth counter instead of parentTrack (which has issues with group master tracks)
     * @return true if inside a track group
     */
    private boolean hasParentGroup() {
        return navigationDepth > 0;
    }

    /**
     * Send track list to controller
     */
    public void sendTrackList() {
        TrackBank bank = getCurrentBank();
        boolean hasParent = hasParentGroup();
        String parentName = hasParent ? parentTrack.name().get() : "";

        // Build list of tracks
        List<TrackListMessage.Tracks> tracks = buildTrackList(bank);

        // Find which track is selected
        int selectedIndex = findSelectedTrackIndex(tracks);

        // Log
        String context = hasParent ? " ↳ " + parentName : " ⌂ root";
        host.println("[TRACK HOST] Tracks: " + tracks.size() + " | Cursor: " + selectedIndex + context);

        // Send immediately (caller handles delay if needed)
        protocol.send(new TrackListMessage(tracks.size(), selectedIndex, hasParent, parentName, tracks));
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
                    trackTypeToInt(track.trackType().get())
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
     * Execute track navigation: enter group
     * Called FROM TrackController when controller sends navigation command
     *
     * Navigation logic is HERE (not in Controller) because:
     * - Needs siblingTrackBank access (can't be shared with Controller)
     * - Bitwig API limitation: siblingTrackBank must be created in Host
     */
    public void enterTrackGroup(int trackIndex) {
        TrackBank currentBank = getCurrentBank();
        Track track = currentBank.getItemAt(trackIndex);

        if (!track.exists().get() || !track.isGroup().get()) {
            host.println("[TRACK HOST] ✗ Enter failed: track " + trackIndex + " is not a group");
            sendTrackList();
            return;
        }

        host.println("[TRACK HOST] ▶ Enter: " + track.name().get());

        // Track navigation depth
        navigationDepth++;

        // Execute Bitwig API navigation
        cursorTrack.selectChannel(track);
        cursorTrack.selectFirstChild();

        // Send track list after delay for Bitwig to update (industry standard: 100ms)
        host.scheduleTask(() -> sendTrackList(), BitwigConfig.TRACK_ENTER_GROUP_MS);
    }

    /**
     * Execute track navigation: exit group
     * Called FROM TrackController when controller sends navigation command
     */
    public void exitTrackGroup() {
        if (!hasParentGroup()) {
            host.println("[TRACK HOST] ✗ Exit failed: already at root");
            sendTrackList();
            return;
        }

        String parentName = parentTrack.name().get();
        host.println("[TRACK HOST] ◀ Exit: " + parentName);

        // Track navigation depth
        navigationDepth--;

        // Execute Bitwig API navigation
        cursorTrack.selectParent();

        // Send track list after delay for Bitwig to update (industry standard: 100ms)
        host.scheduleTask(() -> sendTrackList(), BitwigConfig.TRACK_EXIT_GROUP_MS);
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
            boolean currentState = track.solo().get();
            track.solo().set(!currentState);
            // Observer (addTrackObservers) will send TrackSoloMessage confirmation
        }
    }
}
