package handler.controller;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import protocol.Protocol;

/**
 * TransportController - Handles Transport commands FROM controller
 *
 * Listens to protocol callbacks and executes actions on Bitwig Transport.
 *
 * SINGLE RESPONSIBILITY: Controller → Bitwig (Transport)
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
            float tempo = msg.getTempo();
            host.println("TransportController: received tempo " + tempo + " BPM, setting...");
            transport.tempo().setRaw(tempo);
            host.println("TransportController: tempo set to " + transport.tempo().getRaw() + " BPM");
        };
    }
}
