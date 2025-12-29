package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import handler.host.DeviceHost;
import config.BitwigConfig;

/**
 * DeviceController - Handles Device commands FROM controller
 *
 * RESPONSIBILITY: Controller → Bitwig (Device)
 * - Receives protocol callbacks (protocol.onXXX)
 * - Executes Bitwig API actions (parameter changes, page selection, device selection)
 * - Delegates navigation to DeviceHost (needs layerBank/drumPadBank)
 * - NEVER observes Bitwig API directly
 * - NEVER sends protocol messages (confirmations come from Host observers)
 */
public class DeviceController {
    private final ControllerHost host;
    private final CursorDevice cursorDevice;
    private final CursorRemoteControlsPage remoteControls;
    private final Protocol protocol;
    private final DeviceBank deviceBank;
    private final Transport transport;
    private DeviceHost deviceHost;

    private final boolean[] touchState = new boolean[BitwigConfig.MAX_PARAMETERS];
    private final long[] lastReleaseTime = new long[BitwigConfig.MAX_PARAMETERS];
    private final long[] lastPressTime = new long[BitwigConfig.MAX_PARAMETERS];
    private final int[] parameterDiscreteCount = new int[BitwigConfig.MAX_PARAMETERS];  // -1=continuous, 2=button, >2=list

    // Echo tracking with timeout: timestamp of last controller change
    // All callbacks within BitwigConfig.ECHO_TIMEOUT_MS are considered echoes
    private final long[] lastControllerChangeTime = new long[BitwigConfig.MAX_PARAMETERS];

    public DeviceController(
        ControllerHost host,
        CursorDevice cursorDevice,
        CursorRemoteControlsPage remoteControls,
        Protocol protocol,
        DeviceBank deviceBank,
        Transport transport
    ) {
        this.host = host;
        this.cursorDevice = cursorDevice;
        this.remoteControls = remoteControls;
        this.protocol = protocol;
        this.deviceBank = deviceBank;
        this.transport = transport;

        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            parameterDiscreteCount[i] = -1;  // Default: continuous (safest for echo suppression)
        }

