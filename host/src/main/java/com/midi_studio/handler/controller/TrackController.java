package com.midi_studio.handler.controller;

import com.bitwig.extension.controller.api.*;
import com.midi_studio.protocol.Protocol;
import com.midi_studio.config.BitwigConfig;
import com.midi_studio.handler.host.TrackHost;

/**
 * TrackController - Handles Track commands FROM controller
 *
 * Listens to protocol callbacks and executes actions on Bitwig Tracks.
 *
 * SINGLE RESPONSIBILITY: Controller → Bitwig (Tracks)
 */
public class TrackController {
    private final ControllerHost host;
    private final CursorTrack cursorTrack;
    private final TrackBank trackBank;
    private final CursorDevice cursorDevice;
    private final DeviceBank deviceBank;
    private final Protocol protocol;
    private TrackHost trackHost;
    private com.midi_studio.handler.host.DeviceHost deviceHost;

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

    public void setDeviceHost(com.midi_studio.handler.host.DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    private void setupCallbacks() {
        // Request track list FROM controller
        protocol.onRequestTrackList = msg -> {
            if (msg.fromHost) return;
            host.println("[TrackController] Received REQUEST_TRACK_LIST from controller");
            if (trackHost != null) {
                trackHost.sendTrackList();
            }
        };

        // Select track by index FROM controller
        protocol.onTrackSelectByIndex = msg -> {
            if (msg.fromHost) return;
            int trackIndex = msg.getTrackIndex();
            Track track = trackBank.getItemAt(trackIndex);
            if (track.exists().get()) {
                cursorTrack.selectChannel(track);
                host.scheduleTask(() -> {
                    com.bitwig.extension.controller.api.Device firstDevice = deviceBank.getItemAt(0);
                    if (firstDevice.exists().get()) {
                        cursorDevice.selectDevice(firstDevice);}

                }, BitwigConfig.CURSOR_UPDATE_DELAY_MS);
                
                host.scheduleTask(() -> {
                    if (deviceHost != null) {
                        deviceHost.sendDeviceList();
                    }
                }, BitwigConfig.CURSOR_UPDATE_DELAY_MS*2);
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
            int trackIndex = msg.getTrackIndex();
            Track track = trackBank.getItemAt(trackIndex);
            if (track.exists().get()) {
                track.mute().toggle();

                // Send updated state back to controller after toggle completes
                host.scheduleTask(() -> {
                    boolean newMuteState = track.mute().get();
                    protocol.send(new com.midi_studio.protocol.struct.TrackMuteMessage(
                        trackIndex,
                        newMuteState
                    ));
                }, BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        };

        // Toggle track solo FROM controller
        protocol.onTrackSolo = msg -> {
            if (msg.fromHost) return;
            int trackIndex = msg.getTrackIndex();
            Track track = trackBank.getItemAt(trackIndex);
            if (track.exists().get()) {
                track.solo().toggle();

                // Send updated state back to controller after toggle completes
                host.scheduleTask(() -> {
                    boolean newSoloState = track.solo().get();
                    protocol.send(new com.midi_studio.protocol.struct.TrackSoloMessage(
                        trackIndex,
                        newSoloState
                    ));
                }, BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        };

        // Toggle track activated FROM controller
        protocol.onTrackActivate = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track.exists().get()) {
                track.isActivated().toggle();
            }
        };
    }
}
