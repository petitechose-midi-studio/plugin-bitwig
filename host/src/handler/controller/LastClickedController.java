package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import handler.host.LastClickedHost;

/**
 * LastClickedController - Handles LastClicked commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Last Clicked Parameter)
 * - Receives protocol callbacks (protocol.onLastClickedValueChange)
 * - Applies value changes to the parameter
 * - NEVER observes Bitwig API directly (that's LastClickedHost's job)
 */
public class LastClickedController {
    private final Protocol protocol;
    private LastClickedHost lastClickedHost;

    public LastClickedController(Protocol protocol) {
        this.protocol = protocol;
        setupProtocolCallbacks();
    }

    public void setLastClickedHost(LastClickedHost lastClickedHost) {
        this.lastClickedHost = lastClickedHost;
    }

    private void setupProtocolCallbacks() {
        protocol.onLastClickedValueChange = msg -> {
            if (msg.fromHost) return;
            handleValueChange(msg.getParameterValue());
        };
    }

    private void handleValueChange(float value) {
        if (lastClickedHost != null) {
            Parameter param = lastClickedHost.getParameter();
            if (param != null && param.exists().get()) {
                param.set(value);
            }
        }
    }
}
