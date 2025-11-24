package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import config.BitwigConfig;
import handler.host.TrackHost;
import handler.util.ObserverBasedRequestHandler;

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
    private final ObserverBasedRequestHandler requestHandler;
    private TrackHost trackHost;
    private handler.host.DeviceHost deviceHost;
    private DeviceController deviceController;

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
        this.requestHandler = new ObserverBasedRequestHandler(host);

        // Register contexts for observer-based requests
        requestHandler.registerContext("trackList", trackBank.itemCount());

        setupCallbacks();
    }

    public void setTrackHost(TrackHost trackHost) {
        this.trackHost = trackHost;
    }

    public void setDeviceHost(handler.host.DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    public void setDeviceController(DeviceController deviceController) {
        this.deviceController = deviceController;
    }

    private void setupCallbacks() {
        // Request track list FROM controller
        protocol.onRequestTrackList = msg -> {
            if (msg.fromHost) return;
            host.println("\n[TRACK CTRL] → Requesting track list\n");
            if (trackHost != null) {
                requestHandler.requestSend("trackList", () -> trackHost.sendTrackList());
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

                // Notify DeviceController that deviceBank will be updating
                if (deviceController != null) {
                    deviceController.notifyTrackChangePending();
                }

                cursorTrack.selectChannel(track);

                // Select first device when deviceBank is ready (using DeviceHost's observer)
                if (deviceHost != null) {
                    deviceHost.getRequestHandler().requestSend("deviceList", () -> {
                        com.bitwig.extension.controller.api.Device firstDevice = deviceBank.getItemAt(0);
                        if (firstDevice.exists().get()) {
                            cursorDevice.selectDevice(firstDevice);
                        }
                    });
                }

                // NOTE: DeviceHost observers will send device updates automatically
                // Full sendDeviceList() only called when track selector is released
            }
        };

        // Enter track group FROM controller
        protocol.onEnterTrackGroup = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                requestHandler.notifyChangePending("trackList");
                trackHost.enterTrackGroup(msg.getTrackIndex());
            }
        };

        // Exit track group FROM controller
        protocol.onExitTrackGroup = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                requestHandler.notifyChangePending("trackList");
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
