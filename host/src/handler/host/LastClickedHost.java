package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;

/**
 * LastClickedHost - Observes last clicked parameter in Bitwig and sends updates TO controller
 *
 * RESPONSIBILITY: Bitwig → Controller (Last Clicked Parameter)
 * - Observes last clicked parameter changes
 * - Sends protocol messages when parameter changes
 * - NEVER receives protocol callbacks (that's LastClickedController's job)
 */
public class LastClickedHost {
    private final Protocol protocol;
    private final LastClickedParameter lastClicked;

    public LastClickedHost(ControllerHost host, Protocol protocol) {
        this.protocol = protocol;
        this.lastClicked = host.createLastClickedParameter("last_clicked", "Last Clicked");
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

        // Observer: when parameter changes (different param clicked in Bitwig)
        lastClicked.parameter().exists().addValueObserver(exists -> {
            sendLastClickedUpdate();
        });
    }

    /**
     * Send initial state (called at startup)
     */
    public void sendInitialState() {
        if (lastClicked.parameter().exists().get()) {
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

        protocol.send(new LastClickedUpdateMessage(
            name,
            (float) value,
            displayValue,
            (float) origin,
            exists,
            (byte) paramType,
            (short) discreteCount,
            (byte) currentValueIndex
        ));
    }
}
