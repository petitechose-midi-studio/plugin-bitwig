package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;
import util.DeviceTypeUtils;
import java.util.List;
import java.util.ArrayList;
import handler.controller.DeviceController;

/**
 * DeviceHost - Observes Bitwig Device and sends updates TO controller
 *
 * RESPONSIBILITIES:
 * 1. Observe device changes (name, enabled, parameters) → send protocol messages
 * 2. Send device lists/children on request
 * 3. Manage parameter echo suppression (via DeviceController)
 *
 * DELEGATES TO:
 * - DeviceNavigator: Hierarchical navigation (slots/layers/drums)
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
    private DeviceController deviceController;
    private DeviceNavigator navigator;

    private String lastDeviceName = "";
    private int lastPageIndex = -1;

    // Prevent duplicate calls during transitions
    private boolean deviceChangePending = false;  // Ignores individual param messages during device change
    private boolean deviceListPending = false;  // Debounce for sendDeviceList
    private boolean pageChangePending = false;  // Debounce for sendPageChange

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

    public void setupObservers() {
        // Create banks
        layerBank = cursorDevice.createLayerBank(BitwigConfig.MAX_CHILDREN);
        drumPadBank = cursorDevice.createDrumPadBank(BitwigConfig.MAX_CHILDREN);

        // Create navigator (needs banks)
        navigator = new DeviceNavigator(host, cursorDevice, deviceBank, layerBank);

        // Mark interested + add observers by domain
        setupCursorTrackObservables();
        setupCursorDeviceObservables();
        setupRemoteControlsObservables();
        setupDeviceBankObservables();
        setupLayerAndDrumBanksObservables();
    }

    private void setupCursorTrackObservables() {
        cursorTrack.name().markInterested();

        // Reset device name cache when track changes, so device observer will trigger sendDeviceChange
        // Note: TrackHost now handles TrackChangeMessage (name, color, position, type)
        cursorTrack.name().addValueObserver(trackName -> {
            lastDeviceName = "";  // Reset so device observer will trigger sendDeviceChange
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

        // Page change observer - debounced, skip if device change will handle it
        remoteControls.selectedPageIndex().addValueObserver(pageIndex -> {
            if (pageIndex != lastPageIndex) {
                lastPageIndex = pageIndex;
                if (!deviceChangePending && !pageChangePending) {
                    pageChangePending = true;
                    host.scheduleTask(() -> {
                        pageChangePending = false;
                        if (!deviceChangePending) {
                            sendPageChange();
                        }
                    }, BitwigConfig.PAGE_CHANGE_MS);
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
                protocol.send(new DeviceRemoteControlValueChangeMessage(paramIndex, (float) value, displayValue, isEcho));
            });

            param.name().addValueObserver(name -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain all names
                protocol.send(new DeviceRemoteControlNameChangeMessage(paramIndex, name));
            });
        }
    }

    private void setupDeviceBankObservables() {
        deviceBank.itemCount().markInterested();
        deviceBank.scrollPosition().markInterested();
        deviceBank.canScrollBackwards().markInterested();
        deviceBank.canScrollForwards().markInterested();

        // Observer for device count changes (add/remove device)
        deviceBank.itemCount().addValueObserver(count -> {
            host.println("[DEVICE HOST] Device count changed: " + count);
            sendDeviceList();
        });

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
     * Note: TrackHost now handles TrackChangeMessage
     */
    public void sendInitialState() {
        // Delay to let Bitwig API initialize values
        host.scheduleTask(() -> {
            sendDeviceChange();
        }, BitwigConfig.DEVICE_CHANGE_HEADER_MS);
    }

    public void setDeviceController(DeviceController deviceController) {
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

            final List<String> names = new ArrayList<>();
            final String[] pageNamesArray = remoteControls.pageNames().get();
            for (int i = 0; i < pageCount; i++) {
                if (pageNamesArray != null && i < pageNamesArray.length) {
                    names.add(pageNamesArray[i]);
                } else {
                    names.add("");
                }
            }

            protocol.send(new DevicePageNamesMessage(pageCount, currentIndex, names));
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    public void sendDeviceList() {
        // Debounce: skip if already pending
        if (deviceListPending) return;
        deviceListPending = true;

        // Delay to let Bitwig API update values
        host.scheduleTask(() -> {
            deviceListPending = false;
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
            int deviceTypeInt = DeviceTypeUtils.toInt(deviceTypeRaw);
            host.println("[DeviceList] " + device.name().get() + " -> deviceType='" + deviceTypeRaw + "' (" + deviceTypeInt + ")");

            list.add(new DeviceListMessage.Devices(
                device.position().get(),
                device.name().get(),
                device.isEnabled().get(),
                deviceTypeInt,
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

            host.println("[DEVICE HOST] DeviceChildren: deviceIndex=" + deviceIndex + ", count=" + allChildren.size());

            protocol.send(new DeviceChildrenMessage(deviceIndex, 0, allChildren.size(), allChildren));
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    /**
     * Execute device navigation: enter child (slot/layer/drum)
     * Delegates to DeviceNavigator
     */
    public void enterDeviceChild(int deviceIndex, int itemType, int childIndex) {
        navigator.enterDeviceChild(deviceIndex, itemType, childIndex);
    }

    /**
     * Execute device navigation: exit to parent
     * Delegates to DeviceNavigator
     */
    public void exitToParent() {
        navigator.exitToParent();
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

    private static final String[] MIDI_NOTE_NAMES = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    private String getMidiNoteName(int midiNote) {
        int octave = (midiNote / 12) - 1;
        int noteIndex = midiNote % 12;
        return MIDI_NOTE_NAMES[noteIndex] + octave;
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

    private void sendDeviceCleared() {
        // Block individual observers during clear - no messages sent
        // Controller keeps current display until a real device is selected
        deviceChangePending = true;

        // Small delay then resume observers (in case device is quickly reselected)
        host.scheduleTask(() -> {
            deviceChangePending = false;
        }, BitwigConfig.STANDARD_DELAY_MS);
    }

    private void sendPageChange() {
        final int pageIndex = remoteControls.selectedPageIndex().get();
        final int pageCount = remoteControls.pageCount().get();
        final String pageName = remoteControls.getName().get();
        final List<DevicePageChangeMessage.RemoteControls> remoteControlsList = buildRemoteControlsListForPageChange();

        protocol.send(new DevicePageChangeMessage(
            new DevicePageChangeMessage.PageInfo(pageIndex, pageCount, pageName),
            remoteControlsList
        ));

        // Resume individual observers
        deviceChangePending = false;
    }

    private String getPageName(int pageIndex, int pageCount) {
        if (pageCount > 0 && pageIndex >= 0 && pageIndex < pageCount) {
            String[] names = remoteControls.pageNames().get();
            if (names != null && pageIndex < names.length) {
                return names[pageIndex];
            }
        }
        return "";
    }

    private List<DevicePageChangeMessage.RemoteControls> buildRemoteControlsListForPageChange() {
        List<DevicePageChangeMessage.RemoteControls> remoteControlsList = new ArrayList<>();
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            ParameterData data = captureParameterData(param, i);

            remoteControlsList.add(new DevicePageChangeMessage.RemoteControls(
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
        return remoteControlsList;
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
}
