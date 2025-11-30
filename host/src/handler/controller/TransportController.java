package handler.controller;

import com.bitwig.extension.controller.api.Transport;
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
    private final Transport transport;
    private final Protocol protocol;

    public TransportController(Transport transport, Protocol protocol) {
        this.transport = transport;
        this.protocol = protocol;
        setupProtocolCallbacks();
    }

    private void setupProtocolCallbacks() {
        protocol.onTransportPlay = msg -> {
            if (msg.fromHost) return;
            if (msg.isPlaying()) {
                transport.play();
            } else {
                transport.stop();
            }
        };

        protocol.onTransportRecord = msg -> {
            if (msg.fromHost) return;
            transport.record();
        };

        protocol.onTransportStop = msg -> {
            if (msg.fromHost) return;
            transport.stop();
        };

        protocol.onTransportTempo = msg -> {
            if (msg.fromHost) return;
            transport.tempo().incRaw(msg.getTempo());
        };
    }
}
