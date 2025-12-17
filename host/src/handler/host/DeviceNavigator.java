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
    private final ControllerHost host;
    private final CursorDevice cursorDevice;
    private final DeviceBank deviceBank;
    private final DeviceLayerBank layerBank;

    public DeviceNavigator(
        ControllerHost host,
        CursorDevice cursorDevice,
        DeviceBank deviceBank,
        DeviceLayerBank layerBank
    ) {
        this.host = host;
        this.cursorDevice = cursorDevice;
        this.deviceBank = deviceBank;
        this.layerBank = layerBank;
    }

    public void enterDeviceChild(int deviceIndex, int itemType, int childIndex) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) {
            host.println("[DEVICE NAV] ⚠ enterDeviceChild: device " + deviceIndex + " doesn't exist");
            return;
        }

        String deviceName = device.name().get();
        host.println("\n[DEVICE NAV] ▶ Entering: device=" + deviceName + " (" + deviceIndex
                + "), itemType=" + itemType + " (" + getItemTypeString(itemType) + "), childIndex=" + childIndex + "\n");

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
                        host.println("[DEVICE NAV] → selectFirstInSlot: " + slotNames[childIndex]);
                        cursorDevice.selectFirstInSlot(slotNames[childIndex]);
                    }
                    break;
                case 1: // Layer
                    DeviceLayer layer = layerBank.getItemAt(childIndex);
                    if (layer.exists().get()) {
                        host.println("[DEVICE NAV] → selectFirstInChannel: layer " + childIndex);
                        layer.selectInEditor();
                        cursorDevice.selectFirstInChannel(layer);
                    }
                    break;
                case 2: // Drum pad
                    host.println("[DEVICE NAV] → selectFirstInKeyPad: " + childIndex);
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
        host.println("\n[DEVICE NAV] ◀ Exit to parent\n");
        cursorDevice.selectParent();
        // itemCount observer will trigger sendDeviceList() with debounce
    }

    private String getItemTypeString(int itemType) {
        switch (itemType) {
            case 0: return "SLOT";
            case 1: return "LAYER";
            case 2: return "DRUM";
            default: return "UNKNOWN";
        }
    }
}
