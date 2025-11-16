package com.midi_studio.handler.host;

import com.bitwig.extension.controller.api.*;
import com.midi_studio.protocol.Protocol;
import com.midi_studio.protocol.struct.*;
import com.midi_studio.config.BitwigConfig;
import com.midi_studio.util.ColorUtils;
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

    // Pre-initialized banks for hierarchical navigation (MUST be created during init)
    private DeviceLayerBank layerBank;
    private DrumPadBank drumPadBank;

    // Reference to DeviceController for parameter metadata sync
    private com.midi_studio.handler.controller.DeviceController deviceController;

    // Cache for change detection
    private String lastDeviceName = "";
    private int lastPageIndex = -1;
    private String lastTrackName = "";
    private long lastTrackColor = 0;

    // Track parameter changes from protocol (to suppress echo)
    private final boolean[] ignoreNextParameterChange = new boolean[8];

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

    /**
     * Setup device state observers
     */
    public void setup() {
        // ===== CRITICAL: Create banks during setup() =====
        // MUST be created here, NOT on-demand (Bitwig restriction)
        layerBank = cursorDevice.createLayerBank(16);
        drumPadBank = cursorDevice.createDrumPadBank(16);

        // Mark bank observables as interested
        layerBank.itemCount().markInterested();
        layerBank.scrollPosition().markInterested();
        drumPadBank.scrollPosition().markInterested();
        drumPadBank.canScrollBackwards().markInterested();
        drumPadBank.canScrollForwards().markInterested();

        // Mark all layer items as interested (16 slots)
        for (int i = 0; i < 16; i++) {
            DeviceLayer layer = layerBank.getItemAt(i);
            layer.exists().markInterested();
            layer.name().markInterested();
        }

        // Mark all drum pad items as interested (16 slots)
        for (int i = 0; i < 16; i++) {
            DrumPad pad = drumPadBank.getItemAt(i);
            pad.exists().markInterested();
            pad.name().markInterested();
        }

        // Mark all observables as interested (required before .get() calls)
        cursorDevice.exists().markInterested();
        cursorDevice.name().markInterested();
        cursorDevice.isEnabled().markInterested();

        // Mark cursor track observables as interested (for track change events)
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

        // Mark DeviceBank observables as interested
        deviceBank.itemCount().markInterested();
        deviceBank.scrollPosition().markInterested();
        deviceBank.canScrollBackwards().markInterested();
        deviceBank.canScrollForwards().markInterested();

        // Mark all devices in bank as interested (mark ALL 32 slots, not just current count)
        // CRITICAL: Must mark ALL possible devices because bank content can change
        for (int i = 0; i < 32; i++) {  // Always 32 slots in bank
            Device device = deviceBank.getItemAt(i);
            device.exists().markInterested();
            device.name().markInterested();
            device.position().markInterested();
            device.isEnabled().markInterested();
            device.hasSlots().markInterested();
            device.hasLayers().markInterested();
            device.hasDrumPads().markInterested();
            device.slotNames().markInterested();     // For getSlots()
        }

        // Mark parameters as interested
        for (int i = 0; i < 8; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            param.exists().markInterested();
            param.value().markInterested();
            param.name().markInterested();
            param.value().getOrigin().markInterested();
            param.hasAutomation().markInterested();  // For touch automation debugging
            param.value().discreteValueCount().markInterested();  // For parameter type detection
            param.value().displayedValue().markInterested();      // For formatted value display
            param.value().discreteValueNames().markInterested();  // For discrete value names (API v23+)
        }

        // Device exists changed (device added/removed from track)
        cursorDevice.exists().addValueObserver(exists -> {
            if (exists) {
                // Device appeared - reset cache and send device change
                // This handles the case where user drags first device onto an empty track
                lastDeviceName = "";
                host.scheduleTask(() -> sendDeviceChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            } else {
                // Device disappeared - send cleared state immediately (no API calls needed)
                lastDeviceName = "";
                sendDeviceCleared();
            }
        });

        // Device name changed (user selects new device)
        cursorDevice.name().addValueObserver(deviceName -> {
            if (cursorDevice.exists().get() && !deviceName.equals(lastDeviceName)) {
                lastDeviceName = deviceName;
                host.scheduleTask(() -> sendDeviceChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        });

        // Track changed (user selects new track)
        // We observe both name and color, but send the full track context in one message
        cursorTrack.name().addValueObserver(trackName -> {
            if (!trackName.equals(lastTrackName)) {
                lastTrackName = trackName;
                // Reset device name cache to force device update even if same name
                lastDeviceName = "";
                // OPTIMIZED: Send both track and device updates quickly
                // Track change first (provides context)
                host.scheduleTask(() -> sendTrackChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
                // Device change right after (20ms total instead of 70ms)
                host.scheduleTask(() -> sendDeviceChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        });

        // Track color observer (for color changes without name change)
        cursorTrack.color().addValueObserver((r, g, b) -> {
            // RGB observer provides raw values, compute hex
            long trackColor = ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
            if (trackColor != lastTrackColor) {
                lastTrackColor = trackColor;
                host.scheduleTask(() -> sendTrackChange(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
            }
        });

        // Device enabled/bypassed state changed
        cursorDevice.isEnabled().addValueObserver(isEnabled -> {
            // Send state change for current device (index from position in bank)
            int deviceIndex = cursorDevice.position().get();
            protocol.send(new DeviceStateChangeMessage(deviceIndex, isEnabled));
        });

        // Page changed (user navigates remote control pages)
        remoteControls.selectedPageIndex().addValueObserver(pageIndex -> {
            if (pageIndex != lastPageIndex) {
                lastPageIndex = pageIndex;
                sendPageChange();
            }
        });

        // Parameter observers (8 macros)
        for (int i = 0; i < 8; i++) {
            final int paramIndex = i;
            RemoteControl param = remoteControls.getParameter(i);

            // Use displayedValue observer to ensure value/displayValue synchronization
            param.value().displayedValue().addValueObserver(displayValue -> {
                double value = param.value().get();

                host.println("[DeviceHost] >>> Observer fired: param=" + paramIndex + " value=" + value + " displayValue=" + displayValue);

                // Check if this is an echo of a controller change or a host-initiated change
                boolean isEcho = false;
                if (deviceController != null) {
                    isEcho = deviceController.consumeEcho(paramIndex);
                }

                host.println("[DeviceHost] Bitwig callback param=" + paramIndex + " value=" + value +
                            " displayValue=" + displayValue + (isEcho ? " (ECHO)" : " (HOST-INITIATED)"));

                // Send with isEcho flag: true=echo (skip on controller), false=host-initiated (apply on controller)
                DeviceMacroValueChangeMessage message = new DeviceMacroValueChangeMessage(
                    paramIndex,
                    (float) value,
                    displayValue,
                    isEcho
                );
                protocol.send(message);
            });

            // Parameter name changed
            param.name().addValueObserver(name -> {
                protocol.send(new DeviceMacroNameChangeMessage(
                    paramIndex,
                    name
                ));
            });
        }
    }

    /**
     * Send current device state (called at startup)
     */
    public void sendInitialState() {
        sendTrackChange();
        sendDeviceChange();
    }

    /**
     * Suppress next parameter change observer callback
     * Called by DeviceController when applying controller input to Bitwig
     */
    /**
     * Set DeviceController reference (for metadata sync)
     */
    public void setDeviceController(com.midi_studio.handler.controller.DeviceController deviceController) {
        this.deviceController = deviceController;
    }

    public void suppressNextParameterChange(int paramIndex) {
        if (paramIndex >= 0 && paramIndex < 8) {
            host.println("[DeviceHost] suppressNextParameterChange param=" + paramIndex + " FLAG SET");
            ignoreNextParameterChange[paramIndex] = true;
        }
    }

    /**
     * Send current page names list to controller
     * Called by DeviceController when REQUEST_DEVICE_PAGE_NAMES received
     */
    public void sendPageNames() {
        int pageCount = remoteControls.pageCount().get();
        int currentIndex = remoteControls.selectedPageIndex().get();

        // Build list of page names
        java.util.List<String> names = new java.util.ArrayList<>();
        for (int i = 0; i < pageCount; i++) {
            try {
                String pageName = remoteControls.pageNames().get(i);
                names.add(pageName);
            } catch (ArrayIndexOutOfBoundsException e) {
                // Page names not ready yet, add empty string
                names.add("");
            }
        }

        protocol.send(new DevicePageNamesMessage(pageCount, currentIndex, names));
    }

    /**
     * Send device list to controller with hierarchical navigation info
     * Called by DeviceController when REQUEST_DEVICE_LIST received
     */
    public void sendDeviceList() {
        int totalDeviceCount = deviceBank.itemCount().get();
        int currentDevicePosition = cursorDevice.position().get();
        boolean isNested = cursorDevice.isNested().get();
        String parentName = isNested ? cursorDevice.deviceChain().name().get() : "";

        // Build list of devices in bank
        List<DeviceListMessage.Devices> devicesList = new ArrayList<>();
        int bankSize = deviceBank.getSizeOfBank();

        for (int i = 0; i < bankSize; i++) {
            Device device = deviceBank.getItemAt(i);

            if (device.exists().get()) {
                String name = device.name().get();
                int position = device.position().get();
                boolean enabled = device.isEnabled().get();

                // Build childrenTypes array (not bitmap!)
                // Array elements: 1=hasSlots, 2=hasLayers, 4=hasDrumPads, 0=unused
                // Example: [1, 2, 0, 0] = has Slots AND Layers
                // CRITICAL: These values MUST be marked interested in setup()
                // We rely on setup() having marked them interested already
                int[] childrenTypes = new int[4];
                int index = 0;
                if (device.hasSlots().get() && index < 4) childrenTypes[index++] = 1;
                if (device.hasLayers().get() && index < 4) childrenTypes[index++] = 2;
                if (device.hasDrumPads().get() && index < 4) childrenTypes[index++] = 4;
                // Remaining elements are 0 by default

                devicesList.add(new DeviceListMessage.Devices(
                    position,
                    name,
                    enabled,
                    childrenTypes
                ));
            }
        }

        // Find current device index in the list
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

    /**
     * Send children list for a specific device and child type
     * Called by DeviceController when REQUEST_DEVICE_CHILDREN received
     *
     * @param deviceIndex Index in device bank
     * @param childType 1=Slots, 2=Layers, 3=DrumPads
     */
    public void sendDeviceChildren(int deviceIndex, int childType) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) {
            // Send empty list
            protocol.send(new DeviceChildrenMessage(
                deviceIndex,
                childType,
                0,
                new ArrayList<>()
            ));
            return;
        }

        List<DeviceChildrenMessage.Children> children = new ArrayList<>();

        switch (childType) {
            case 1: // Slots
                children = getSlots(device);
                break;
            case 2: // Layers
                children = getLayers(device);
                break;
            case 3: // DrumPads
                children = getAllDrumPads(device);
                break;
        }

        protocol.send(new DeviceChildrenMessage(
            deviceIndex,
            childType,
            children.size(),
            children
        ));
    }

    /**
     * Enter a device child (slot/layer/drum pad)
     * Called by DeviceController when ENTER_DEVICE_CHILD received
     *
     * @param deviceIndex Index in device bank
     * @param childType 1=Slots, 2=Layers, 3=DrumPads
     * @param childIndex Index of child (MIDI note 0-127 for drum pads)
     */
    public void enterDeviceChild(int deviceIndex, int childType, int childIndex) {
        Device device = deviceBank.getItemAt(deviceIndex);
        if (!device.exists().get()) return;

        // Select the device first
        cursorDevice.selectDevice(device);

        // Enter the child based on type
        switch (childType) {
            case 1: // Slot
                String[] slotNames = device.slotNames().get();
                if (childIndex >= 0 && childIndex < slotNames.length) {
                    cursorDevice.selectFirstInSlot(slotNames[childIndex]);
                }
                break;
            case 2: // Layer
                cursorDevice.selectFirstInLayer(childIndex);
                break;
            case 3: // DrumPad
                // childIndex is the MIDI note (0-127)
                cursorDevice.selectFirstInKeyPad(childIndex);
                break;
        }

        // After entering child, send updated device list showing devices in this chain
        // Wait for Bitwig to update the cursor to the new context
        host.scheduleTask(() -> sendDeviceList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    /**
     * Exit to parent device chain
     * Called by DeviceController when EXIT_TO_PARENT received
     */
    public void exitToParent() {
        cursorDevice.selectParent();
        // After exiting to parent, send updated device list
        // Wait for Bitwig to update the cursor to the parent context
        host.scheduleTask(() -> sendDeviceList(), BitwigConfig.CURSOR_UPDATE_DELAY_MS);
    }

    // ========================================================================
    // Helpers - Device Children Navigation
    // ========================================================================

    /**
     * Get all slots for a device
     * @param device Device to query
     * @return List of slot children
     */
    private List<DeviceChildrenMessage.Children> getSlots(Device device) {
        List<DeviceChildrenMessage.Children> slots = new ArrayList<>();
        String[] slotNames = device.slotNames().get();

        if (slotNames != null) {
            for (int i = 0; i < slotNames.length; i++) {
                slots.add(new DeviceChildrenMessage.Children(i, slotNames[i]));
            }
        }

        return slots;
    }

    /**
     * Get all layers for a device
     * @param device Device to query (unused, uses cursorDevice's layerBank)
     * @return List of layer children
     */
    private List<DeviceChildrenMessage.Children> getLayers(Device device) {
        List<DeviceChildrenMessage.Children> layers = new ArrayList<>();

        // Use pre-initialized layerBank (created in setup())
        // NOTE: layerBank is ALWAYS attached to cursorDevice, not the device parameter
        int layerCount = layerBank.itemCount().get();
        for (int i = 0; i < layerCount && i < 16; i++) {
            DeviceLayer layer = layerBank.getItemAt(i);

            if (layer.exists().get()) {
                String name = layer.name().get();
                layers.add(new DeviceChildrenMessage.Children(i, name));
            }
        }

        return layers;
    }

    /**
     * Get ALL drum pads for a device with full scroll
     * CRITICAL: Scrolls through entire drum pad bank to get all pads
     *
     * @param device Device to query (unused, uses cursorDevice's drumPadBank)
     * @return List of drum pad children with MIDI note notation
     */
    private List<DeviceChildrenMessage.Children> getAllDrumPads(Device device) {
        List<DeviceChildrenMessage.Children> allPads = new ArrayList<>();

        // Use pre-initialized drumPadBank (created in setup())
        // NOTE: drumPadBank is ALWAYS attached to cursorDevice, not the device parameter

        // Scroll to beginning
        while (drumPadBank.canScrollBackwards().get()) {
            drumPadBank.scrollBackwards();
        }

        // Iterate through all windows
        do {
            int scrollPos = drumPadBank.scrollPosition().get();

            for (int i = 0; i < 16; i++) {
                DrumPad pad = drumPadBank.getItemAt(i);
                if (pad.exists().get()) {
                    int midiNote = scrollPos + i;  // MIDI note (0-127)
                    String name = pad.name().get();
                    String notation = getMidiNoteName(midiNote);
                    String displayName = name + " (" + notation + ")";

                    allPads.add(new DeviceChildrenMessage.Children(midiNote, displayName));
                }
            }

        } while (drumPadBank.canScrollForwards().get() && scrollForward(drumPadBank));

        return allPads;
    }

    /**
     * Scroll drum pad bank forward (helper with side effect)
     * @param bank Bank to scroll
     * @return true if scrolled
     */
    private boolean scrollForward(DrumPadBank bank) {
        bank.scrollForwards();
        return true;
    }

    /**
     * Convert MIDI note number to international notation
     * International standard: C3 = MIDI 60
     * Drum pads typically start at C1 (MIDI 36)
     *
     * @param midiNote MIDI note number (0-127)
     * @return Note name with octave (e.g., "C1", "D#2")
     */
    private String getMidiNoteName(int midiNote) {
        String[] noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        int octave = (midiNote / 12) - 1;  // International standard: C3 = MIDI 60
        int noteIndex = midiNote % 12;
        return noteNames[noteIndex] + octave;
    }

    // ========================================================================
    // Helpers - Device State
    // ========================================================================

    private void sendDeviceChange() {
        String deviceName = cursorDevice.name().get();
        boolean isEnabled = cursorDevice.isEnabled().get();

        // Build page info
        int pageIndex = remoteControls.selectedPageIndex().get();
        int pageCount = remoteControls.pageCount().get();
        String pageName = getPageName(pageIndex, pageCount);

        // OPTIMIZED FLOW: Send header immediately (observer already has delay)
        sendDeviceChangeHeader(deviceName, isEnabled, pageIndex, pageCount, pageName);

        // Send macros with minimal stagger (5ms) to avoid MIDI buffer overflow
        // Total time: 35ms for all 8 macros (vs 180ms before)
        for (int i = 0; i < 8; i++) {
            final int index = i;
            host.scheduleTask(() -> sendMacroUpdate(index), i * 5);
        }
    }

    /**
     * Send lightweight device header (NEW - optimized for speed)
     */
    private void sendDeviceChangeHeader(String deviceName, boolean isEnabled,
                                        int pageIndex, int pageCount, String pageName) {
        protocol.send(new DeviceChangeHeaderMessage(
            deviceName,
            isEnabled,
            new DeviceChangeHeaderMessage.PageInfo(pageIndex, pageCount, pageName)
        ));
    }

    /**
     * Send track context change (NEW - granular track updates)
     */
    private void sendTrackChange() {
        String trackName = cursorTrack.name().get();
        long trackColor = ColorUtils.toUint32Hex(cursorTrack.color().get());
        int trackPosition = cursorTrack.position().get();

        protocol.send(new TrackChangeMessage(trackName, trackColor, trackPosition));
    }

    /**
     * Send device cleared state (no device on track)
     * Clears device name and hides all parameters
     */
    private void sendDeviceCleared() {
        // Send empty header
        protocol.send(new DeviceChangeHeaderMessage(
            "",      // Empty device name
            false,   // Device not enabled (doesn't matter)
            new DeviceChangeHeaderMessage.PageInfo(0, 0, "")  // Empty page info
        ));

        // Send 8 empty parameters to hide all widgets
        for (int i = 0; i < 8; i++) {
            protocol.send(new DeviceMacroUpdateMessage(
                (byte) i,                      // Parameter index
                "",                            // Empty name
                0.0f,                          // Default value
                "",                            // Empty display value
                0.0f,                          // Default origin
                false,                         // parameterExists = false (hides widget)
                (byte) 0,                      // Knob type (doesn't matter)
                (short) 0,                     // No discrete values
                (byte) 0                       // No current index
            ));
        }
    }

    /**
     * Send individual macro update with optional discrete values (NEW)
     */
    private void sendMacroUpdate(int paramIndex) {
        RemoteControl param = remoteControls.getParameter(paramIndex);

        boolean exists = param.exists().get();
        int discreteCount = param.value().discreteValueCount().get();
        String displayValue = param.value().displayedValue().get();

        // Sync parameter metadata to DeviceController for echo suppression logic
        if (deviceController != null) {
            deviceController.updateParameterMetadata(paramIndex, discreteCount);
        }

        // Detect parameter type
        ParameterTypeInfo typeInfo = detectParameterType(param);

        // Send macro update
        protocol.send(new DeviceMacroUpdateMessage(
            (byte) paramIndex,
            param.name().get(),
            (float) param.value().get(),
            displayValue,
            (float) param.value().getOrigin().get(),
            exists,
            (byte) typeInfo.parameterType,
            (short) discreteCount,
            (byte) typeInfo.currentValueIndex
        ));

        // If this is a List parameter, send discrete values immediately after
        if (typeInfo.parameterType == 2 && typeInfo.discreteValueNames.length > 0) {
            sendMacroDiscreteValues(paramIndex, typeInfo.discreteValueNames, typeInfo.currentValueIndex);
        }
    }

    /**
     * Send discrete value names for List parameters (NEW)
     */
    private void sendMacroDiscreteValues(int paramIndex, String[] discreteNames, int currentIndex) {
        protocol.send(new DeviceMacroDiscreteValuesMessage(
            (byte) paramIndex,
            java.util.Arrays.asList(discreteNames),
            (byte) currentIndex
        ));
    }

    private void sendPageChange() {
        int pageIndex = remoteControls.selectedPageIndex().get();
        int pageCount = remoteControls.pageCount().get();
        String pageName = remoteControls.getName().get();

        DevicePageChangeMessage.PageInfo pageInfo = new DevicePageChangeMessage.PageInfo(
            pageIndex,
            pageCount,
            pageName
        );

        List<DevicePageChangeMessage.Macros> macrosList = buildMacrosListForPageChange();

        protocol.send(new DevicePageChangeMessage(pageInfo, macrosList));
    }

    private String getPageName(int pageIndex, int pageCount) {
        if (pageCount > 0 && pageIndex >= 0 && pageIndex < pageCount) {
            try {
                return remoteControls.pageNames().get(pageIndex);
            } catch (ArrayIndexOutOfBoundsException e) {
                // pageNames not ready yet
            }
        }
        return "";
    }


    private List<DevicePageChangeMessage.Macros> buildMacrosListForPageChange() {
        List<DevicePageChangeMessage.Macros> macrosList = new ArrayList<>();
        for (int i = 0; i < 8; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            int discreteCount = param.value().discreteValueCount().get();

            // Sync parameter metadata to DeviceController for echo suppression logic
            if (deviceController != null) {
                deviceController.updateParameterMetadata(i, discreteCount);
            }

            // Detect parameter type and extract metadata
            ParameterTypeInfo typeInfo = detectParameterType(param);

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

    // ========================================================================
    // Helpers - Parameter Type Detection
    // ========================================================================

    /**
     * Helper class to store parameter type detection results
     */
    private static class ParameterTypeInfo {
        final int parameterType;          // 0=Knob, 1=Button, 2=List
        final String[] discreteValueNames; // Array of discrete value names (empty array if continuous)
        final int currentValueIndex;      // Current index in discreteValueNames (0 if continuous)

        ParameterTypeInfo(int parameterType, String[] discreteValueNames, int currentValueIndex) {
            this.parameterType = parameterType;
            this.discreteValueNames = discreteValueNames;
            this.currentValueIndex = currentValueIndex;
        }
    }

    /**
     * Detect parameter type and extract discrete value metadata
     */
    private ParameterTypeInfo detectParameterType(RemoteControl param) {
        int discreteCount = param.value().discreteValueCount().get();
        double normalizedValue = param.value().get();

        String[] discreteNames = param.value().discreteValueNames().get();
        if (discreteNames == null) {
            discreteNames = new String[0];
        }

        int currentIndex = 0;
        if (discreteCount > 0) {
            currentIndex = (int) Math.round(normalizedValue * (discreteCount - 1));
            currentIndex = Math.max(0, Math.min(currentIndex, discreteCount - 1));
        }

        int parameterType = 0;

        if (discreteCount == 2 && discreteNames.length >= 2) {
            String name0 = discreteNames[0].toLowerCase().trim();
            String name1 = discreteNames[1].toLowerCase().trim();

            boolean isButton =
                (name0.equals("off") && name1.equals("on")) ||
                (name0.equals("disabled") && name1.equals("enabled")) ||
                (name0.equals("no") && name1.equals("yes")) ||
                (name0.equals("false") && name1.equals("true"));

            parameterType = isButton ? 1 : 2;
        } else if (discreteCount > 2) {
            parameterType = 2;
        }

        // Use actual discrete names without padding (etl::vector on embedded side)
        // Only send non-empty names to save bandwidth and memory
        return new ParameterTypeInfo(parameterType, discreteNames, currentIndex);
    }
}
