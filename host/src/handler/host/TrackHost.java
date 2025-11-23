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
 * Monitors track state changes (name, mute, solo, activated) and sends
 * protocol messages to hardware controller.
 *
 * SINGLE RESPONSIBILITY: Bitwig → Controller (Tracks)
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

        // Mark all tracks in bank as interested (32 slots)
        for (int i = 0; i < 32; i++) {
            markTrackInterested(mainTrackBank.getItemAt(i));
        }

        // Mark parent track observables as interested
        parentTrack.exists().markInterested();
        parentTrack.name().markInterested();

        // Mark sibling track bank observables as interested
        siblingTrackBank.itemCount().markInterested();
        for (int i = 0; i < 32; i++) {
            markTrackInterested(siblingTrackBank.getItemAt(i));
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
        final TrackBank bank = getCurrentBank();
        final boolean hasParent = hasParentGroup();
        final String parentName = hasParent ? parentTrack.name().get() : "";

        // Build list of tracks
        final List<TrackListMessage.Tracks> tracks = buildTrackList(bank);

        // Find which track is selected
        final int selectedIndex = findSelectedTrackIndex(tracks);

        // Log
        String context = hasParent ? " ↳ " + parentName : " ⌂ root";
        host.println("[TRACK HOST] Tracks: " + tracks.size() + " | Cursor: " + selectedIndex + context);

        host.scheduleTask(() -> {
            protocol.send(new TrackListMessage(tracks.size(), selectedIndex, hasParent, parentName, tracks));
        }, BitwigConfig.LIST_OPERATION_DELAY_MS);
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
                    track.isGroup().get()
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
     * Enter a track group (navigate into group's children)
     * Called by TrackController when ENTER_TRACK_GROUP received
     *
     * @param trackIndex Index in current bank
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

        // Increment navigation depth
        navigationDepth++;

        // Select the group track, then select its first child
        cursorTrack.selectChannel(track);

        // Single element operation: select first child
        host.scheduleTask(() -> cursorTrack.selectFirstChild(), BitwigConfig.SINGLE_ELEMENT_DELAY_MS);

        // List operation: refresh track list
        sendTrackList();
    }

    /**
     * Exit track group (navigate back to parent)
     * Called by TrackController when EXIT_TRACK_GROUP received
     */
    public void exitTrackGroup() {
        if (!hasParentGroup()) {
            host.println("[TRACK HOST] ✗ Exit failed: already at root");
            sendTrackList();
            return;
        }

        String parentName = parentTrack.name().get();
        host.println("[TRACK HOST] ◀ Exit: " + parentName);

        // Decrement navigation depth
        navigationDepth--;

        // Select the parent track
        cursorTrack.selectParent();

        // List operation: refresh track list
        sendTrackList();
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
}
