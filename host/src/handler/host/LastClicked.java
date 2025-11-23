package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;

/**
 * LastClickedHost - Observes last clicked parameter in Bitwig and sends updates TO controller
 *
 * Monitors the last clicked/touched parameter and sends protocol messages
 * to hardware controller for control via OPT encoder.
 *
 * SINGLE RESPONSIBILITY: Bitwig → Controller (Last Clicked Parameter)
 */
public class LastClicked {
    private final ControllerHost host;
    private final Protocol protocol;
    private final LastClickedParameter lastClicked;

    public LastClicked(
        ControllerHost host,
        Protocol protocol
    ) {
        this.host = host;
        this.protocol = protocol;
        this.lastClicked = host.createLastClickedParameter("last_clicked", "Last Clicked");
    }

    /**
     * Setup observers for last clicked parameter
     */
    public void setup() {
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

        // Protocol callback: value change FROM controller
        protocol.onLastClickedValueChange = msg -> {
            handleValueChange(msg.getParameterValue());
        };
    }

    /**
     * Send initial state (called at startup)
     *
     * Delay send to ensure Bitwig API values are stabilized
     */
    public void sendInitialState() {
        host.scheduleTask(() -> {
            if (lastClicked.parameter().exists().get()) {
                sendLastClickedUpdate();
            }
        }, BitwigConfig.INITIAL_STATE_SEND_DELAY_MS);
    }

    /**
     * Send full parameter info (when new parameter clicked)
     */
    private void sendLastClickedUpdate() {
        // Use lastClicked.parameter() directly (already marked interested in setup)
        String name = lastClicked.parameter().name().get();
        double value = lastClicked.parameter().value().get();
        String displayValue = lastClicked.parameter().displayedValue().get();
        double origin = lastClicked.parameter().value().getOrigin().get();
        boolean exists = lastClicked.parameter().exists().get();

        // Detect parameter type (simplified - could be enhanced)
        int discreteCount = lastClicked.parameter().value().discreteValueCount().get(); // -1=continuous (default)
        int paramType = discreteCount > 2 ? 1 : discreteCount < 0 ? -1: 0; // 0=Knob (default)
        int currentValueIndex = (int)(value * (double)discreteCount);

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

    /**
     * Handle value change from controller
     */
    private void handleValueChange(float value) {
        // Apply value to parameter (Bitwig will update the UI)
        lastClicked.parameter().set(value);
    }
}
