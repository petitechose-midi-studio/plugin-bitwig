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
            transport.tempo().incRaw(msg.getTempo());
        };
    }

    private void setupAutomationCallbacks() {
        protocol.onTransportArrangerAutomationWriteEnabled = msg -> {
            transport.isArrangerAutomationWriteEnabled().set(msg.isArrangerAutomationWriteEnabled());
        };

        protocol.onTransportClipLauncherAutomationWriteEnabled = msg -> {
            transport.isClipLauncherAutomationWriteEnabled().set(msg.isClipLauncherAutomationWriteEnabled());
        };

        protocol.onTransportAutomationWriteMode = msg -> {
            int modeInt = msg.getAutomationWriteMode();
            String mode = (modeInt >= 0 && modeInt < WRITE_MODES.length) ? WRITE_MODES[modeInt] : "latch";
            transport.automationWriteMode().set(mode);
        };

        protocol.onResetAutomationOverrides = msg -> {
            transport.resetAutomationOverrides();
        };
    }

    private void setupOverdubCallbacks() {
        protocol.onTransportArrangerOverdubEnabled = msg -> {
            transport.isArrangerOverdubEnabled().set(msg.isArrangerOverdubEnabled());
        };

        protocol.onTransportClipLauncherOverdubEnabled = msg -> {
            transport.isClipLauncherOverdubEnabled().set(msg.isClipLauncherOverdubEnabled());
        };
    }
}
