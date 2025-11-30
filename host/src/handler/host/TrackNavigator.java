package handler.host;

import com.bitwig.extension.controller.api.*;

/**
 * TrackNavigator - Handles track group navigation
 *
 * RESPONSIBILITY: Navigate track groups (enter/exit)
 * - Tracks navigation depth (root = 0, inside group = 1+)
 * - Manages current bank selection (main vs sibling)
 * - Executes Bitwig API navigation
 *
 * NOTE: Separated from TrackHost for single responsibility.
 * Consistency with DeviceNavigator pattern.
 */
public class TrackNavigator {
    private final ControllerHost host;
    private final CursorTrack cursorTrack;
    private final TrackBank mainTrackBank;
    private final TrackBank siblingTrackBank;
    private final Track parentTrack;

    // Navigation depth: 0 = root level, >0 = inside group(s)
    private int navigationDepth = 0;

    public TrackNavigator(
        ControllerHost host,
        CursorTrack cursorTrack,
        TrackBank mainTrackBank,
        TrackBank siblingTrackBank,
        Track parentTrack
    ) {
        this.host = host;
        this.cursorTrack = cursorTrack;
        this.mainTrackBank = mainTrackBank;
        this.siblingTrackBank = siblingTrackBank;
        this.parentTrack = parentTrack;
    }

    /**
     * Get current track bank based on navigation depth
     * @return siblingTrackBank if nested, mainTrackBank otherwise
     */
    public TrackBank getCurrentBank() {
        return navigationDepth > 0 ? siblingTrackBank : mainTrackBank;
    }

    /**
     * Check if currently inside a track group
     * @return true if inside a group (navigationDepth > 0)
     */
    public boolean hasParentGroup() {
        return navigationDepth > 0;
    }

    /**
     * Get parent track name (for display)
     * @return parent track name, or empty string if at root
     */
    public String getParentName() {
        return hasParentGroup() ? parentTrack.name().get() : "";
    }

    /**
     * Enter track group at given index
     * @param trackIndex Index of group track to enter
     * @param onComplete Callback after navigation (for list refresh)
     */
    public void enterTrackGroup(int trackIndex, Runnable onComplete) {
        TrackBank currentBank = getCurrentBank();
        Track track = currentBank.getItemAt(trackIndex);

        if (!track.exists().get() || !track.isGroup().get()) {
            host.println("[TRACK NAV] ✗ Enter failed: track " + trackIndex + " is not a group");
            if (onComplete != null) onComplete.run();
            return;
        }

        host.println("[TRACK NAV] ▶ Enter: " + track.name().get());
        navigationDepth++;

        cursorTrack.selectChannel(track);
        cursorTrack.selectFirstChild();
        // itemCount observer will trigger list refresh
    }

    /**
     * Exit current track group to parent
     * @param onComplete Callback after navigation (for list refresh)
     */
    public void exitTrackGroup(Runnable onComplete) {
        if (!hasParentGroup()) {
            host.println("[TRACK NAV] ✗ Exit failed: already at root");
            if (onComplete != null) onComplete.run();
            return;
        }

        host.println("[TRACK NAV] ◀ Exit: " + parentTrack.name().get());
        navigationDepth--;

        cursorTrack.selectParent();
        // itemCount observer will trigger list refresh
    }
}
