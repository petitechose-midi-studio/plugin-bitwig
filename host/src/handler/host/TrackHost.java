package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;
import util.ColorUtils;
import java.util.List;
import java.util.ArrayList;
import java.util.Stack;

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
            Track track = mainTrackBank.getItemAt(i);
            track.exists().markInterested();
            track.name().markInterested();
            track.color().markInterested();
            track.position().markInterested();
            track.isActivated().markInterested();
            track.mute().markInterested();
            track.solo().markInterested();
            track.isGroup().markInterested();
        }

        // Mark parent track observables as interested
        parentTrack.exists().markInterested();
        parentTrack.name().markInterested();
        parentTrack.isGroup().markInterested();

        // Mark sibling track bank observables as interested
        siblingTrackBank.itemCount().markInterested();
        for (int i = 0; i < 32; i++) {
            Track track = siblingTrackBank.getItemAt(i);
            track.exists().markInterested();
            track.name().markInterested();
            track.color().markInterested();
            track.position().markInterested();
            track.isActivated().markInterested();
            track.mute().markInterested();
            track.solo().markInterested();
            track.isGroup().markInterested();
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
     */
    public void sendInitialState() {
        // Send initial track list
        sendTrackList();
    }

    /**
     * Send track list to controller with hierarchical navigation info
     * Called by TrackController when REQUEST_TRACK_LIST received
     */
    public void sendTrackList() {
        // Automatically detect if cursor track has a parent GROUP (not just technical parent)
        boolean hasParent = parentTrack.exists().get() && parentTrack.isGroup().get();

        // Use siblingTrackBank if track has parent, mainTrackBank otherwise
        TrackBank currentBank = hasParent ? siblingTrackBank : mainTrackBank;

        int totalTrackCount = currentBank.itemCount().get();
        int currentTrackPosition = cursorTrack.position().get();
        String parentGroupName = hasParent ? parentTrack.name().get() : "";

        host.println("\n[TRACK HOST] Sending track list: " + totalTrackCount + " tracks (cursor=" + currentTrackPosition + ", hasParent=" + hasParent + ", parent=" + parentGroupName + ")\n");

        // Build list of tracks in current bank
        List<TrackListMessage.Tracks> tracksList = new ArrayList<>();
        int bankSize = currentBank.getSizeOfBank();

        for (int i = 0; i < bankSize; i++) {
            Track track = currentBank.getItemAt(i);

            if (track.exists().get()) {
                String name = track.name().get();
                long color = ColorUtils.toUint32Hex(track.color().get());
                int position = track.position().get();
                boolean isActivated = track.isActivated().get();
                boolean isMute = track.mute().get();
                boolean isSolo = track.solo().get();
                boolean isGroup = track.isGroup().get();

                tracksList.add(new TrackListMessage.Tracks(
                    position,
                    name,
                    color,
                    isActivated,
                    isMute,
                    isSolo,
                    isGroup
                ));
            }
        }

        // Find current track index in the list
        int currentTrackIndex = 0;
        for (int i = 0; i < tracksList.size(); i++) {
            if (tracksList.get(i).getTrackIndex() == currentTrackPosition) {
                currentTrackIndex = i;
                break;
            }
        }

        protocol.send(new TrackListMessage(
            totalTrackCount,
            currentTrackIndex,
            hasParent,
            parentGroupName,
            tracksList
        ));
    }

    /**
     * Enter a track group (navigate into group's children)
     * Called by TrackController when ENTER_TRACK_GROUP received
     *
     * @param trackIndex Index in current bank
     */
    public void enterTrackGroup(int trackIndex) {
        boolean hasParent = parentTrack.exists().get() && parentTrack.isGroup().get();
        TrackBank currentBank = hasParent ? siblingTrackBank : mainTrackBank;
        Track track = currentBank.getItemAt(trackIndex);

        if (!track.exists().get() || !track.isGroup().get()) {
            host.println("\n[TRACK HOST] ⚠ Cannot enter track " + trackIndex + " (not a group or doesn't exist)\n");
            sendTrackList();
            return;
        }

        host.println("\n[TRACK HOST] ▶ Entering group: " + track.name().get() + "\n");

        // Select the group track, then select its first child
        cursorTrack.selectChannel(track);

        // Wait for Bitwig to update, then select first child
        host.scheduleTask(() -> {
            cursorTrack.selectFirstChild();
            // Wait again before sending track list
            host.scheduleTask(() -> sendTrackList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
        }, BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    /**
     * Exit track group (navigate back to parent)
     * Called by TrackController when EXIT_TRACK_GROUP received
     */
    public void exitTrackGroup() {
        boolean hasParent = parentTrack.exists().get() && parentTrack.isGroup().get();

        if (!hasParent) {
            host.println("\n[TRACK HOST] ⚠ Already at root level\n");
            sendTrackList();
            return;
        }

        String parentName = parentTrack.name().get();
        host.println("\n[TRACK HOST] ◀ Exiting to parent: " + parentName + "\n");

        // Select the parent track
        cursorTrack.selectParent();

        // Wait for Bitwig to update context, then send list
        host.scheduleTask(() -> sendTrackList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    /**
     * Get track at index from current context (siblings or main bank)
     * Called by TrackController to get correct track based on current navigation level
     *
     * @param trackIndex Index in current bank
     * @return Track at index, or null if doesn't exist
     */
    public Track getTrackAtIndex(int trackIndex) {
        boolean hasParent = parentTrack.exists().get() && parentTrack.isGroup().get();
        TrackBank currentBank = hasParent ? siblingTrackBank : mainTrackBank;
        return currentBank.getItemAt(trackIndex);
    }
}
