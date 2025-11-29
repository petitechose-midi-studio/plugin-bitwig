package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;
import util.ColorUtils;
import util.DeviceTypeUtils;
import util.TrackTypeUtils;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceHost - Observes Bitwig Device and sends updates TO controller
 *
 * RESPONSIBILITIES:
 * 1. Observe device changes (name, enabled, parameters) → send protocol messages
 * 2. Execute device navigation (layers/slots/drums require DeviceLayerBank/DrumPadBank)
 * 3. Send device lists/children on request
 *
 * NOTE: Navigation logic is here because it needs access to layerBank/drumPadBank
 * which can't be shared with DeviceController (Bitwig API limitation)
 */
public class DeviceHost {
    private final ControllerHost host;
    private final Protocol protocol;
    private final CursorTrack cursorTrack;
    private final CursorDevice cursorDevice;
    private final CursorRemoteControlsPage remoteControls;
    private final DeviceBank deviceBank;

    private DeviceLayerBank layerBank;
    private DrumPadBank drumPadBank;
    private handler.controller.DeviceController deviceController;

    private String lastDeviceName = "";
    private int lastPageIndex = -1;
    private String lastTrackName = "";
    private long lastTrackColor = 0;

    // Prevent duplicate calls during transitions
    private boolean deviceChangePending = false;  // Ignores individual param messages during device change

    public DeviceHost(
        ControllerHost host,
        Protocol protocol,
        CursorTrack cursorTrack,
        CursorDevice cursorDevice,
        CursorRemoteControlsPage remoteControls,
        DeviceBank deviceBank
    ) {
        this.host = host;
        this.protocol = protocol;
        this.cursorTrack = cursorTrack;
        this.cursorDevice = cursorDevice;
        this.remoteControls = remoteControls;
        this.deviceBank = deviceBank;
    }

    public void setup() {
        // Create banks
        layerBank = cursorDevice.createLayerBank(16);
        drumPadBank = cursorDevice.createDrumPadBank(16);

        // Mark interested + add observers by domain
        setupCursorTrackObservables();
        setupCursorDeviceObservables();
        setupRemoteControlsObservables();
        setupDeviceBankObservables();
        setupLayerAndDrumBanksObservables();
    }

    private void setupCursorTrackObservables() {
        cursorTrack.name().markInterested();
        cursorTrack.color().markInterested();
        cursorTrack.position().markInterested();
        cursorTrack.trackType().markInterested();

        cursorTrack.name().addValueObserver(trackName -> {
            if (!trackName.equals(lastTrackName)) {
                lastTrackName = trackName;
                lastDeviceName = "";  // Reset so device observer will trigger sendDeviceChange
                sendTrackChange();
                // Note: sendDeviceChange() is NOT called here - it will be triggered
                // by cursorDevice.exists() or cursorDevice.name() observers when the device changes
            }
        });

        cursorTrack.color().addValueObserver((r, g, b) -> {
            long trackColor = ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
            if (trackColor != lastTrackColor) {
                lastTrackColor = trackColor;
                sendTrackChange();
            }
        });
    }

    private void setupCursorDeviceObservables() {
        cursorDevice.exists().markInterested();
        cursorDevice.name().markInterested();
        cursorDevice.isEnabled().markInterested();
        cursorDevice.position().markInterested();
        cursorDevice.deviceChain().name().markInterested();
        cursorDevice.isNested().markInterested();
        cursorDevice.hasSlots().markInterested();
        cursorDevice.slotNames().markInterested();
        cursorDevice.hasLayers().markInterested();
        cursorDevice.hasDrumPads().markInterested();
        cursorDevice.deviceType().markInterested();

        // Single observer for device changes - name() is triggered for both new devices and device switches
        cursorDevice.name().addValueObserver(deviceName -> {
            boolean exists = cursorDevice.exists().get();

            if (!exists) {
                // Device cleared
                lastDeviceName = "";
                sendDeviceCleared();
            } else if (!deviceName.equals(lastDeviceName)) {
                // New or different device
                lastDeviceName = deviceName;
                sendDeviceChange();
            }
        });
    }

