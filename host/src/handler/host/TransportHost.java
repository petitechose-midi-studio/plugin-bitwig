package handler.host;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;

/**
 * TransportHost - Observes Bitwig Transport and sends updates TO controller
 *
 * RESPONSIBILITY: Bitwig → Controller (Transport)
 * - Observes transport changes (play, stop, record, tempo)
 * - Sends protocol messages when changes occur
 * - NEVER receives protocol callbacks (that's TransportController's job)
 * - NEVER executes Bitwig actions (that's TransportController's job)
 */
public class TransportHost {
    private final ControllerHost host;
    private final Protocol protocol;
    private final Transport transport;

    public TransportHost(
        ControllerHost host,
        Protocol protocol,
        Transport transport
    ) {
        this.host = host;
        this.protocol = protocol;
        this.transport = transport;
    }

    /**
     * Setup transport state observers
     */
    public void setupObservers() {
        transport.isPlaying().markInterested();
        transport.isArrangerRecordEnabled().markInterested();
        transport.tempo().markInterested();

        transport.isPlaying().addValueObserver(isPlaying -> {
            protocol.send(new TransportPlayMessage(isPlaying));
        });

        transport.isArrangerRecordEnabled().addValueObserver(isRecording -> {
            protocol.send(new TransportRecordMessage(isRecording));
        });

        transport.tempo().value().addRawValueObserver(tempo -> {
            protocol.send(new TransportTempoMessage((float) tempo));
        });
    }

    /**
     * Send current transport state (called at startup)
     */
    public void sendInitialState() {
        boolean isPlaying = transport.isPlaying().get();
        boolean isRecording = transport.isArrangerRecordEnabled().get();
        double tempo = transport.tempo().getRaw();

        String state = (isPlaying ? "▶" : "⏸") + (isRecording ? " ●" : "");
        host.println("[TRANSPORT HOST] Init: " + state + " | " + tempo + " BPM");

        protocol.send(new TransportPlayMessage(isPlaying));
        protocol.send(new TransportRecordMessage(isRecording));
        protocol.send(new TransportTempoMessage((float) tempo));
    }
}