        setupProtocolCallbacks();
    }

    public void setDeviceHost(DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    public void updateParameterMetadata(int paramIndex, int discreteCount) {
        if (paramIndex >= 0 && paramIndex < BitwigConfig.MAX_PARAMETERS) {
            parameterDiscreteCount[paramIndex] = discreteCount;
        }
    }

    /**
     * Mark the timestamp of controller change for this parameter.
     * Called when controller sends an update to Bitwig.
     */
    private void markControllerChange(int paramIndex) {
        if (paramIndex >= 0 && paramIndex < BitwigConfig.MAX_PARAMETERS) {
            lastControllerChangeTime[paramIndex] = System.currentTimeMillis();
        }
    }

    /**
     * Check if this callback is within the echo timeout window.
     * Returns true if callback arrived within ECHO_TIMEOUT_MS of last controller change.
     */
    public boolean consumeEcho(int paramIndex) {
        if (paramIndex >= 0 && paramIndex < BitwigConfig.MAX_PARAMETERS) {
            long now = System.currentTimeMillis();
            long timeSinceChange = now - lastControllerChangeTime[paramIndex];
            return timeSinceChange < BitwigConfig.ECHO_TIMEOUT_MS;
        }
        return false;
    }

    /**
     * Check if ANY parameter is currently in echo state.
     * Used to skip batch sends during controller activity.
     */
    public boolean isAnyEchoActive() {
        long now = System.currentTimeMillis();
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            if (now - lastControllerChangeTime[i] < BitwigConfig.ECHO_TIMEOUT_MS) {
                return true;
            }
        }
        return false;
    }

    private void setupProtocolCallbacks() {
        // Remote control value FROM controller
        protocol.onRemoteControlValue = msg -> {
            int index = msg.getRemoteControlIndex();
            if (index < 0 || index >= BitwigConfig.MAX_PARAMETERS) return;

            long now = System.currentTimeMillis();
            long timeSincePress = now - lastPressTime[index];
            long timeSinceRelease = now - lastReleaseTime[index];

            boolean inPressDelay = touchState[index] && timeSincePress < BitwigConfig.TOUCH_PRESS_DELAY_MS;
            boolean inReleaseGrace = !touchState[index] && timeSinceRelease < BitwigConfig.TOUCH_RELEASE_GRACE_MS;

            if (inPressDelay || inReleaseGrace)
                return;

            RemoteControl param = remoteControls.getParameter(index);
            markControllerChange(index);
            param.set(msg.getParameterValue());
        };

        // Toggle device state FROM controller
        protocol.onDeviceState = msg -> {
            int deviceIndex = msg.getDeviceIndex();
            Device device = deviceBank.getItemAt(deviceIndex);

            if (device.exists().get()) {
                // Toggle device enabled state
                boolean currentState = device.isEnabled().get();
                device.isEnabled().set(!currentState);
                // Confirmation will be sent by observer in DeviceHost
            }
        };

        protocol.onDeviceRemoteControlTouch = msg -> {
            int paramIndex = msg.getRemoteControlIndex();
            if (paramIndex < 0 || paramIndex >= BitwigConfig.MAX_PARAMETERS)
                return;

            boolean isTouched = msg.isTouched();
            if (touchState[paramIndex] == isTouched)
                return;

            touchState[paramIndex] = isTouched;
            RemoteControl param = remoteControls.getParameter(paramIndex);

            if (isTouched) {
                transport.isClipLauncherAutomationWriteEnabled().set(true);
                param.touch(true);
                lastPressTime[paramIndex] = System.currentTimeMillis();
            } else {
                param.touch(false);
                lastReleaseTime[paramIndex] = System.currentTimeMillis();
                host.scheduleTask(() -> {
                    transport.isClipLauncherAutomationWriteEnabled().set(false);
                }, BitwigConfig.TOUCH_RELEASE_GRACE_MS);
            }
        };

        // Request windowed page names FROM controller
        protocol.onRequestDevicePageNamesWindow = msg -> {
            if (deviceHost != null) {
                deviceHost.sendPageNamesWindow(msg.getPageStartIndex());
            }
        };

        // Select page by index FROM controller (with modulo wrap)
        protocol.onDevicePageSelect = msg -> {
            int requestedIndex = msg.getDevicePageIndex();
            int pageCount = remoteControls.pageCount().get();
            if (pageCount > 0) {
                // Modulo wrap: index 10 with 5 pages → page 0
                int validIndex = requestedIndex % pageCount;
                remoteControls.selectedPageIndex().set(validIndex);
            }
        };

        // ========================================================================
        // Hierarchical Device Navigation Callbacks
        // ========================================================================

        // Request windowed device list FROM controller
        protocol.onRequestDeviceListWindow = msg -> {
            if (deviceHost != null) {
                deviceHost.sendDeviceListWindow(msg.getDeviceStartIndex());
            }
        };

        // Request device children (slots/layers/drum pads) FROM controller
        protocol.onRequestDeviceChildren = msg -> {
            if (deviceHost != null) {
                deviceHost.sendDeviceChildren(msg.getDeviceIndex(), msg.getChildType());
            }
        };

        // Enter device child (navigate into slot/layer/drum pad) FROM controller
        protocol.onEnterDeviceChild = msg -> {
            if (deviceHost != null) {
                deviceHost.enterDeviceChild(
                    msg.getDeviceIndex(),
                    msg.getChildType(),
                    msg.getChildIndex()
                );
            }
        };

        // Exit to parent device FROM controller
        protocol.onExitToParent = msg -> {
            if (deviceHost != null) {
                deviceHost.exitToParent();
            }
        };

        // Select device by index FROM controller
        protocol.onDeviceSelect = msg -> {
            int deviceIndex = msg.getDeviceIndex();
            Device device = deviceBank.getItemAt(deviceIndex);
            if (device.exists().get()) {
                cursorDevice.selectDevice(device);
                // NOTE: Don't auto-select first layer here
                // Let enterDeviceChild handle layer/slot/drumpad navigation explicitly
            }
        };

        // ========================================================================
        // View State Callback (for batch send control)
        // ========================================================================

        // Controller view state changed - controls batch modulated values send
        protocol.onViewState = msg -> {
            if (deviceHost != null) {
                deviceHost.setControllerViewState(msg.getViewType(), msg.getSelectorActive());
            }
        };

        // ========================================================================
        // Automation Restore Callback
        // ========================================================================

        // Restore automation playback globally (double tap on NAV button)
        // Uses Bitwig's transport.resetAutomationOverrides() to restore ALL automation
        protocol.onDeviceRemoteControlRestoreAutomation = msg -> {
            // Only process once (first message triggers global reset)
            // Controller sends for all 8 params, but we only need to act once
            if (msg.getRemoteControlIndex() != 0) return;

            // Global automation reset via Bitwig API
            transport.resetAutomationOverrides();

            // Clear all touched states
            for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
                touchState[i] = false;
            }
        };
    }
}
