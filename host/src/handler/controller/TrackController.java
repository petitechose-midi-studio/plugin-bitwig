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
    private DeviceHost deviceHost;

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

        setupProtocolCallbacks();
    }

    public void setTrackHost(TrackHost trackHost) {
        this.trackHost = trackHost;
    }

    public void setDeviceHost(DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    private void setupProtocolCallbacks() {
        // Request track list FROM controller (legacy - redirect to windowed)
        protocol.onRequestTrackList = msg -> {
            if (msg.fromHost) return;
            host.println("\n[TRACK CTRL] → Requesting track list (redirected to windowed)\n");
            if (trackHost != null) {
                trackHost.sendTrackListWindow(0);  // Redirect legacy to windowed
            }
        };

        // Request windowed track list FROM controller (new)
        protocol.onRequestTrackListWindow = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.sendTrackListWindow(msg.getTrackStartIndex());
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

        // =====================================================================
        // MixView: Volume/Pan/Arm/Sends
        // =====================================================================

        // Select which send to observe for MixView
        protocol.onSelectMixSend = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.setSelectedMixSend(msg.getSendIndex());
            }
        };

        // Request send destinations (effect track names)
        protocol.onRequestSendDestinations = msg -> {
            if (msg.fromHost) return;
            if (trackHost != null) {
                trackHost.sendSendDestinations();
            }
        };

        // Volume change FROM controller
        protocol.onTrackVolumeChange = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.volume().value().set(msg.getVolume());
            }
        };

        // Volume touch FROM controller
        protocol.onTrackVolumeTouch = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.volume().touch(msg.isTouched());
            }
        };

        // Pan change FROM controller
        protocol.onTrackPanChange = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.pan().value().set(msg.getPan());
            }
        };

        // Pan touch FROM controller
        protocol.onTrackPanTouch = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.pan().touch(msg.isTouched());
            }
        };

        // Arm change FROM controller
        protocol.onTrackArmChange = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                track.arm().set(msg.isArm());
            }
        };

        // Send value change FROM controller
        protocol.onTrackSendValueChange = msg -> {
            if (msg.fromHost) return;
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
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                Send send = track.sendBank().getItemAt(msg.getSendIndex());
                if (send.exists().get()) {
                    send.touch(msg.isTouched());
                }
            }
        };

        // Send enabled change FROM controller
        protocol.onTrackSendEnabledChange = msg -> {
            if (msg.fromHost) return;
            Track track = trackBank.getItemAt(msg.getTrackIndex());
            if (track != null && track.exists().get()) {
                Send send = track.sendBank().getItemAt(msg.getSendIndex());
                if (send.exists().get()) {
                    send.isEnabled().set(msg.getSendIsEnabled());
                }
            }
        };

        // Send mode change FROM controller
        protocol.onTrackSendModeChange = msg -> {
            if (msg.fromHost) return;
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
