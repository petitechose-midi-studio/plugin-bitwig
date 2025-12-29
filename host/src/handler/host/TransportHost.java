package handler.host;

import com.bitwig.extension.controller.api.Transport;
import com.bitwig.extension.controller.api.ControllerHost;
import protocol.Protocol;

/**
 * TransportHost - Observes Bitwig Transport and sends updates TO controller
 *
 * RESPONSIBILITY: Bitwig → Controller (Transport)
 * - Observes transport changes (play, stop, record, tempo)
 * - Observes automation state (write enabled, write mode, override active)
 * - Observes overdub state (arranger, clip launcher)
 * - Sends protocol messages when changes occur
 * - NEVER receives protocol callbacks (that's TransportController's job)
 * - NEVER executes Bitwig actions (that's TransportController's job)
 */
public class TransportHost {
    private final Protocol protocol;
    private final Transport transport;

    // Automation write mode mapping: Bitwig string → protocol UINT8
    private static final int WRITE_MODE_LATCH = 0;
    private static final int WRITE_MODE_TOUCH = 1;
    private static final int WRITE_MODE_WRITE = 2;

    public TransportHost(
        ControllerHost host,
        Protocol protocol,
        Transport transport
    ) {
        this.protocol = protocol;
        this.transport = transport;
    }

    /**
     * Setup transport state observers
     */
    public void setupObservers() {
        setupTransportObservers();
        setupAutomationObservers();
        setupOverdubObservers();
    }

    private void setupTransportObservers() {
        transport.isPlaying().markInterested();
        transport.isArrangerRecordEnabled().markInterested();
        transport.tempo().markInterested();

        transport.isPlaying().addValueObserver(isPlaying -> {
            protocol.transportPlayingState(isPlaying);
        });

        transport.isArrangerRecordEnabled().addValueObserver(isRecording -> {
            protocol.transportRecordingState(isRecording);
        });

        transport.tempo().value().addRawValueObserver(tempo -> {
            protocol.transportTempoState((float) tempo);
        });
    }

    private void setupAutomationObservers() {
        transport.isAutomationOverrideActive().markInterested();
        transport.isArrangerAutomationWriteEnabled().markInterested();
        transport.isClipLauncherAutomationWriteEnabled().markInterested();
        transport.automationWriteMode().markInterested();

        transport.isAutomationOverrideActive().addValueObserver(isActive -> {
            protocol.transportAutomationOverrideActiveState(isActive);
        });

        transport.isArrangerAutomationWriteEnabled().addValueObserver(isEnabled -> {
            protocol.transportArrangerAutomationWriteEnabledState(isEnabled);
        });

        transport.isClipLauncherAutomationWriteEnabled().addValueObserver(isEnabled -> {
            protocol.transportClipLauncherAutomationWriteEnabledState(isEnabled);
        });

        transport.automationWriteMode().addValueObserver(mode -> {
            protocol.transportAutomationWriteModeState(writeModeToInt(mode));
        });
    }

    private void setupOverdubObservers() {
        transport.isArrangerOverdubEnabled().markInterested();
        transport.isClipLauncherOverdubEnabled().markInterested();

        transport.isArrangerOverdubEnabled().addValueObserver(isEnabled -> {
            protocol.transportArrangerOverdubEnabledState(isEnabled);
        });

        transport.isClipLauncherOverdubEnabled().addValueObserver(isEnabled -> {
            protocol.transportClipLauncherOverdubEnabledState(isEnabled);
        });
    }

    /**
     * Send current transport state (called at startup)
     */
    public void sendInitialState() {
        sendTransportState();
        sendAutomationState();
        sendOverdubState();
    }

    private void sendTransportState() {
        boolean isPlaying = transport.isPlaying().get();
        boolean isRecording = transport.isArrangerRecordEnabled().get();
        double tempo = transport.tempo().getRaw();

        protocol.transportPlayingState(isPlaying);
        protocol.transportRecordingState(isRecording);
        protocol.transportTempoState((float) tempo);
    }

    private void sendAutomationState() {
        boolean overrideActive = transport.isAutomationOverrideActive().get();
        boolean arrangerWriteEnabled = transport.isArrangerAutomationWriteEnabled().get();
        boolean clipLauncherWriteEnabled = transport.isClipLauncherAutomationWriteEnabled().get();
        String writeMode = transport.automationWriteMode().get();

        protocol.transportAutomationOverrideActiveState(overrideActive);
        protocol.transportArrangerAutomationWriteEnabledState(arrangerWriteEnabled);
        protocol.transportClipLauncherAutomationWriteEnabledState(clipLauncherWriteEnabled);
        protocol.transportAutomationWriteModeState(writeModeToInt(writeMode));
    }

    private void sendOverdubState() {
        boolean arrangerOverdub = transport.isArrangerOverdubEnabled().get();
        boolean clipLauncherOverdub = transport.isClipLauncherOverdubEnabled().get();

        protocol.transportArrangerOverdubEnabledState(arrangerOverdub);
        protocol.transportClipLauncherOverdubEnabledState(clipLauncherOverdub);
    }

    /**
     * Convert Bitwig automation write mode string to protocol UINT8
     * @param mode Bitwig mode string ("latch", "touch", "write")
     * @return Protocol mode value (0=latch, 1=touch, 2=write)
     */
    private int writeModeToInt(String mode) {
        if (mode == null) return WRITE_MODE_LATCH;
        switch (mode.toLowerCase()) {
            case "touch": return WRITE_MODE_TOUCH;
            case "write": return WRITE_MODE_WRITE;
            default: return WRITE_MODE_LATCH;
        }
    }
}
