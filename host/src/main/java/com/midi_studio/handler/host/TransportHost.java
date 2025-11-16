package com.midi_studio.handler.host;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import com.midi_studio.protocol.Protocol;
import com.midi_studio.protocol.struct.*;

/**
 * TransportHost - Observes Bitwig Transport and sends updates TO controller
 *
 * Monitors transport state changes (play/stop/record) and sends
 * protocol messages to hardware controller.
 *
 * SINGLE RESPONSIBILITY: Bitwig → Controller (Transport)
 */
public class TransportHost {
    private final Protocol protocol;
    private final Transport transport;

    public TransportHost(
        ControllerHost host,
        Protocol protocol,
        Transport transport
    ) {
        // host parameter kept for future logging/debug needs
        this.protocol = protocol;
        this.transport = transport;
    }

    /**
     * Setup transport state observers
     */
    public void setup() {
        // Mark all observables as interested (required before .get() calls)
        transport.isPlaying().markInterested();
        transport.isArrangerRecordEnabled().markInterested();

        // Observe play/stop state changes
        transport.isPlaying().addValueObserver(isPlaying -> {
            protocol.send(new TransportPlayMessage(isPlaying));
        });

        // Observe record state changes
        transport.isArrangerRecordEnabled().addValueObserver(isRecording -> {
            protocol.send(new TransportRecordMessage(isRecording));
        });
    }

    /**
     * Send current transport state (called at startup)
     */
    public void sendInitialState() {
        protocol.send(new TransportPlayMessage(transport.isPlaying().get()));
        protocol.send(new TransportRecordMessage(
            transport.isArrangerRecordEnabled().get()
        ));
    }
}
