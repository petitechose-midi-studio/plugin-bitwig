package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import handler.host.LastClickedHost;
import config.BitwigConfig;

/**
 * LastClickedController - Handles LastClicked commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Last Clicked Parameter)
 * - Receives protocol callbacks (protocol.onLastClickedValue)
 * - Applies value changes to the parameter
 * - Tracks echo timing for value changes
 * - NEVER observes Bitwig API directly (that's LastClickedHost's job)
 */
public class LastClickedController {
    private final Protocol protocol;
    private LastClickedHost lastClickedHost;

    // Echo tracking: timestamp of last controller change
    private long lastControllerChangeTime = 0;
    private float lastControllerValue = 0.0f;
    private boolean hasLastControllerValue = false;

    public LastClickedController(ControllerHost host, Protocol protocol) {
        this.protocol = protocol;
        setupProtocolCallbacks();
    }

    public void setLastClickedHost(LastClickedHost lastClickedHost) {
        this.lastClickedHost = lastClickedHost;
    }

    /**
     * Mark timestamp when controller sends a value change.
     * Called before applying value to Bitwig.
     */
    private void markControllerChange(float value) {
        lastControllerChangeTime = System.currentTimeMillis();
        lastControllerValue = value;
        hasLastControllerValue = true;
    }

    /**
     * Check if this callback is within the echo timeout window.
     * Returns true if callback arrived within ECHO_TIMEOUT_MS of last controller change.
     */
    public boolean consumeEcho(float candidateValue) {
        if (!hasLastControllerValue) return false;
        long now = System.currentTimeMillis();
        long timeSinceChange = now - lastControllerChangeTime;
        if (timeSinceChange >= BitwigConfig.ECHO_TIMEOUT_MS) return false;
        return Math.abs(candidateValue - lastControllerValue) < BitwigConfig.LAST_CLICKED_ECHO_EPSILON;
    }

    private void setupProtocolCallbacks() {
        protocol.onLastClickedValue = msg -> {
            float value = msg.getParameterValue();
            markControllerChange(value);
            handleValueChange(value);
        };

        protocol.onLastClickedTouch = msg -> {
            if (lastClickedHost != null) {
                Parameter param = lastClickedHost.getParameter();
                if (param != null && param.exists().get()) {
                    param.touch(msg.isTouched());
                }
            }
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