    private void setupRemoteControlsObservables() {
        remoteControls.selectedPageIndex().markInterested();
        remoteControls.pageCount().markInterested();
        remoteControls.getName().markInterested();
        remoteControls.pageNames().markInterested();

        // Parameters
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            markParameterInterested(remoteControls.getParameter(i));
        }

        // Page change observer - skip if device change will handle it
        remoteControls.selectedPageIndex().addValueObserver(pageIndex -> {
            if (pageIndex != lastPageIndex) {
                lastPageIndex = pageIndex;
                if (!deviceChangePending) {
                    host.scheduleTask(() -> sendPageChange(), BitwigConfig.PAGE_CHANGE_MS);
                }
            }
        });

        // Parameter observers - skip during device transitions (batch message handles it)
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            final int paramIndex = i;
            RemoteControl param = remoteControls.getParameter(i);

            param.value().displayedValue().addValueObserver(displayValue -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain all values
                double value = param.value().get();
                boolean isEcho = deviceController != null && deviceController.consumeEcho(paramIndex);
                protocol.send(new DeviceMacroValueChangeMessage(paramIndex, (float) value, displayValue, isEcho));
            });

            param.name().addValueObserver(name -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain all names
                protocol.send(new DeviceMacroNameChangeMessage(paramIndex, name));
            });
        }
    }

    private void setupDeviceBankObservables() {
        deviceBank.itemCount().markInterested();
        deviceBank.scrollPosition().markInterested();
        deviceBank.canScrollBackwards().markInterested();
        deviceBank.canScrollForwards().markInterested();

        // All devices in bank
        for (int i = 0; i < BitwigConfig.MAX_BANK_SIZE; i++) {
            final int deviceIndex = i;
            Device device = deviceBank.getItemAt(i);
            markDeviceInterested(device);

            // Device enabled state observer
            device.isEnabled().addValueObserver(isEnabled -> {
                if (device.exists().get()) {
                    protocol.send(new DeviceStateChangeMessage(deviceIndex, isEnabled));
                }
            });
        }
    }

    private void setupLayerAndDrumBanksObservables() {
        layerBank.itemCount().markInterested();
        layerBank.scrollPosition().markInterested();
        drumPadBank.scrollPosition().markInterested();
        drumPadBank.canScrollBackwards().markInterested();
        drumPadBank.canScrollForwards().markInterested();

        for (int i = 0; i < BitwigConfig.MAX_CHILDREN; i++) {
            markLayerInterested(layerBank.getItemAt(i));
            markDrumPadInterested(drumPadBank.getItemAt(i));
        }
    }

    /**
     * Send current device state (called at startup)
     */
    public void sendInitialState() {
        // Delay to let Bitwig API initialize values
        host.scheduleTask(() -> {
            sendTrackChange();
            sendDeviceChange();
        }, BitwigConfig.DEVICE_CHANGE_HEADER_MS);
    }

    public void setDeviceController(handler.controller.DeviceController deviceController) {
        this.deviceController = deviceController;
    }

    /**
     * Schedule action after device bank update delay.
     * Used by TrackController when selecting track to auto-select first device.
     */
    public void scheduleAfterDeviceBankUpdate(Runnable action) {
        host.scheduleTask(action, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    public void sendPageNames() {
        // Delay to let Bitwig API update values
        host.scheduleTask(() -> {
            final int pageCount = remoteControls.pageCount().get();
            final int currentIndex = remoteControls.selectedPageIndex().get();

            final java.util.List<String> names = new java.util.ArrayList<>();
            for (int i = 0; i < pageCount; i++) {
                try {
                    names.add(remoteControls.pageNames().get(i));
                } catch (ArrayIndexOutOfBoundsException e) {
                    names.add("");
                }
            }

            protocol.send(new DevicePageNamesMessage(pageCount, currentIndex, names));
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    public void sendDeviceList() {
        // Delay to let Bitwig API update values
        host.scheduleTask(() -> {
            final int totalDeviceCount = deviceBank.itemCount().get();
            final int currentDevicePosition = cursorDevice.position().get();
            final boolean isNested = cursorDevice.isNested().get();
            final String parentName = isNested ? cursorDevice.deviceChain().name().get() : "";
            final List<DeviceListMessage.Devices> devicesList = buildDevicesList();
            final int currentDeviceIndex = findCurrentDeviceIndex(devicesList, currentDevicePosition);

            protocol.send(new DeviceListMessage(
                totalDeviceCount,
                currentDeviceIndex,
                isNested,
                parentName,
                devicesList
            ));
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    private List<DeviceListMessage.Devices> buildDevicesList() {
        List<DeviceListMessage.Devices> list = new ArrayList<>();
        int bankSize = deviceBank.getSizeOfBank();

        for (int i = 0; i < bankSize; i++) {
            Device device = deviceBank.getItemAt(i);
            if (!device.exists().get()) continue;

            String deviceTypeRaw = device.deviceType().get();
            host.println("[DEVICE LIST] " + device.name().get() + " -> deviceType raw=\"" + deviceTypeRaw + "\"");

            list.add(new DeviceListMessage.Devices(
                device.position().get(),
                device.name().get(),
                device.isEnabled().get(),
                DeviceTypeUtils.toInt(deviceTypeRaw),
                getDeviceChildrenTypes(device)
            ));
        }

        return list;
    }

    private int[] getDeviceChildrenTypes(Device device) {
        int[] types = new int[4];
        int index = 0;
        if (device.hasSlots().get() && index < 4) types[index++] = 1;
        if (device.hasLayers().get() && index < 4) types[index++] = 2;
        if (device.hasDrumPads().get() && index < 4) types[index++] = 4;
        return types;
    }

    private int findCurrentDeviceIndex(List<DeviceListMessage.Devices> devicesList, int currentDevicePosition) {
        for (int i = 0; i < devicesList.size(); i++) {
            if (devicesList.get(i).getDeviceIndex() == currentDevicePosition) {
                return i;
            }
        }
        return 0;
    }

    public void sendDeviceChildren(int deviceIndex, int childType) {
        // Delay to let Bitwig API update values
        host.scheduleTask(() -> {
            final Device device = deviceBank.getItemAt(deviceIndex);
            if (!device.exists().get()) {
                protocol.send(new DeviceChildrenMessage(deviceIndex, 0, 0, new ArrayList<>()));
                return;
            }

            // Build flat list of ALL children (slots + layers OR drums)
            final List<DeviceChildrenMessage.Children> allChildren = new ArrayList<>();

            // Add slots first (itemType=0)
            for (DeviceChildrenMessage.Children slot : getSlots(device)) {
                if (allChildren.size() >= BitwigConfig.MAX_CHILDREN) break;
                allChildren.add(slot);
            }

            // Get drum pads first to check if this is a drum device
            List<DeviceChildrenMessage.Children> drumPads = getAllDrumPads(device);
            boolean hasDrumPads = !drumPads.isEmpty();

            // Add layers only if NOT a drum device (layers duplicate drum pads)
            if (!hasDrumPads) {
                for (DeviceChildrenMessage.Children layer : getLayers(device)) {
                    if (allChildren.size() >= BitwigConfig.MAX_CHILDREN) break;
                    allChildren.add(layer);
                }
            }

            // Add drum pads (itemType=2)
            for (DeviceChildrenMessage.Children drum : drumPads) {
                if (allChildren.size() >= BitwigConfig.MAX_CHILDREN) break;
                allChildren.add(drum);
            }

            host.println("\n[DEVICE HOST] Sending DeviceChildren: deviceIndex=" + deviceIndex + ", childrenCount="
                    + allChildren.size());
            for (int i = 0; i < allChildren.size(); i++) {
                DeviceChildrenMessage.Children child = allChildren.get(i);
                host.println("  [" + i + "] " + child.getChildName() + " (itemType=" + child.getItemType() + "/"
                        + getItemTypeString(child.getItemType()) + ", childIndex=" + child.getChildIndex() + ")");
            }

            protocol.send(new DeviceChildrenMessage(deviceIndex, 0, allChildren.size(), allChildren));
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    /**
     * Execute device navigation: enter child (slot/layer/drum)
     * Called FROM DeviceController when controller sends navigation command
     *
     * Navigation logic is HERE (not in Controller) because:
     * - Needs layerBank/drumPadBank access (can't be shared with Controller)
     * - Bitwig API limitation: these banks must be created in Host
     */
    public void enterDeviceChild(int deviceIndex, int itemType, int childIndex) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) {
            host.println("[DEVICE HOST] ⚠ enterDeviceChild: device " + deviceIndex + " doesn't exist");
            return;
        }

        String deviceName = device.name().get();
        host.println("\n[DEVICE HOST] ▶ Entering device child: device=" + deviceName + " (" + deviceIndex
                + "), itemType=" + itemType + " (" + getItemTypeString(itemType) + "), childIndex=" + childIndex + "\n");

        // Execute Bitwig API navigation
        cursorDevice.selectDevice(device);

        // itemType: 0=slot, 1=layer, 2=drum
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

        // Wait for deviceBank to be ready, then send (100ms for enter)
        host.scheduleTask(() -> sendDeviceList(), BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    /**
     * Execute device navigation: exit to parent
     * Called FROM DeviceController when controller sends navigation command
     */
    public void exitToParent() {
        host.println("\n[DEVICE HOST] ◀ Exit to parent\n");

        // Execute Bitwig API navigation
        cursorDevice.selectParent();

        // Wait for deviceBank to be ready, then send (300ms for exit - industry standard)
        host.scheduleTask(() -> sendDeviceList(), BitwigConfig.DEVICE_EXIT_NESTED_MS);
    }

    private List<DeviceChildrenMessage.Children> getSlots(Device device) {
        List<DeviceChildrenMessage.Children> slots = new ArrayList<>();
        String[] slotNames = device.slotNames().get();

        if (slotNames != null) {
            for (int i = 0; i < slotNames.length && i < BitwigConfig.MAX_CHILDREN; i++) {
                slots.add(new DeviceChildrenMessage.Children(i, slotNames[i], 0)); // itemType=0 for Slot
            }
        }

        return slots;
    }

    private List<DeviceChildrenMessage.Children> getLayers(Device device) {
        List<DeviceChildrenMessage.Children> layers = new ArrayList<>();

        // Limit layers max, only active ones
        for (int i = 0; i < BitwigConfig.MAX_CHILDREN; i++) {
            DeviceLayer layer = layerBank.getItemAt(i);
            if (layer.exists().get()) {
                layers.add(new DeviceChildrenMessage.Children(i, layer.name().get(), 1)); // itemType=1 for Layer
            }
        }

        return layers;
    }

    private List<DeviceChildrenMessage.Children> getAllDrumPads(Device device) {
        List<DeviceChildrenMessage.Children> allPads = new ArrayList<>();
        int scrollPos = drumPadBank.scrollPosition().get();

        // Simple iteration like layers - no async scroll operations
        for (int i = 0; i < BitwigConfig.MAX_CHILDREN; i++) {
            DrumPad pad = drumPadBank.getItemAt(i);
            if (pad.exists().get()) {
                int midiNote = scrollPos + i;
                String padName = pad.name().get() + " (" + getMidiNoteName(midiNote) + ")";
                allPads.add(new DeviceChildrenMessage.Children(midiNote, padName, 2)); // itemType=2 for DrumPad
            }
        }

        return allPads;
    }

    private String getMidiNoteName(int midiNote) {
        String[] noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        int octave = (midiNote / 12) - 1;
        int noteIndex = midiNote % 12;
        return noteNames[noteIndex] + octave;
    }

    private void sendDeviceChange() {
        // Block individual observers - batch message will contain everything
        deviceChangePending = true;

        // Delay all reads to let Bitwig API update hasSlots/hasLayers/hasDrumPads
        // Uses longer delay (100ms) because child type properties populate slower than name/state
        host.scheduleTask(() -> {
            String deviceName = cursorDevice.name().get();
            boolean isEnabled = cursorDevice.isEnabled().get();
            String deviceTypeRaw = cursorDevice.deviceType().get();
            int deviceType = DeviceTypeUtils.toInt(deviceTypeRaw);
            int pageIndex = remoteControls.selectedPageIndex().get();
            int pageCount = remoteControls.pageCount().get();
            String pageName = getPageName(pageIndex, pageCount);
            List<Integer> childrenTypes = intArrayToList(getDeviceChildrenTypes(cursorDevice));

            host.println("[DEVICE] deviceType raw=\"" + deviceTypeRaw + "\" -> " + deviceType);

            sendDeviceChangeHeader(deviceName, isEnabled, deviceType, pageIndex, pageCount, pageName, childrenTypes);
            sendPageChange();
        }, BitwigConfig.DEVICE_CHANGE_HEADER_MS);
    }

    private void sendDeviceChangeHeader(String deviceName, boolean isEnabled, int deviceType, int pageIndex, int pageCount, String pageName, List<Integer> childrenTypes) {
        protocol.send(new DeviceChangeHeaderMessage(deviceName, isEnabled, deviceType, new DeviceChangeHeaderMessage.PageInfo(pageIndex, pageCount, pageName), childrenTypes));
    }

    private List<Integer> intArrayToList(int[] array) {
        List<Integer> list = new ArrayList<>();
        for (int value : array) {
            list.add(value);
        }
        return list;
    }

    private void sendTrackChange() {
        final String trackName = cursorTrack.name().get();
        final long trackColor = ColorUtils.toUint32Hex(cursorTrack.color().get());
        final int trackPosition = cursorTrack.position().get();
        final int trackType = TrackTypeUtils.toInt(cursorTrack.trackType().get());

        protocol.send(new TrackChangeMessage(trackName, trackColor, trackPosition, trackType));
    }


    private void sendDeviceCleared() {
        // Send header with empty values
        protocol.send(new DeviceChangeHeaderMessage("No Device", false, DeviceTypeUtils.UNKNOWN, new DeviceChangeHeaderMessage.PageInfo(0, 0, ""), new ArrayList<>()));

        // Send empty macros bundled in one message
        List<DevicePageChangeMessage.Macros> emptyMacros = new ArrayList<>();
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            emptyMacros.add(new DevicePageChangeMessage.Macros(
                i, 0.0f, "", 0.0f, false, (short) 0, "", 0, new String[0], 0
            ));
        }
        protocol.send(new DevicePageChangeMessage(
            new DevicePageChangeMessage.PageInfo(0, 0, ""),
            emptyMacros
        ));
    }

    private void sendPageChange() {
        final int pageIndex = remoteControls.selectedPageIndex().get();
        final int pageCount = remoteControls.pageCount().get();
        final String pageName = remoteControls.getName().get();
        final List<DevicePageChangeMessage.Macros> macrosList = buildMacrosListForPageChange();

        protocol.send(new DevicePageChangeMessage(
            new DevicePageChangeMessage.PageInfo(pageIndex, pageCount, pageName),
            macrosList
        ));

        // Resume individual observers
        deviceChangePending = false;
    }

    private String getPageName(int pageIndex, int pageCount) {
        if (pageCount > 0 && pageIndex >= 0 && pageIndex < pageCount) {
            try {
                return remoteControls.pageNames().get(pageIndex);
            } catch (ArrayIndexOutOfBoundsException e) {}
        }
        return "";
    }

    private List<DevicePageChangeMessage.Macros> buildMacrosListForPageChange() {
        List<DevicePageChangeMessage.Macros> macrosList = new ArrayList<>();
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            ParameterData data = captureParameterData(param, i);

            macrosList.add(new DevicePageChangeMessage.Macros(
                i,
                data.value,
                data.name,
                data.origin,
                data.exists,
                (short) data.discreteCount,
                data.displayedValue,
                data.typeInfo.parameterType,
                data.typeInfo.discreteValueNames,
                data.typeInfo.currentValueIndex
            ));
        }
        return macrosList;
    }

    private static class ParameterTypeInfo {
        final int parameterType;
        final String[] discreteValueNames;
        final int currentValueIndex;

        ParameterTypeInfo(int parameterType, String[] discreteValueNames, int currentValueIndex) {
            this.parameterType = parameterType;
            this.discreteValueNames = discreteValueNames;
            this.currentValueIndex = currentValueIndex;
        }
    }

    private static class ParameterData {
        final String name;
        final float value;
        final String displayedValue;
        final float origin;
        final boolean exists;
        final int discreteCount;
        final ParameterTypeInfo typeInfo;

        ParameterData(String name, float value, String displayedValue, float origin,
                     boolean exists, int discreteCount, ParameterTypeInfo typeInfo) {
            this.name = name;
            this.value = value;
            this.displayedValue = displayedValue;
            this.origin = origin;
            this.exists = exists;
            this.discreteCount = discreteCount;
            this.typeInfo = typeInfo;
        }
    }

    private ParameterData captureParameterData(RemoteControl param, int paramIndex) {
        int discreteCount = param.value().discreteValueCount().get();
        String name = param.name().get();
        float value = (float) param.value().get();
        String displayedValue = param.value().displayedValue().get();
        float origin = (float) param.value().getOrigin().get();
        boolean exists = param.exists().get();

        if (deviceController != null) {
            deviceController.updateParameterMetadata(paramIndex, discreteCount);
        }

        ParameterTypeInfo typeInfo = detectParameterType(param, discreteCount);

        return new ParameterData(name, value, displayedValue, origin, exists, discreteCount, typeInfo);
    }

    private ParameterTypeInfo detectParameterType(RemoteControl param, int discreteCount) {
        String[] discreteNames = param.value().discreteValueNames().get();
        if (discreteNames == null) discreteNames = new String[0];

        int currentIndex = 0;
        if (discreteCount > 0) {
            currentIndex = (int) Math.round(param.value().get() * (discreteCount - 1));
            currentIndex = Math.max(0, Math.min(currentIndex, discreteCount - 1));
        }

        int parameterType = 0;
        if (discreteCount == 2 && discreteNames.length >= 2) {
            String name0 = discreteNames[0].toLowerCase().trim();
            String name1 = discreteNames[1].toLowerCase().trim();
            boolean isButton = (name0.equals("off") && name1.equals("on")) ||
                               (name0.equals("disabled") && name1.equals("enabled")) ||
                               (name0.equals("no") && name1.equals("yes")) ||
                               (name0.equals("false") && name1.equals("true"));
            parameterType = isButton ? 1 : 2;
        } else if (discreteCount > 2) {
            parameterType = 2;
        }

        return new ParameterTypeInfo(parameterType, discreteNames, currentIndex);
    }

    private void markLayerInterested(DeviceLayer layer) {
        layer.exists().markInterested();
        layer.name().markInterested();
    }

    private void markDrumPadInterested(DrumPad pad) {
        pad.exists().markInterested();
        pad.name().markInterested();
    }

    private void markDeviceInterested(Device device) {
        device.exists().markInterested();
        device.name().markInterested();
        device.position().markInterested();
        device.isEnabled().markInterested();
        device.deviceType().markInterested();
        device.hasSlots().markInterested();
        device.hasLayers().markInterested();
        device.hasDrumPads().markInterested();
        device.slotNames().markInterested();
    }

    private void markParameterInterested(RemoteControl param) {
        param.exists().markInterested();
        param.value().markInterested();
        param.name().markInterested();
        param.value().getOrigin().markInterested();
        param.hasAutomation().markInterested();
        param.value().discreteValueCount().markInterested();
        param.value().displayedValue().markInterested();
        param.value().discreteValueNames().markInterested();
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
