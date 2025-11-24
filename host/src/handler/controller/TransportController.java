package handler.controller;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import protocol.Protocol;

/**
 * TransportController - Handles Transport commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Transport)
 * - Receives protocol callbacks (protocol.onXXX)
 * - Executes Bitwig API actions (play, stop, record, tempo changes)
 * - NEVER observes Bitwig API directly
 * - NEVER sends protocol messages (confirmations come from TransportHost observers)
 */
public class TransportController {
    private final ControllerHost host;
    private final Transport transport;
    private final Protocol protocol;

    public TransportController(
        ControllerHost host,
        Transport transport,
        Protocol protocol
    ) {
        this.host = host;
        this.transport = transport;
        this.protocol = protocol;

        setupCallbacks();
    }

    private void setupCallbacks() {
        protocol.onTransportPlay = msg -> {
            if (msg.isPlaying()) {
                transport.play();
            } else {
                transport.stop();
            }
        };

        protocol.onTransportRecord = msg -> {
            transport.record();
        };

        protocol.onTransportStop = msg -> {
            transport.stop();
        };

        protocol.onTransportTempo = msg -> {
            if (msg.fromHost) {
                // Set absolute tempo from host
                transport.tempo().setRaw(msg.getTempo());
                host.println("[TRANSPORT] Tempo: " + transport.tempo().getRaw() + " BPM ← host");
            } else {
                // Increment tempo from controller
                transport.tempo().incRaw(msg.getTempo());
                host.println("[TRANSPORT] Tempo: " + transport.tempo().getRaw() + " BPM (Δ" + msg.getTempo() + ")");
            }
        };
    }
}
