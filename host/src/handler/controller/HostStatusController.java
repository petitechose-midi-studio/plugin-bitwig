package handler.controller;

import protocol.Protocol;
import handler.host.*;

/**
 * HostStatusController - Handles host status requests FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Host Status)
 * - Receives protocol callback (onRequestHostStatus)
 * - Triggers all Hosts to send their initial state
 * - Acts as coordinator for full state sync
 */
public class HostStatusController {
    private final Protocol protocol;
    private final TransportHost transportHost;
    private final DeviceHost deviceHost;
    private final TrackHost trackHost;
    private final LastClickedHost lastClickedHost;

    public HostStatusController(
        Protocol protocol,
        TransportHost transportHost,
        DeviceHost deviceHost,
        TrackHost trackHost,
        LastClickedHost lastClickedHost
    ) {
        this.protocol = protocol;
        this.transportHost = transportHost;
        this.deviceHost = deviceHost;
        this.trackHost = trackHost;
        this.lastClickedHost = lastClickedHost;

        setupProtocolCallbacks();
    }

    private void setupProtocolCallbacks() {
        protocol.onRequestHostStatus = msg -> {
            sendFullState();
        };
    }

    /**
     * Send full host state to controller
     * Called on initial connection and on resync request
     */
    public void sendFullState() {
        protocol.hostInitialized(true);
        transportHost.sendInitialState();
        deviceHost.sendInitialState();
        trackHost.sendInitialState();
        lastClickedHost.sendInitialState();
    }
}
