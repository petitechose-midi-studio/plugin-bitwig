package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import handler.controller.LastClickedController;

/**
 * LastClickedHost - Observes last clicked parameter in Bitwig and sends updates TO controller
 *
 * RESPONSIBILITY: Bitwig → Controller (Last Clicked Parameter)
 * - Observes parameter changes (via name() observer)
 * - Sends full update when different param clicked
 * - Observes value changes and sends ValueState with echo detection
 * - NEVER receives protocol callbacks (that's LastClickedController's job)
 */
public class LastClickedHost {
    private final Protocol protocol;
    private final LastClickedParameter lastClicked;
    private LastClickedController lastClickedController;

    private String lastParamName = "";

    public LastClickedHost(ControllerHost host, Protocol protocol) {
        this.protocol = protocol;
        this.lastClicked = host.createLastClickedParameter("last_clicked", "Last Clicked");
    }

    public void setLastClickedController(LastClickedController controller) {
        this.lastClickedController = controller;
    }

    /**
     * Setup observers for last clicked parameter
     */
    public void setupObservers() {
        // Mark observables as interested
        lastClicked.parameter().markInterested();
        lastClicked.parameter().name().markInterested();
        lastClicked.parameter().value().markInterested();
        lastClicked.parameter().value().getOrigin().markInterested();
        lastClicked.parameter().value().discreteValueCount().markInterested();
        lastClicked.parameter().value().discreteValueNames().markInterested();
        lastClicked.parameter().displayedValue().markInterested();
        lastClicked.parameter().exists().markInterested();

        // Observer: when exists changes (parameter selected/deselected)
        // Using exists() as primary trigger like the original working version
        lastClicked.parameter().exists().addValueObserver(exists -> {
            String name = lastClicked.parameter().name().get();

            if (!exists) {
                // Parameter cleared - only if we had a param before
                if (!lastParamName.isEmpty()) {
                    lastParamName = "";
                    sendLastClickedCleared();
                }
            } else if (!name.isEmpty()) {
                // Parameter selected with valid name
                lastParamName = name;
                sendLastClickedUpdate();
            }
        });

        // Value observer: send value changes to controller with echo detection
        // Only send for host-initiated changes (not echoes from controller)
        lastClicked.parameter().displayedValue().addValueObserver(displayValue -> {
            if (!lastClicked.parameter().exists().get()) return;
            if (lastClickedController != null && lastClickedController.consumeEcho()) return;

            double value = lastClicked.parameter().value().get();
            protocol.lastClickedValueState((float) value, displayValue);
        });
    }

    /**
     * Send initial state (called at startup)
     */
    public void sendInitialState() {
        if (lastClicked.parameter().exists().get()) {
            lastParamName = lastClicked.parameter().name().get();
            sendLastClickedUpdate();
        }
    }

    /**
     * Get the underlying parameter for controller to modify
     */
    public Parameter getParameter() {
        return lastClicked.parameter();
    }

    /**
     * Send full parameter info (when new parameter clicked)
     */
    private void sendLastClickedUpdate() {
        final Parameter param = lastClicked.parameter();
        final String name = param.name().get();
        final double value = param.value().get();
        final String displayValue = param.displayedValue().get();
        final double origin = param.value().getOrigin().get();
        final boolean exists = param.exists().get();
        final int discreteCount = param.value().discreteValueCount().get();

        // Parameter type: 0=continuous, 1=list (>2 values), -1=unknown
        final int paramType;
        if (discreteCount > 2) {
            paramType = 1;  // List selector
        } else if (discreteCount < 0) {
            paramType = -1; // Unknown/continuous
        } else {
            paramType = 0;  // Button or continuous
        }

        final int currentValueIndex = discreteCount > 0 ? (int)(value * discreteCount) : 0;

        protocol.lastClickedUpdate(
            name,
            (float) value,
            displayValue,
            (float) origin,
            exists,
            paramType,
            (short) discreteCount,
            currentValueIndex
        );
    }

    /**
     * Send cleared message (when parameter deselected)
     */
    private void sendLastClickedCleared() {
        protocol.lastClickedUpdate(
            "",
            0.0f,
            "",
            0.0f,
            false,
            0,
            (short) 0,
            0
        );
    }
}
