package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import config.BitwigConfig;
import handler.host.TrackHost;

/**
 * TrackController - Handles Track commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Tracks)
 * - Receives protocol callbacks (protocol.onXXX)
 * - Checks fromHost flag (returns early if true)
 * - Executes Bitwig API actions (select, toggle, etc.)
 * - Delegates navigation to TrackHost (needs siblingTrackBank)
 * - NEVER observes Bitwig API directly
 * - NEVER sends protocol messages (except via Host delegates)
 */
public class TrackController {
    private final ControllerHost host;
    private final CursorTrack cursorTrack;
    private final TrackBank trackBank;
    private final CursorDevice cursorDevice;
    private final DeviceBank deviceBank;
    private final Protocol protocol;
    private TrackHost trackHost;
    private handler.host.DeviceHost deviceHost;

    public TrackController(
        ControllerHost host,
        CursorTrack cursorTrack,
        TrackBank trackBank,
        CursorDevice cursorDevice,
        DeviceBank deviceBank,
        Protocol protocol
    ) {
        this.host = host;
        this.cursorTrack = cursorTrack;
        this.trackBank = trackBank;
        this.cursorDevice = cursorDevice;
        this.deviceBank = deviceBank;
        this.protocol = protocol;

        setupCallbacks();
    }

    public void setTrackHost(TrackHost trackHost) {
        this.trackHost = trackHost;
    }

    public void setDeviceHost(handler.host.DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    private void setupCallbacks() {
        // Request track list FROM controller
        protocol.onRequestTrackList = msg -> {
            if (msg.fromHost) return;
            host.println("\n[TRACK CTRL] → Requesting track list\n");
            if (trackHost != null) {
                trackHost.sendTrackList();
            }
        };

        // Select track by index FROM controller
        protocol.onTrackSelectByIndex = msg -> {
            if (msg.fromHost) return;
            int trackIndex = msg.getTrackIndex();

            // Use TrackHost to get track from correct bank (siblings or main)
            Track track = (trackHost != null) ? trackHost.getTrackAtIndex(trackIndex) : trackBank.getItemAt(trackIndex);

            if (track != null && track.exists().get()) {
                String trackName = track.name().get();
                host.println("\n[TRACK CTRL] SELECT → \"" + trackName + "\" [" + trackIndex + "]\n");

                cursorTrack.selectChannel(track);

                // Select first device when deviceBank is ready, then send device list
                if (deviceHost != null) {
                    deviceHost.scheduleAfterDeviceBankUpdate(() -> {
                        com.bitwig.extension.controller.api.Device firstDevice = deviceBank.getItemAt(0);
                        if (firstDevice.exists().get()) {
                            cursorDevice.selectDevice(firstDevice);
                        }
                        // Send device list after another delay for device selection to complete
                        host.scheduleTask(() -> deviceHost.sendDeviceList(), BitwigConfig.DEVICE_ENTER_CHILD_MS);
                    });
                }
            }
        };

        // Enter track group FROM controller
        protocol.onEnterTrackGroup = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.enterTrackGroup(msg.getTrackIndex());
            }
        };

        // Exit track group FROM controller
        protocol.onExitTrackGroup = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.exitTrackGroup();
            }
        };

        // Toggle track mute FROM controller
        protocol.onTrackMute = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.toggleMute(msg.getTrackIndex());
                // Confirmation will be sent by observer in TrackHost
            }
        };

        // Toggle track solo FROM controller
        protocol.onTrackSolo = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.toggleSolo(msg.getTrackIndex());
                // Confirmation will be sent by observer in TrackHost
            }
        };

        // Toggle track activated FROM controller
        protocol.onTrackActivate = msg -> {
            if (msg.fromHost) return;

            // Use TrackHost to get track from correct bank (siblings or main)
            Track track = (trackHost != null) ? trackHost.getTrackAtIndex(msg.getTrackIndex()) : trackBank.getItemAt(msg.getTrackIndex());

            if (track != null && track.exists().get()) {
                track.isActivated().toggle();
            }
        };
    }
}
