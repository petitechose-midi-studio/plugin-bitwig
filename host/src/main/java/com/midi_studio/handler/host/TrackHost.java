package com.midi_studio.handler.host;

import com.bitwig.extension.controller.api.*;
import com.midi_studio.protocol.Protocol;
import com.midi_studio.protocol.struct.*;
import com.midi_studio.config.BitwigConfig;
import com.midi_studio.util.ColorUtils;
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
    private final TrackBank cursorGroupBank;  // Pre-created bank for group navigation

    // Navigation state
    private boolean isInGroup = false;

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

        // Create a TrackBank for group navigation (follows cursorTrack's children)
        // This must be created during init, not dynamically
        this.cursorGroupBank = cursorTrack.createTrackBank(32, 0, 0, false);
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

        // Mark cursor group bank observables as interested
        cursorGroupBank.itemCount().markInterested();
        for (int i = 0; i < 32; i++) {
            Track track = cursorGroupBank.getItemAt(i);
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
        // Use cursorGroupBank if in group, mainTrackBank otherwise
        TrackBank currentBank = isInGroup ? cursorGroupBank : mainTrackBank;

        int totalTrackCount = currentBank.itemCount().get();
        int currentTrackPosition = cursorTrack.position().get();
        String parentGroupName = "";

        host.println("[TrackHost] sendTrackList: totalCount=" + totalTrackCount + ", cursorPos=" + currentTrackPosition + ", isInGroup=" + isInGroup);

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

        host.println("[TrackHost] Sending TRACK_LIST: " + tracksList.size() + " tracks, currentIndex=" + currentTrackIndex);
        for (int i = 0; i < tracksList.size(); i++) {
            TrackListMessage.Tracks t = tracksList.get(i);
            host.println("  [" + i + "] " + t.getTrackName() + " (act=" + t.isActivated() + ", mute=" + t.isMute() + ", solo=" + t.isSolo() + ", group=" + t.isGroup() + ")");
        }

        protocol.send(new TrackListMessage(
            totalTrackCount,
            currentTrackIndex,
            isInGroup,
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
        TrackBank currentBank = isInGroup ? cursorGroupBank : mainTrackBank;
        Track track = currentBank.getItemAt(trackIndex);

        if (!track.exists().get() || !track.isGroup().get()) {
            host.println("[TrackHost] enterTrackGroup: track " + trackIndex + " is not a group or doesn't exist");
            sendTrackList();
            return;
        }

        host.println("[TrackHost] enterTrackGroup: entering group " + track.name().get());

        // Select the group track (cursorGroupBank will automatically follow its children)
        cursorTrack.selectChannel(track);

        // Switch to group mode
        isInGroup = true;

        // Wait for Bitwig to update cursorGroupBank, then send list
        host.scheduleTask(() -> sendTrackList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    /**
     * Exit track group (navigate back to parent)
     * Called by TrackController when EXIT_TRACK_GROUP received
     */
    public void exitTrackGroup() {
        if (!isInGroup) {
            host.println("[TrackHost] exitTrackGroup: already at root");
            sendTrackList();
            return;
        }

        host.println("[TrackHost] exitTrackGroup: returning to main track bank");

        // Switch back to main track bank
        isInGroup = false;

        // Wait for Bitwig to update context, then send list
        host.scheduleTask(() -> sendTrackList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }
}
