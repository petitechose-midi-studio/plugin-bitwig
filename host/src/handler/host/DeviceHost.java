package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.*;
import config.BitwigConfig;
import util.ColorUtils;
import java.util.List;
import java.util.ArrayList;

/**
 * DeviceHost - Observes Bitwig Device and sends updates TO controller
 *
 * Monitors device state changes (name, enabled, parameters) and sends
 * protocol messages to hardware controller.
 *
 * SINGLE RESPONSIBILITY: Bitwig → Controller (Device)
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

    private long lastDeviceChangeTime = 0;
    private static final long DEVICE_CHANGE_THROTTLE_MS = 50;

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
        layerBank = cursorDevice.createLayerBank(16);
        drumPadBank = cursorDevice.createDrumPadBank(16);

        layerBank.itemCount().markInterested();
        layerBank.scrollPosition().markInterested();
        drumPadBank.scrollPosition().markInterested();
        drumPadBank.canScrollBackwards().markInterested();
        drumPadBank.canScrollForwards().markInterested();

        for (int i = 0; i < 16; i++) {
            DeviceLayer layer = layerBank.getItemAt(i);
            layer.exists().markInterested();
            layer.name().markInterested();
        }

        for (int i = 0; i < 16; i++) {
            DrumPad pad = drumPadBank.getItemAt(i);
            pad.exists().markInterested();
            pad.name().markInterested();
        }

        cursorDevice.exists().markInterested();
        cursorDevice.name().markInterested();
        cursorDevice.isEnabled().markInterested();

        cursorTrack.name().markInterested();
        cursorTrack.color().markInterested();
        cursorTrack.position().markInterested();
        cursorDevice.position().markInterested();
        cursorDevice.deviceChain().name().markInterested();
        cursorDevice.isNested().markInterested();
        cursorDevice.hasSlots().markInterested();
        cursorDevice.slotNames().markInterested();
        cursorDevice.hasLayers().markInterested();
        cursorDevice.hasDrumPads().markInterested();
        remoteControls.selectedPageIndex().markInterested();
        remoteControls.pageCount().markInterested();
        remoteControls.getName().markInterested();
        remoteControls.pageNames().markInterested();

        deviceBank.itemCount().markInterested();
        deviceBank.scrollPosition().markInterested();
        deviceBank.canScrollBackwards().markInterested();
        deviceBank.canScrollForwards().markInterested();

        for (int i = 0; i < 32; i++) {
            Device device = deviceBank.getItemAt(i);
            device.exists().markInterested();
            device.name().markInterested();
            device.position().markInterested();
            device.isEnabled().markInterested();
            device.hasSlots().markInterested();
            device.hasLayers().markInterested();
            device.hasDrumPads().markInterested();
            device.slotNames().markInterested();
        }

        for (int i = 0; i < 8; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            param.exists().markInterested();
            param.value().markInterested();
            param.name().markInterested();
            param.value().getOrigin().markInterested();
            param.hasAutomation().markInterested();
            param.value().discreteValueCount().markInterested();
            param.value().displayedValue().markInterested();
            param.value().discreteValueNames().markInterested();
        }

        cursorDevice.exists().addValueObserver(exists -> {
            lastDeviceName = "";
            if (exists) {
                host.scheduleTask(() -> sendDeviceChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            } else {
                sendDeviceCleared();
            }
        });

        cursorDevice.name().addValueObserver(deviceName -> {
            if (cursorDevice.exists().get() && !deviceName.equals(lastDeviceName)) {
                lastDeviceName = deviceName;
                host.scheduleTask(() -> sendDeviceChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        });

        cursorTrack.name().addValueObserver(trackName -> {
            if (!trackName.equals(lastTrackName)) {
                lastTrackName = trackName;
                lastDeviceName = "";
                host.scheduleTask(() -> sendTrackChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
                host.scheduleTask(() -> sendDeviceChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS + 60);
            }
        });

        cursorTrack.color().addValueObserver((r, g, b) -> {
            long trackColor = ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
            if (trackColor != lastTrackColor) {
                lastTrackColor = trackColor;
                host.scheduleTask(() -> sendTrackChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        });

        cursorDevice.isEnabled().addValueObserver(isEnabled -> {
            int deviceIndex = cursorDevice.position().get();
            protocol.send(new DeviceStateChangeMessage(deviceIndex, isEnabled));
        });

        remoteControls.selectedPageIndex().addValueObserver(pageIndex -> {
            if (pageIndex != lastPageIndex) {
                lastPageIndex = pageIndex;
                sendPageChange();
            }
        });

        for (int i = 0; i < 8; i++) {
            final int paramIndex = i;
            RemoteControl param = remoteControls.getParameter(i);

            param.value().displayedValue().addValueObserver(displayValue -> {
                double value = param.value().get();
                boolean isEcho = deviceController != null && deviceController.consumeEcho(paramIndex);
                protocol.send(new DeviceMacroValueChangeMessage(paramIndex, (float) value, displayValue, isEcho));
            });

            param.name().addValueObserver(name -> {
                protocol.send(new DeviceMacroNameChangeMessage(paramIndex, name));
            });
        }
    }

    public void sendInitialState() {
        sendTrackChange();
        sendDeviceChange();
    }

    public void setDeviceController(handler.controller.DeviceController deviceController) {
        this.deviceController = deviceController;
    }

    public void sendPageNames() {
        int pageCount = remoteControls.pageCount().get();
        int currentIndex = remoteControls.selectedPageIndex().get();

        java.util.List<String> names = new java.util.ArrayList<>();
        for (int i = 0; i < pageCount; i++) {
            try {
                names.add(remoteControls.pageNames().get(i));
            } catch (ArrayIndexOutOfBoundsException e) {
                names.add("");
            }
        }

        protocol.send(new DevicePageNamesMessage(pageCount, currentIndex, names));
    }

    public void sendDeviceList() {
        int totalDeviceCount = deviceBank.itemCount().get();
        int currentDevicePosition = cursorDevice.position().get();
        boolean isNested = cursorDevice.isNested().get();
        String parentName = isNested ? cursorDevice.deviceChain().name().get() : "";

        List<DeviceListMessage.Devices> devicesList = new ArrayList<>();
        int bankSize = deviceBank.getSizeOfBank();

        for (int i = 0; i < bankSize; i++) {
            Device device = deviceBank.getItemAt(i);

            if (device.exists().get()) {
                String name = device.name().get();
                int position = device.position().get();
                boolean enabled = device.isEnabled().get();

                int[] childrenTypes = new int[4];
                int index = 0;
                if (device.hasSlots().get() && index < 4) childrenTypes[index++] = 1;
                if (device.hasLayers().get() && index < 4) childrenTypes[index++] = 2;
                if (device.hasDrumPads().get() && index < 4) childrenTypes[index++] = 4;

                devicesList.add(new DeviceListMessage.Devices(
                    position,
                    name,
                    enabled,
                    childrenTypes
                ));
            }
        }

        int currentDeviceIndex = 0;
        for (int i = 0; i < devicesList.size(); i++) {
            if (devicesList.get(i).getDeviceIndex() == currentDevicePosition) {
                currentDeviceIndex = i;
                break;
            }
        }

        protocol.send(new DeviceListMessage(
            totalDeviceCount,
            currentDeviceIndex,
            isNested,
            parentName,
            devicesList
        ));
    }

    public void sendDeviceChildren(int deviceIndex, int childType) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) {
            protocol.send(new DeviceChildrenMessage(deviceIndex, childType, 0, new ArrayList<>()));
            return;
        }

        // Build flat list of ALL children (slots + layers + drums), max 16
        List<DeviceChildrenMessage.Children> allChildren = new ArrayList<>();

        // Add slots first (itemType=0)
        for (DeviceChildrenMessage.Children slot : getSlots(device)) {
            if (allChildren.size() >= 16) break;
            allChildren.add(slot);
        }

        // Add layers (itemType=1)
        for (DeviceChildrenMessage.Children layer : getLayers(device)) {
            if (allChildren.size() >= 16) break;
            allChildren.add(layer);
        }

        // Add drum pads (itemType=2)
        for (DeviceChildrenMessage.Children drum : getAllDrumPads(device)) {
            if (allChildren.size() >= 16) break;
            allChildren.add(drum);
        }

        protocol.send(new DeviceChildrenMessage(deviceIndex, childType, allChildren.size(), allChildren));
    }

    public void enterDeviceChild(int deviceIndex, int childType, int childIndex) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) return;

        cursorDevice.selectDevice(device);

        host.scheduleTask(() -> {
            switch (childType) {
                case 1:
                    String[] slotNames = device.slotNames().get();
                    if (childIndex >= 0 && childIndex < slotNames.length) {
                        cursorDevice.selectFirstInSlot(slotNames[childIndex]);
                    }
                    break;
                case 2:
                    DeviceLayer layer = layerBank.getItemAt(childIndex);
                    if (layer.exists().get()) {
                        layer.selectInEditor();
                        host.scheduleTask(() -> cursorDevice.selectFirstInChannel(layer), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
                    }
                    break;
                case 3:
                    cursorDevice.selectFirstInKeyPad(childIndex);
                    break;
            }
            host.scheduleTask(() -> sendDeviceList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
        }, BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    public void exitToParent() {
        cursorDevice.selectParent();
        host.scheduleTask(() -> sendDeviceList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    private List<DeviceChildrenMessage.Children> getSlots(Device device) {
        List<DeviceChildrenMessage.Children> slots = new ArrayList<>();
        String[] slotNames = device.slotNames().get();

        if (slotNames != null) {
            for (int i = 0; i < slotNames.length && i < 16; i++) {
                slots.add(new DeviceChildrenMessage.Children(i, slotNames[i], 0)); // itemType=0 for Slot
            }
        }

        return slots;
    }

    private List<DeviceChildrenMessage.Children> getLayers(Device device) {
        List<DeviceChildrenMessage.Children> layers = new ArrayList<>();

        // Limit to 16 layers max, only active ones
        for (int i = 0; i < 16; i++) {
            DeviceLayer layer = layerBank.getItemAt(i);
            if (layer.exists().get()) {
                layers.add(new DeviceChildrenMessage.Children(i, layer.name().get(), 1)); // itemType=1 for Layer
            }
        }

        return layers;
    }

    private List<DeviceChildrenMessage.Children> getAllDrumPads(Device device) {
        List<DeviceChildrenMessage.Children> allPads = new ArrayList<>();

        while (drumPadBank.canScrollBackwards().get()) {
            drumPadBank.scrollBackwards();
        }

        do {
            int scrollPos = drumPadBank.scrollPosition().get();
            for (int i = 0; i < 16; i++) {
                if (allPads.size() >= 16) break; // Limit to 16 drum pads max
                DrumPad pad = drumPadBank.getItemAt(i);
                if (pad.exists().get()) {
                    int midiNote = scrollPos + i;
                    String padName = pad.name().get() + " (" + getMidiNoteName(midiNote) + ")";
                    allPads.add(new DeviceChildrenMessage.Children(midiNote, padName, 2)); // itemType=2 for DrumPad
                }
            }
            if (allPads.size() >= 16 || !drumPadBank.canScrollForwards().get()) break;
            drumPadBank.scrollForwards();
        } while (true);

        return allPads;
    }

    private String getMidiNoteName(int midiNote) {
        String[] noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        int octave = (midiNote / 12) - 1;
        int noteIndex = midiNote % 12;
        return noteNames[noteIndex] + octave;
    }

    private void sendDeviceChange() {
        long currentTime = System.currentTimeMillis();
        if (currentTime - lastDeviceChangeTime < DEVICE_CHANGE_THROTTLE_MS) return;
        lastDeviceChangeTime = currentTime;

        String deviceName = cursorDevice.name().get();
        boolean isEnabled = cursorDevice.isEnabled().get();
        int pageIndex = remoteControls.selectedPageIndex().get();
        int pageCount = remoteControls.pageCount().get();
        String pageName = getPageName(pageIndex, pageCount);

        sendDeviceChangeHeader(deviceName, isEnabled, pageIndex, pageCount, pageName);

        for (int i = 0; i < 8; i++) {
            final int index = i;
            host.scheduleTask(() -> sendMacroUpdate(index), i * 5);
        }
    }

    private void sendDeviceChangeHeader(String deviceName, boolean isEnabled, int pageIndex, int pageCount, String pageName) {
        protocol.send(new DeviceChangeHeaderMessage(deviceName, isEnabled, new DeviceChangeHeaderMessage.PageInfo(pageIndex, pageCount, pageName)));
    }

    private void sendTrackChange() {
        String trackName = cursorTrack.name().get();
        long trackColor = ColorUtils.toUint32Hex(cursorTrack.color().get());
        int trackPosition = cursorTrack.position().get();

        protocol.send(new TrackChangeMessage(trackName, trackColor, trackPosition));
    }

    private void sendDeviceCleared() {
        protocol.send(new DeviceChangeHeaderMessage("", false, new DeviceChangeHeaderMessage.PageInfo(0, 0, "")));

        for (int i = 0; i < 8; i++) {
            final int index = i;
            host.scheduleTask(() -> {
                protocol.send(new DeviceMacroUpdateMessage(
                    (byte) index, "", 0.0f, "", 0.0f, false, (byte) 0, (short) 0, (byte) 0
                ));
            }, index * 5);
        }
    }

    private void sendMacroUpdate(int paramIndex) {
        RemoteControl param = remoteControls.getParameter(paramIndex);
        int discreteCount = param.value().discreteValueCount().get();

        if (deviceController != null) {
            deviceController.updateParameterMetadata(paramIndex, discreteCount);
        }

        ParameterTypeInfo typeInfo = detectParameterType(param, discreteCount);

        protocol.send(new DeviceMacroUpdateMessage(
            (byte) paramIndex,
            param.name().get(),
            (float) param.value().get(),
            param.value().displayedValue().get(),
            (float) param.value().getOrigin().get(),
            param.exists().get(),
            (byte) typeInfo.parameterType,
            (short) discreteCount,
            (byte) typeInfo.currentValueIndex
        ));

        if (typeInfo.parameterType == 2 && typeInfo.discreteValueNames.length > 0) {
            sendMacroDiscreteValues(paramIndex, typeInfo.discreteValueNames, typeInfo.currentValueIndex);
        }
    }

    private void sendMacroDiscreteValues(int paramIndex, String[] discreteNames, int currentIndex) {
        protocol.send(new DeviceMacroDiscreteValuesMessage((byte) paramIndex, java.util.Arrays.asList(discreteNames), (byte) currentIndex));
    }

    private void sendPageChange() {
        int pageIndex = remoteControls.selectedPageIndex().get();
        int pageCount = remoteControls.pageCount().get();
        protocol.send(new DevicePageChangeMessage(
            new DevicePageChangeMessage.PageInfo(pageIndex, pageCount, remoteControls.getName().get()),
            buildMacrosListForPageChange()
        ));
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
        for (int i = 0; i < 8; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            int discreteCount = param.value().discreteValueCount().get();

            if (deviceController != null) {
                deviceController.updateParameterMetadata(i, discreteCount);
            }

            ParameterTypeInfo typeInfo = detectParameterType(param, discreteCount);

            macrosList.add(new DevicePageChangeMessage.Macros(
                i,
                (float) param.value().get(),
                param.name().get(),
                (float) param.value().getOrigin().get(),
                param.exists().get(),
                (short) discreteCount,
                param.value().displayedValue().get(),
                typeInfo.parameterType,
                typeInfo.discreteValueNames,
                typeInfo.currentValueIndex
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
}
