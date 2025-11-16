package com.midi_studio.handler.controller;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import com.midi_studio.protocol.Protocol;

/**
 * TransportController - Handles Transport commands FROM controller
 *
 * Listens to protocol callbacks and executes actions on Bitwig Transport.
 *
 * SINGLE RESPONSIBILITY: Controller → Bitwig (Transport)
 */
public class TransportController {
    private final Transport transport;
    private final Protocol protocol;

    public TransportController(
        ControllerHost host,
        Transport transport,
        Protocol protocol
    ) {
        // host parameter kept for future logging/debug needs
        this.transport = transport;
        this.protocol = protocol;

        setupCallbacks();
    }

    private void setupCallbacks() {
        // Receive TransportPlay command FROM controller
        protocol.onTransportPlay = msg -> {
            if (msg.isPlaying()) {
                transport.play();
            } else {
                transport.stop();
            }
        };

        // Receive TransportRecord command FROM controller
        protocol.onTransportRecord = msg -> {
            transport.record();
        };
        
        // Receive TransportRecord command FROM controller
        protocol.onTransportStop = msg -> {
            transport.stop();
        };
    }
}
