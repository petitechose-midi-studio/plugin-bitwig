package handler.host;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;

/**
 * TransportHost - Observes Bitwig Transport and sends updates TO controller
 *
 * Monitors transport state changes (play/stop/record) and sends
 * protocol messages to hardware controller.
 *
 * SINGLE RESPONSIBILITY: Bitwig → Controller (Transport)
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
    public void setup() {
        transport.isPlaying().markInterested();
        transport.isArrangerRecordEnabled().markInterested();
        transport.tempo().markInterested();

        transport.isPlaying().addValueObserver(isPlaying -> {
            final boolean state = isPlaying;
            host.scheduleTask(() -> {
                protocol.send(new TransportPlayMessage(state));
            }, BitwigConfig.SINGLE_ELEMENT_DELAY_MS);
        });

        transport.isArrangerRecordEnabled().addValueObserver(isRecording -> {
            final boolean state = isRecording;
            host.scheduleTask(() -> {
                protocol.send(new TransportRecordMessage(state));
            }, BitwigConfig.SINGLE_ELEMENT_DELAY_MS);
        });

        transport.tempo().value().addRawValueObserver(tempo -> {
            final float tempoValue = (float) tempo;
            host.scheduleTask(() -> {
                protocol.send(new TransportTempoMessage(tempoValue));
            }, BitwigConfig.SINGLE_ELEMENT_DELAY_MS);
        });
    }

    /**
     * Send current transport state (called at startup)
     */
    public void sendInitialState() {
        host.scheduleTask(() -> {
            boolean isPlaying = transport.isPlaying().get();
            boolean isRecording = transport.isArrangerRecordEnabled().get();
            double tempo = transport.tempo().getRaw();

            String state = (isPlaying ? "▶" : "⏸") + (isRecording ? " ●" : "");
            host.println("[TRANSPORT HOST] Init: " + state + " | " + tempo + " BPM");

            protocol.send(new TransportPlayMessage(isPlaying));
            protocol.send(new TransportRecordMessage(isRecording));
            protocol.send(new TransportTempoMessage((float) tempo));
        }, BitwigConfig.INITIAL_STATE_SEND_DELAY_MS);
    }
}
