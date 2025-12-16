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
 * - Checks fromHost flag (returns early if true)
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

    private void setupProtocolCallbacks() {
        protocol.onDeviceMacroValueChange = msg -> {
            if (msg.fromHost) {
                host.println("[DEVICE CTRL] Feedback from host: param=" + msg.getParameterIndex() + " value=" + msg.getParameterValue());
                return;
            }

            int index = msg.getParameterIndex();
            if (index < 0 || index >= BitwigConfig.MAX_PARAMETERS) return;

            long now = System.currentTimeMillis();
            long timeSincePress = now - lastPressTime[index];
            long timeSinceRelease = now - lastReleaseTime[index];

            boolean inPressDelay = touchState[index] && timeSincePress < BitwigConfig.TOUCH_PRESS_DELAY_MS;
            boolean inReleaseGrace = !touchState[index] && timeSinceRelease < BitwigConfig.TOUCH_RELEASE_GRACE_MS;

            host.println("[DEVICE CTRL] Param " + index + " = " + msg.getParameterValue() +
                        " (touched=" + touchState[index] + ", pressDelay=" + inPressDelay + ", releaseGrace=" + inReleaseGrace + ")");

            if (inPressDelay || inReleaseGrace) {
                host.println("[DEVICE CTRL] ✗ Blocked by timing delay");
                return;
            }

            RemoteControl param = remoteControls.getParameter(index);
            markControllerChange(index);
            param.set(msg.getParameterValue());
        };

        // Toggle device state FROM controller
        protocol.onDeviceStateChange = msg -> {
            if (msg.fromHost) return;

            int deviceIndex = msg.getDeviceIndex();
            Device device = deviceBank.getItemAt(deviceIndex);

            if (device.exists().get()) {
                // Toggle device enabled state
                boolean currentState = device.isEnabled().get();
                device.isEnabled().set(!currentState);
                // Confirmation will be sent by observer in DeviceHost
            }
        };

        protocol.onDeviceMacroTouch = msg -> {
            if (msg.fromHost) return;

            int paramIndex = msg.getParameterIndex();
            host.println("[TOUCH] Received: param=" + paramIndex + " touched=" + msg.isTouched());

            if (paramIndex < 0 || paramIndex >= BitwigConfig.MAX_PARAMETERS) {
                host.println("[TOUCH] ✗ Invalid param index");
                return;
            }

            boolean isTouched = msg.isTouched();
            if (touchState[paramIndex] == isTouched) {
                host.println("[TOUCH] ✗ Debounced (same state)");
                return;
            }

            touchState[paramIndex] = isTouched;
            RemoteControl param = remoteControls.getParameter(paramIndex);

            if (isTouched) {
                host.println("[TOUCH] ▶ Press: enabling automation write + touch(true)");
                transport.isClipLauncherAutomationWriteEnabled().set(true);
                param.touch(true);
                lastPressTime[paramIndex] = System.currentTimeMillis();
            } else {
                host.println("[TOUCH] ◀ Release: touch(false) + scheduling automation disable");
                param.touch(false);
                lastReleaseTime[paramIndex] = System.currentTimeMillis();
                host.scheduleTask(() -> {
                    host.println("[TOUCH] ⏱ Delayed: disabling automation write");
                    transport.isClipLauncherAutomationWriteEnabled().set(false);
                }, BitwigConfig.TOUCH_RELEASE_GRACE_MS);
            }
        };

        // Request page names list FROM controller
        protocol.onRequestDevicePageNames = msg -> {
            if (msg.fromHost) return;
            if (deviceHost != null) {
                deviceHost.sendPageNames();
            }
        };

        // Select page by index FROM controller (with modulo wrap)
        protocol.onDevicePageSelectByIndex = msg -> {
            if (msg.fromHost) return;
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

        // Request device list FROM controller
        protocol.onRequestDeviceList = msg -> {
            if (msg.fromHost) return;
            if (deviceHost != null) {
                deviceHost.sendDeviceList();
            }
        };

        // Request device children (slots/layers/drum pads) FROM controller
        protocol.onRequestDeviceChildren = msg -> {
            if (msg.fromHost) return;
            if (deviceHost != null) {
                deviceHost.sendDeviceChildren(msg.getDeviceIndex(), msg.getChildType());
            }
        };

        // Enter device child (navigate into slot/layer/drum pad) FROM controller
        protocol.onEnterDeviceChild = msg -> {
            if (msg.fromHost) return;
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
            if (msg.fromHost) return;
            if (deviceHost != null) {
                deviceHost.exitToParent();
            }
        };

        // Select device by index FROM controller
        protocol.onDeviceSelectByIndex = msg -> {
            if (msg.fromHost) return;
            int deviceIndex = msg.getDeviceIndex();
            Device device = deviceBank.getItemAt(deviceIndex);
            if (device.exists().get()) {
                cursorDevice.selectDevice(device);
                // NOTE: Don't auto-select first layer here
                // Let enterDeviceChild handle layer/slot/drumpad navigation explicitly
            }
        };
    }
}
