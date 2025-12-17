package handler.controller;

import com.bitwig.extension.controller.api.Transport;
import protocol.Protocol;

/**
 * TransportController - Handles Transport commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Transport)
 * - Receives protocol callbacks (protocol.onXXX)
 * - Executes Bitwig API actions (play, stop, record, tempo changes)
 * - Handles automation and overdub toggle commands
 * - NEVER observes Bitwig API directly
 * - NEVER sends protocol messages (confirmations come from TransportHost observers)
 */
public class TransportController {
    private final Transport transport;
    private final Protocol protocol;

    // Automation write mode mapping: protocol UINT8 → Bitwig string
    private static final String[] WRITE_MODES = {"latch", "touch", "write"};

    public TransportController(Transport transport, Protocol protocol) {
        this.transport = transport;
        this.protocol = protocol;
        setupProtocolCallbacks();
    }

    private void setupProtocolCallbacks() {
        setupTransportCallbacks();
        setupAutomationCallbacks();
        setupOverdubCallbacks();
    }

    private void setupTransportCallbacks() {
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

    private void setupAutomationCallbacks() {
        protocol.onTransportArrangerAutomationWriteEnabledChange = msg -> {
            if (msg.fromHost) return;
            transport.isArrangerAutomationWriteEnabled().set(msg.isArrangerAutomationWriteEnabled());
        };

        protocol.onTransportClipLauncherAutomationWriteEnabledChange = msg -> {
            if (msg.fromHost) return;
            transport.isClipLauncherAutomationWriteEnabled().set(msg.isClipLauncherAutomationWriteEnabled());
        };

        protocol.onTransportAutomationWriteModeChange = msg -> {
            if (msg.fromHost) return;
            int modeInt = msg.getAutomationWriteMode();
            String mode = (modeInt >= 0 && modeInt < WRITE_MODES.length) ? WRITE_MODES[modeInt] : "latch";
            transport.automationWriteMode().set(mode);
        };

        protocol.onResetAutomationOverrides = msg -> {
            if (msg.fromHost) return;
            transport.resetAutomationOverrides();
        };
    }

    private void setupOverdubCallbacks() {
        protocol.onTransportArrangerOverdubEnabledChange = msg -> {
            if (msg.fromHost) return;
            transport.isArrangerOverdubEnabled().set(msg.isArrangerOverdubEnabled());
        };

        protocol.onTransportClipLauncherOverdubEnabledChange = msg -> {
            if (msg.fromHost) return;
            transport.isClipLauncherOverdubEnabled().set(msg.isClipLauncherOverdubEnabled());
        };
    }
}
