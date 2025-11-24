package handler.util;

import com.bitwig.extension.controller.api.ControllerHost;
import com.bitwig.extension.controller.api.IntegerValue;
import com.bitwig.extension.controller.api.BooleanValue;
import com.bitwig.extension.controller.api.StringValue;

import java.util.HashMap;
import java.util.Map;

/**
 * Generic handler for requests that need to wait for Bitwig state updates.
 *
 * Eliminates fixed delays by using Bitwig API observers to detect when data is ready.
 *
 * Usage:
 * 1. Register a context: handler.registerContext("deviceList", deviceBank.itemCount())
 * 2. Before state change: handler.notifyChangePending("deviceList")
 * 3. On request: handler.requestSend("deviceList", () -> sendData())
 */
public class ObserverBasedRequestHandler {
    private final ControllerHost host;
    private final Map<String, RequestContext> contexts;
    private static final int SAFETY_TIMEOUT_MS = 50;

    public ObserverBasedRequestHandler(ControllerHost host) {
        this.host = host;
        this.contexts = new HashMap<>();
    }

    /**
     * Register a context with its trigger observable.
     * The observable should fire when Bitwig has finished updating the relevant state.
     */
    public void registerContext(String contextName, IntegerValue trigger) {
        RequestContext context = new RequestContext(contextName);
        contexts.put(contextName, context);

        // Set up observer on trigger
        trigger.addValueObserver(value -> {
            if (context.hasPendingRequest()) {
                host.println("[ObserverHandler] Context '" + contextName + "' ready (value: " + value + ") - executing pending request");
                context.executePendingRequest();
            }
        });
    }

    /**
     * Register a context with its trigger observable (BooleanValue overload).
     * The observable should fire when Bitwig has finished updating the relevant state.
     */
    public void registerContext(String contextName, BooleanValue trigger) {
        RequestContext context = new RequestContext(contextName);
        contexts.put(contextName, context);

        // Set up observer on trigger
        trigger.addValueObserver(value -> {
            if (context.hasPendingRequest()) {
                host.println("[ObserverHandler] Context '" + contextName + "' ready (boolean: " + value + ") - executing pending request");
                context.executePendingRequest();
            }
        });
    }

    /**
     * Register a context with its trigger observable (StringValue overload).
     * The observable should fire when Bitwig has finished updating the relevant state.
     */
    public void registerContext(String contextName, StringValue trigger) {
        RequestContext context = new RequestContext(contextName);
        contexts.put(contextName, context);

        // Set up observer on trigger
        trigger.addValueObserver(value -> {
            if (context.hasPendingRequest()) {
                host.println("[ObserverHandler] Context '" + contextName + "' ready (string: '" + value + "') - executing pending request");
                context.executePendingRequest();
            }
        });
    }

    /**
     * Notify that a state change is about to happen for this context.
     * Any subsequent requests will wait for the trigger observable to fire.
     */
    public void notifyChangePending(String contextName) {
        RequestContext context = contexts.get(contextName);
        if (context == null) {
            host.println("[ObserverHandler] WARNING: Unknown context '" + contextName + "'");
            return;
        }

        host.println("[ObserverHandler] Context '" + contextName + "' change pending");
        context.setChangePending(true);

        // Safety timeout: clear flag even if observer doesn't fire
        host.scheduleTask(() -> {
            if (context.isChangePending()) {
                host.println("[ObserverHandler] Context '" + contextName + "' timeout - clearing flag");
                context.setChangePending(false);
                context.executePendingRequestIfAny();
            }
        }, SAFETY_TIMEOUT_MS);
    }

    /**
     * Request to send data. Will execute immediately if no change is pending,
     * or wait for the observer to fire if a change is in progress.
     */
    public void requestSend(String contextName, Runnable action) {
        RequestContext context = contexts.get(contextName);
        if (context == null) {
            host.println("[ObserverHandler] WARNING: Unknown context '" + contextName + "' - executing immediately");
            action.run();
            return;
        }

        if (context.isChangePending()) {
            host.println("[ObserverHandler] Context '" + contextName + "' request queued - waiting for observer");
            context.setPendingRequest(action);
        } else {
            host.println("[ObserverHandler] Context '" + contextName + "' request - executing immediately");
            action.run();
        }
    }

    /**
     * Internal class to track state for each context.
     */
    private class RequestContext {
        private final String name;
        private boolean changePending;
        private Runnable pendingRequest;

        public RequestContext(String name) {
            this.name = name;
            this.changePending = false;
            this.pendingRequest = null;
        }

        public boolean isChangePending() {
            return changePending;
        }

        public void setChangePending(boolean pending) {
            this.changePending = pending;
        }

        public boolean hasPendingRequest() {
            return pendingRequest != null;
        }

        public void setPendingRequest(Runnable request) {
            this.pendingRequest = request;
        }

        public void executePendingRequest() {
            if (pendingRequest != null) {
                changePending = false;
                Runnable request = pendingRequest;
                pendingRequest = null;
                request.run();
            }
        }

        public void executePendingRequestIfAny() {
            if (hasPendingRequest()) {
                host.println("[ObserverHandler] Context '" + name + "' executing pending request from timeout");
                executePendingRequest();
            }
        }
    }
}
