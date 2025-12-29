package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import handler.host.TrackHost;
import handler.host.DeviceHost;

/**
 * TrackController - Handles Track commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Tracks)
 * - Receives protocol callbacks (protocol.onXXX)
 * - Executes Bitwig API actions (select, toggle, etc.)
 * - Delegates navigation to TrackHost (needs siblingTrackBank)
 * - NEVER observes Bitwig API directly
 * - NEVER sends protocol messages (except via Host delegates)
 */
public class TrackController {
    private final CursorTrack cursorTrack;
    private final TrackBank trackBank;
    private final CursorDevice cursorDevice;
    private final DeviceBank deviceBank;
    private final Protocol protocol;
    private TrackHost trackHost;
    private DeviceHost deviceHost;

    public TrackController(
        ControllerHost host,
        CursorTrack cursorTrack,
        TrackBank trackBank,
        CursorDevice cursorDevice,
        DeviceBank deviceBank,
        Protocol protocol
    ) {
        this.cursorTrack = cursorTrack;
        this.trackBank = trackBank;
        this.cursorDevice = cursorDevice;
        this.deviceBank = deviceBank;
        this.protocol = protocol;

        setupProtocolCallbacks();
    }

    public void setTrackHost(TrackHost trackHost) {
        this.trackHost = trackHost;
    }

    public void setDeviceHost(DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    private void setupProtocolCallbacks() {
        // Request windowed track list FROM controller
        protocol.onRequestTrackListWindow = msg -> {
            if (trackHost != null) {
                trackHost.sendTrackListWindow(msg.getTrackStartIndex());
            }
        };

        // Select track by index FROM controller
        protocol.onTrackSelect = msg -> {
            int trackIndex = msg.getTrackIndex();

            // Use TrackHost to get track from correct bank (siblings or main)
            Track track = (trackHost != null) ? trackHost.getTrackAtIndex(trackIndex) : trackBank.getItemAt(trackIndex);

            if (track != null && track.exists().get()) {
                cursorTrack.selectChannel(track);

                // Select first device when deviceBank is ready
                if (deviceHost != null) {
                    deviceHost.scheduleAfterDeviceBankUpdate(() -> {
                        com.bitwig.extension.controller.api.Device firstDevice = deviceBank.getItemAt(0);
                        if (firstDevice.exists().get()) {
                            cursorDevice.selectDevice(firstDevice);
                        }
                        // itemCount observer will trigger sendDeviceList() with debounce
                    });
                }
            }
        };

        // Enter track group FROM controller
        protocol.onEnterTrackGroup = msg -> {
            if (trackHost != null) {
                trackHost.enterTrackGroup(msg.getTrackIndex());
            }
        };

        // Exit track group FROM controller
        protocol.onExitTrackGroup = msg -> {
            if (trackHost != null) {
                trackHost.exitTrackGroup();
            }
        };

        // Toggle track mute FROM controller
        protocol.onTrackMute = msg -> {
            if (trackHost != null) {
                trackHost.toggleMute(msg.getTrackIndex());
                // Confirmation will be sent by observer in TrackHost
            }
        };

        // Toggle track solo FROM controller
        protocol.onTrackSolo = msg -> {
            if (trackHost != null) {
                trackHost.toggleSolo(msg.getTrackIndex());
                // Confirmation will be sent by observer in TrackHost
            }
        };

        // Toggle track activated FROM controller
        protocol.onTrackActivate = msg -> {
            // Use TrackHost to get track from correct bank (siblings or main)
            Track track = (trackHost != null) ? trackHost.getTrackAtIndex(msg.getTrackIndex()) : trackBank.getItemAt(msg.getTrackIndex());

            if (track != null && track.exists().get()) {
                track.isActivated().toggle();
            }
        };

        // =====================================================================
        // MixView: Volume/Pan/Arm/Sends
        // =====================================================================

        // Select which send to observe for MixView
        protocol.onSelectMixSend = msg -> {
            if (trackHost != null) {
                trackHost.setSelectedMixSend(msg.getSendIndex());
            }
        };

        // Request send destinations (effect track names)
        protocol.onRequestSendDestinations = msg -> {
            if (trackHost != null) {
                trackHost.sendSendDestinations();
            }
        };

        // Volume change FROM controller
        protocol.onTrackVolume = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.volume().value().set(msg.getVolume());
            }
        };

        // Volume touch FROM controller
        protocol.onTrackVolumeTouch = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.volume().touch(msg.isTouched());
            }
        };

        // Pan change FROM controller
        protocol.onTrackPan = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.pan().value().set(msg.getPan());
            }
        };

        // Pan touch FROM controller
        protocol.onTrackPanTouch = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.pan().touch(msg.isTouched());
            }
        };

        // Arm change FROM controller
        protocol.onTrackArm = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.arm().set(msg.isArm());
            }
        };

        // Send value change FROM controller
        protocol.onTrackSendValue = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                Send send = track.sendBank().getItemAt(msg.getSendIndex());
                if (send.exists().get()) {
                    send.value().set(msg.getSendValue());
                }
            }
        };

        // Send touch FROM controller
        protocol.onTrackSendTouch = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                Send send = track.sendBank().getItemAt(msg.getSendIndex());
                if (send.exists().get()) {
                    send.touch(msg.isTouched());
                }
            }
        };

        // Send enabled change FROM controller
        protocol.onTrackSendEnabled = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                Send send = track.sendBank().getItemAt(msg.getSendIndex());
                if (send.exists().get()) {
                    send.isEnabled().set(msg.getSendIsEnabled());
                }
            }
        };

        // Send mode change FROM controller
        protocol.onTrackSendMode = msg -> {
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                Send send = track.sendBank().getItemAt(msg.getSendIndex());
                if (send.exists().get()) {
                    send.sendMode().set(msg.getSendMode());
                }
            }
        };
    }
}
