package handler.host;

import com.bitwig.extension.controller.api.*;
import config.BitwigConfig;

/**
 * DeviceNavigator - Handles device hierarchy navigation
 *
 * RESPONSIBILITY: Navigate device tree (slots, layers, drum pads)
 * - Enter device child (slot/layer/drum pad)
 * - Exit to parent device
 *
 * NOTE: Separated from DeviceHost for single responsibility.
 * Receives banks from DeviceHost (Bitwig API limitation: banks must be created on CursorDevice).
 */
public class DeviceNavigator {
    private final CursorDevice cursorDevice;
    private final DeviceBank deviceBank;
    private final DeviceLayerBank layerBank;

    public DeviceNavigator(
        ControllerHost host,
        CursorDevice cursorDevice,
        DeviceBank deviceBank,
        DeviceLayerBank layerBank
    ) {
        this.cursorDevice = cursorDevice;
        this.deviceBank = deviceBank;
        this.layerBank = layerBank;
    }

    public void enterDeviceChild(ControllerHost host, int deviceIndex, int itemType, int childIndex) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) {
            return;
        }

        // Select the device first
        cursorDevice.selectDevice(device);

        // Schedule child navigation AFTER device selection completes (async operation)
        // Without this delay, selectFirstInSlot/Channel/KeyPad operates on the PREVIOUS device
        host.scheduleTask(() -> {
            // Navigate into child based on type
            switch (itemType) {
                case 0: // Slot
                    String[] slotNames = device.slotNames().get();
                    if (childIndex >= 0 && childIndex < slotNames.length) {
                        cursorDevice.selectFirstInSlot(slotNames[childIndex]);
                    }
                    break;
                case 1: // Layer
                    DeviceLayer layer = layerBank.getItemAt(childIndex);
                    if (layer.exists().get()) {
                        layer.selectInEditor();
                        cursorDevice.selectFirstInChannel(layer);
                    }
                    break;
                case 2: // Drum pad
                    cursorDevice.selectFirstInKeyPad(childIndex);
                    break;
            }
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
        // itemCount observer will trigger sendDeviceList() with debounce
    }

    /**
     * Exit to parent device
     */
    public void exitToParent() {
        cursorDevice.selectParent();
        // itemCount observer will trigger sendDeviceList() with debounce
    }
}
