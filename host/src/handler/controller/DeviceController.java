package handler.controller;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.DeviceStateChangeMessage;
import handler.host.DeviceHost;
import config.BitwigConfig;
import java.util.concurrent.ConcurrentLinkedQueue;

/**
 * DeviceController - Handles Device commands FROM controller
 *
 * Listens to protocol callbacks and executes actions on Bitwig CursorDevice.
 *
 * SINGLE RESPONSIBILITY: Controller → Bitwig (Device)
 */
public class DeviceController {
    private final ControllerHost host;
    private final CursorDevice cursorDevice;
    private final CursorRemoteControlsPage remoteControls;
    private final Protocol protocol;
    private final DeviceBank deviceBank;
    private final Transport transport;
    private DeviceHost deviceHost;

    private final boolean[] touchState = new boolean[8];
    private final long[] lastReleaseTime = new long[8];
    private final long[] lastPressTime = new long[8];
    private final int[] parameterDiscreteCount = new int[8];  // -1=continuous, 2=button, >2=list

    // Echo tracking with timeout: timestamp of last controller change
    // All callbacks within ECHO_TIMEOUT_MS are considered echoes
    private final long[] lastControllerChangeTime = new long[8];
    private static final long ECHO_TIMEOUT_MS = 100;  // 200ms window for echoes

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

        for (int i = 0; i < 8; i++) {
            parameterDiscreteCount[i] = -1;  // Default: continuous (safest for echo suppression)
        }

        setupCallbacks();
    }

    public void setDeviceHost(DeviceHost deviceHost) {
        this.deviceHost = deviceHost;
    }

    public void updateParameterMetadata(int paramIndex, int discreteCount) {
        if (paramIndex >= 0 && paramIndex < 8) {
            parameterDiscreteCount[paramIndex] = discreteCount;
        }
    }

    /**
     * Mark the timestamp of controller change for this parameter.
     * Called when controller sends an update to Bitwig.
     */
    private void markControllerChange(int paramIndex) {
        if (paramIndex >= 0 && paramIndex < 8) {
            lastControllerChangeTime[paramIndex] = System.currentTimeMillis();
        }
    }

    /**
     * Check if this callback is within the echo timeout window.
     * Returns true if callback arrived within ECHO_TIMEOUT_MS of last controller change.
     * Returns false if no recent controller change (host-initiated).
     *
     * This handles Bitwig sending multiple callbacks per param.set() - all callbacks
     * within the timeout window are considered echoes.
     */
    public boolean consumeEcho(int paramIndex) {
        if (paramIndex >= 0 && paramIndex < 8) {
            long now = System.currentTimeMillis();
            long timeSinceChange = now - lastControllerChangeTime[paramIndex];
            return timeSinceChange < ECHO_TIMEOUT_MS;
        }
        return false;
    }

    private void setupCallbacks() {
        protocol.onDeviceMacroValueChange = msg -> {
            if (msg.fromHost) {
                host.println("[DeviceController] RECEIVED feedback from HOST param=" + msg.getParameterIndex() + " value=" + msg.getParameterValue());
                return;
            }

            int index = msg.getParameterIndex();
            if (index < 0 || index >= 8) return;

            long now = System.currentTimeMillis();
            long timeSincePress = now - lastPressTime[index];
            long timeSinceRelease = now - lastReleaseTime[index];

            boolean inPressDelay = touchState[index] && timeSincePress < BitwigConfig.TOUCH_PRESS_DELAY_MS;
            boolean inReleaseGrace = !touchState[index] && timeSinceRelease < BitwigConfig.TOUCH_RELEASE_GRACE_MS;

            host.println("[DeviceController] FROM controller param=" + index + " value=" + msg.getParameterValue() +
                        " touched=" + touchState[index] + " inPressDelay=" + inPressDelay + " inReleaseGrace=" + inReleaseGrace);

            if (inPressDelay || inReleaseGrace) {
                host.println("[DeviceController] BLOCKED by timing delay");
                return;
            }

            RemoteControl param = remoteControls.getParameter(index);

            // Mark controller change timestamp - all callbacks within 100ms are echoes
            markControllerChange(index);

            host.println("[DeviceController] SETTING Bitwig param=" + index + " to value=" + msg.getParameterValue());
            param.set(msg.getParameterValue());
        };

        // Receive device state toggle FROM controller
        protocol.onDeviceStateChange = msg -> {
            if (msg.fromHost) return;

            // Toggle device at specified index
            int deviceIndex = msg.getDeviceIndex();
            Device device = deviceBank.getItemAt(deviceIndex);

            if (device.exists().get()) {
                boolean currentState = device.isEnabled().get();
                boolean newState = !currentState;
                device.isEnabled().set(newState);

                // Send confirmation back to controller with correct index
                protocol.send(new DeviceStateChangeMessage(deviceIndex, newState));
            }
        };

        protocol.onDeviceMacroTouch = msg -> {
            if (msg.fromHost) return;

            int paramIndex = msg.getParameterIndex();
            if (paramIndex < 0 || paramIndex >= 8) return;

            boolean isTouched = msg.isTouched();
            if (touchState[paramIndex] == isTouched) return;  // Debounce

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
